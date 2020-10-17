#include <benchmark/benchmark.h>
#include "sfr.h"

__attribute__((noinline))
double SlugBenchmarkLoop(benchmark::State& state, slugWrapper& SlugOb)
{
  // benchmark
  double numSerializations = 0;
  for (auto _ : state) {
    // This part of the code gets timed.
    // (Tests on my MacBook Pro show this takes about ~3 milliseconds/iteration.)

    // serialize
    slug_cluster_state_noyields state;
    SlugOb.serializeCluster(state);

    // deserialize
    slugWrapper new_SlugOb(state);

    numSerializations += 1.;
  }
  return numSerializations;
}

static void SlugSerializeDeserialize(benchmark::State& state)
{
  // setup

  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6;            // years
  double t = 0.;                          // years
  slugWrapper SlugOb(particle_mass);

  // advance in time
  constexpr int max_timesteps = 30;
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    SlugOb.advanceToTime(t);
  }

  auto numSerializations = SlugBenchmarkLoop(state, SlugOb);

  state.counters["SerializeDeserializeRate"] = benchmark::Counter(numSerializations, benchmark::Counter::kIsRate);
}
BENCHMARK(SlugSerializeDeserialize);
BENCHMARK_MAIN();

// ➜  build git:(master) ✗ bin/slug_benchmark
// 2020-10-09 11:42:27
// Running bin/slug_benchmark
// Run on (2 X 2800 MHz CPU s)
// CPU Caches:
//   L1 Data 32K (x2)
//   L1 Instruction 32K (x2)
//   L2 Unified 256K (x2)
//   L3 Unified 8192K (x2)
// Load Average: 0.24, 0.28, 0.31
// -----------------------------------------------------------------------------------
// Benchmark                         Time             CPU   Iterations UserCounters...
// -----------------------------------------------------------------------------------
// SlugSerializeDeserialize    3263614 ns      3261646 ns          324 SerializeDeserializeRate=306.594/s
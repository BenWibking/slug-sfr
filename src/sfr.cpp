#include <gtest/gtest.h>
#include "sfr.h"

#define PRINT_SIZES

TEST(SlugWrapperTest, SerializesDeserializes)
{
  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6;            // years
  double t = 0.;                          // years
  slugWrapper SlugOb;
  SlugOb.constructCluster(particle_mass);
  std::cout << "Constructed cluster of mass = " << particle_mass << " Msun." << std::endl;

  // advance in time
  constexpr int max_timesteps = 10;
  std::vector<double> yields(NISO_SUKHBOLD16, 0.0);
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    const std::vector<double> delta_yields = SlugOb.advanceToTime(t);
    for(size_t i = 0; i < delta_yields.size(); ++i) {
      yields[i] += delta_yields[i];
    }
  }

  std::cout << "Advanced star cluster to time t = " << t << " years." << std::endl;
  std::cout << "Yields = (";
  constexpr size_t max_niso = 10;
  const auto max_yields_size = std::min(yields.size(), max_niso);
  for (size_t i = 0; i < (max_yields_size - 1); ++i)
  {
    std::cout << yields[i] << ", ";
  }
  std::cout << yields[max_yields_size - 1] << ")" << std::endl;

  // serialize
  // NOTE: key insight -> yields never need to be serialized!
  //        we only ever care about the incremental yield at a given timestep!!
  //
  //  We assume that the user will *always* call get_yield() after calling advance().
  //  (Otherwise, yield information will obviously be lost.)
  //
  slug_cluster_state<NISO_SUKHBOLD16> state;
  SlugOb.serializeCluster(state);

#if defined(PRINT_SIZES)
  // print sizes of structs (in bytes)
  slug_cluster_state<0> state_small;
  std::cout << "slug_cluster_state<" << NISO_SUKHBOLD16 << "> is "
            << sizeof(state) << " bytes." << std::endl;
  std::cout << "slug_cluster_state<0> is " << sizeof(state_small) << " bytes." << std::endl;
#endif

// SLUG structs
// slug_cluster_state<302> is 7472 bytes.
// slug_cluster_state<0> is 224 bytes.
//
// GIZMO structs
// Size of particle structure     352  [bytes]
// Size of hydro-cell structure   640  [bytes]
// 
// For low-res AGORA:
// 81.5839 MByte for particle storage.
// 143.051 MByte for storage of hydro data.
//
// Therefore, 23.1% increase in memory usage if slug_cluster_state<0>
// is added to particle_data (neglecting alignment padding, which causes additional overhead)

  // deserialize
  slugWrapper new_SlugOb;
  new_SlugOb.reconstructCluster(state);

  // check whether old and new object are equal
  auto oldData = (SlugOb.cluster)->make_tuple();
  auto newData = (new_SlugOb.cluster)->make_tuple();

  for_each2(oldData, newData, [](auto index, auto &&oldDataElem, auto &&newDataElem) {
    EXPECT_EQ(oldDataElem, newDataElem) << "Tuple elements at index "
                      << index
                      << " (type = "
                      << type_name<decltype(oldDataElem)>()
                      << ") are not equal!";
  });

}
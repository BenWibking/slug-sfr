#include "slug_wrapper.h"

constexpr auto do_stochastic_only = true; // true is okay if only radioisotopes
constexpr auto minimum_stochastic_mass = 9.0; // minimum SNe mass is 9 Msun for Sukhbold16
constexpr auto stochastic_sampling_type = POISSON;
constexpr auto imf_type = "chabrier";
constexpr auto stellar_tracks = "modp020.dat";
constexpr auto spectral_synthesis = "sb99";
constexpr auto spectral_filter = "QH0";
constexpr auto yield_table = "SNII_Sukhbold16_nodecay";
constexpr auto compute_yields = true;

constexpr auto slug_cluster_internal_ID = 1;
constexpr auto slug_cluster_internal_time = 0.;

void slugWrapper::constructCluster(double particle_mass)
{
  cluster = new slug_cluster(
      slug_cluster_internal_ID, particle_mass, slug_cluster_internal_time,
      slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type),
      slug_predef.tracks(stellar_tracks),
      slug_predef.specsyn(spectral_synthesis, slug_predef.tracks(stellar_tracks),
                          slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type)),
      slug_predef.filter_set(spectral_filter), nullptr, nullptr,
      slug_predef.yields(yield_table), nullptr,
      slug_predef.ostreams, nullptr, do_stochastic_only, compute_yields);
}

// Method to reconstruct the slug_cluster object from a serialized buffer
void slugWrapper::reconstructCluster(slug_cluster_state_noyields &state)
{
  cluster = new slug_cluster(
      state,
      slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type),
      slug_predef.tracks(stellar_tracks),
      slug_predef.specsyn(spectral_synthesis, slug_predef.tracks(stellar_tracks),
                          slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type)),
      slug_predef.filter_set(spectral_filter), nullptr, nullptr,
      slug_predef.yields(yield_table), nullptr,
      slug_predef.ostreams, nullptr, do_stochastic_only, compute_yields);
}

void slugWrapper::serializeCluster(slug_cluster_state_noyields &state)
{
  cluster->serializeToStructWithoutYields(state);
}

auto slugWrapper::advanceToTime(double particle_age) -> std::vector<double>
{
  const std::vector<double> yields_t0 = cluster->get_yield();
  cluster->advance(particle_age);
  const std::vector<double> yields_t1 = cluster->get_yield();

  std::vector<double> delta_yields(yields_t0.size());
  
  for(size_t i=0; i < delta_yields.size(); ++i) {
    delta_yields[i] = std::max(yields_t1[i] - yields_t0[i], 0.0);
  }

  return delta_yields;
}

int slugWrapper::getStochasticSN()
{
  return cluster->get_stoch_sn();
}

double slugWrapper::getBirthMass()
{
  return cluster->get_birth_mass();
}

double slugWrapper::getStellarMass()
{
  return cluster->get_stellar_mass();
}

double slugWrapper::getPhotometryQH0()
{
  return cluster->get_photometry()[0];
}
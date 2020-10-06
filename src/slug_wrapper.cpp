#include "slug_wrapper.h"

constexpr auto do_stochastic_only = false;
constexpr auto minimum_stochastic_mass = 8.0;
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
template <int N>
void slugWrapper::reconstructCluster(slug_cluster_state<N> &state)
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
// explicitly instantiate template for N = NISO_SUKHBOLD16
template void slugWrapper::reconstructCluster(slug_cluster_state<NISO_SUKHBOLD16> &state);

template <int N>
void slugWrapper::serializeCluster(slug_cluster_state<N> &state)
{
  cluster->serializeToStruct(state);
}
// explicitly instantiate template for N = NISO_SUKHBOLD16
template void slugWrapper::serializeCluster(slug_cluster_state<NISO_SUKHBOLD16> &state);

void slugWrapper::advanceToTime(double particle_age)
{
  cluster->advance(particle_age);
}

int slugWrapper::get_stoch_sn()
{
  return cluster->get_stoch_sn();
}

double slugWrapper::get_birth_mass()
{
  return cluster->get_birth_mass();
}

double slugWrapper::get_stellar_mass()
{
  return cluster->get_stellar_mass();
}

double slugWrapper::get_photometry_QH0()
{
  return cluster->get_photometry()[0];
}
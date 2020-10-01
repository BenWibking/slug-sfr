#include "slug_object.h"

constexpr auto do_stochastic_only = false;
constexpr auto minimum_stochastic_mass = 9.0;
constexpr auto stochastic_sampling_type = POISSON;
constexpr auto imf_type = "chabrier";
constexpr auto stellar_tracks = "modp020.dat";
constexpr auto spectral_synthesis = "sb99";
constexpr auto spectral_filter = "QH0";
constexpr auto yield_table = "SNII_Sukhbold16_nodecay";

constexpr auto slug_cluster_internal_ID = 1;
constexpr auto slug_cluster_internal_time = 0.;

void slug_object::construct_cluster(double particle_mass)
{
  cluster = new slug_cluster(
      slug_cluster_internal_ID, particle_mass, slug_cluster_internal_time,
      slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type),
      slug_predef.tracks(stellar_tracks),
      slug_predef.specsyn(spectral_synthesis, slug_predef.tracks(stellar_tracks),
                          slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type)),
      slug_predef.filter_set(spectral_filter), nullptr, nullptr,
      slug_predef.yields(yield_table), nullptr,
      slug_predef.ostreams, nullptr, do_stochastic_only);
}

// Method to reconstruct the slug_cluster object from a serialized buffer
void slug_object::reconstruct_cluster(char *buf)
{
  cluster = new slug_cluster(
      (slug_cluster_buffer *)buf,
      slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type),
      slug_predef.tracks(stellar_tracks),
      slug_predef.specsyn(spectral_synthesis, slug_predef.tracks(stellar_tracks),
                          slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type)),
      slug_predef.filter_set(spectral_filter), nullptr, nullptr,
      slug_predef.yields(yield_table), nullptr,
      slug_predef.ostreams, nullptr, do_stochastic_only);
}

// Method to reconstruct the slug_cluster object from a serialized buffer
template <int N>
void slug_object::reconstruct_cluster_from_struct(slug_cluster_state<N> &state)
{
  cluster = new slug_cluster(
      state,
      slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type),
      slug_predef.tracks(stellar_tracks),
      slug_predef.specsyn(spectral_synthesis, slug_predef.tracks(stellar_tracks),
                          slug_predef.imf(imf_type, minimum_stochastic_mass, stochastic_sampling_type)),
      slug_predef.filter_set(spectral_filter), nullptr, nullptr,
      slug_predef.yields(yield_table), nullptr,
      slug_predef.ostreams, nullptr, do_stochastic_only);
}
// explicitly instantiate template
template void slug_object::reconstruct_cluster_from_struct(slug_cluster_state<NISO_SUKHBOLD16> &state);

template<int N>
void slug_object::serialize_cluster_to_struct(slug_cluster_state<N> &state)
{
  cluster->serializeToStruct(state);
}
// explicitly instantiate template
template void slug_object::serialize_cluster_to_struct(slug_cluster_state<NISO_SUKHBOLD16> &state);

// Method to return the member functions of the slug_cluster
void slug_object::pack_buffer(char *buf)
{
  cluster->pack_buffer((slug_cluster_buffer *)buf);
}
int slug_object::buffer_size() { return cluster->buffer_size(); }
void slug_object::advance(double particle_age)
{
  cluster->advance(particle_age);
}
int slug_object::get_stoch_sn() { return cluster->get_stoch_sn(); }
double slug_object::get_birth_mass() { return cluster->get_birth_mass(); }
double slug_object::get_stellar_mass() { return cluster->get_stellar_mass(); }
double slug_object::get_photometry_QH0()
{
  return cluster->get_photometry()[0];
}

extern "C"
{
  slug_object *slug_object_new() { return new slug_object(); }
  void slug_object_delete(slug_object *S) { delete S; }
  void slug_construct_cluster(slug_object *S, double particle_mass)
  {
    S->construct_cluster(particle_mass);
  }
  void slug_reconstruct_cluster(slug_object *S, char *buf)
  {
    S->reconstruct_cluster(buf);
  }
  void slug_pack_buffer(slug_object *S, char *buf) { S->pack_buffer(buf); }
  int slug_buffer_size(slug_object *S) { return S->buffer_size(); }
  void slug_advance(slug_object *S, double particle_age)
  {
    return S->advance(particle_age);
  }
  int slug_get_stoch_sn(slug_object *S) { return S->get_stoch_sn(); }
  double slug_get_birth_mass(slug_object *S) { return S->get_birth_mass(); }
  double slug_get_stellar_mass(slug_object *S) { return S->get_stellar_mass(); }
  double slug_get_photometry_QH0(slug_object *S)
  {
    return S->get_photometry_QH0();
  }
}

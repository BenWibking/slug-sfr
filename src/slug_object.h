#ifndef slug_object_h
#define slug_object_h

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <slug.H>
#include <slug_cluster.H>
#include <slug_predefined.H>

class slug_object {
public:
  // Constructor
  slug_object() { cluster = nullptr; }

  // Destructor; this frees the slug_cluster object
  virtual ~slug_object() {
    delete cluster;
    cluster = nullptr;
  }

  // Method to construct the slug_cluster object from particle mass
  void construct_cluster(double particle_mass);

  // Method to reconstruct the slug_cluster object from a serialized buffer
  //void reconstruct_cluster(char *buf);

  template<int N>
  void reconstruct_cluster_from_struct(slug_cluster_state<N> &state);

  template<int N>
  void serialize_cluster_to_struct(slug_cluster_state<N> &state);

  // Method to return the member functions of the slug_cluster
  //void pack_buffer(char *buf);
  //int buffer_size();
  void advance_to_time(double particle_age); // particle_age [yr]
  int get_stoch_sn();
  double get_birth_mass();
  double get_stellar_mass();
  double get_photometry_QH0(); // ionising luminosity [photon/s]

  // This is a pointer to the slug_cluster object
  slug_cluster *cluster;
};
#else
typedef struct slug_object slug_object;
#endif

// File wrapper
#ifdef __cplusplus
extern "C" {
#endif

slug_object *slug_object_new();
void slug_object_delete(slug_object *SlugOb);
void slug_construct_cluster(slug_object *SlugOb, double particle_mass);
void slug_reconstruct_cluster(slug_object *SlugOb, char *buf);
void slug_pack_buffer(slug_object *SlugOb, char *buf);
int slug_buffer_size(slug_object *SlugOb);
void slug_advance_to_time(slug_object *SlugOb, double particle_age);
int slug_get_stoch_sn(slug_object *SlugOb);
double slug_get_birth_mass(slug_object *SlugOb);
double slug_get_stellar_mass(slug_object *SlugOb);
double slug_get_photometry_QH0(slug_object *SlugOb);

#ifdef __cplusplus
}
#endif
#endif

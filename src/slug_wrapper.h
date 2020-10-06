#ifndef SLUG_WRAPPER_H
#define SLUG_WRAPPER_H

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <slug.H>
#include <slug_cluster.H>
#include <slug_predefined.H>

class slugWrapper {
public:
  // Constructor
  slugWrapper() { cluster = nullptr; }

  // Destructor; this frees the slug_cluster object
  virtual ~slugWrapper() {
    delete cluster;
    cluster = nullptr;
  }

  // Method to construct the slug_cluster object from particle mass
  void constructCluster(double particle_mass);

  template<int N>
  void reconstructCluster(slug_cluster_state<N> &state);

  template<int N>
  void serializeCluster(slug_cluster_state<N> &state);

  void advanceToTime(double particle_age); // particle_age [yr]
  
  int get_stoch_sn();
  double get_birth_mass();
  double get_stellar_mass();
  double get_photometry_QH0(); // ionising luminosity [photon/s]

  // This is a pointer to the slug_cluster object
  slug_cluster *cluster;
};

#endif // SLUG_WRAPPER_H

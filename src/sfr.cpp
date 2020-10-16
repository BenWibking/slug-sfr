#include <gtest/gtest.h>
#include "sfr.h"

#define PRINT_SIZE

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
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    SlugOb.advanceToTime(t);
  }
  std::cout << "Advanced star cluster to time t = " << t << " years." << std::endl;

  // serialize
  // NOTE: yields never need to be serialized!
  //       we only ever care about the incremental yield at a given timestep!!
  //       We assume that the user will *always* call get_yield() after calling advance().

  slug_cluster_state_noyields state;
  SlugOb.serializeCluster(state);

#if defined(PRINT_SIZE)
  // print size of struct (in bytes)
  std::cout << "slug_cluster_state_noyields is " << sizeof(state) << " bytes." << std::endl;
#endif

// SLUG structs
// slug_cluster_state_noyields is 224 bytes.
//
// GIZMO structs
// Size of particle structure     352  [bytes]
// Size of hydro-cell structure   640  [bytes]
// 
// For low-res AGORA:
// 81.5839 MByte for particle storage.
// 143.051 MByte for storage of hydro data.
//
// Therefore, 23.1% increase in memory usage if slug_cluster_state_noyields
// is added to particle_data (neglecting alignment padding, which causes additional overhead)

  // deserialize
  slugWrapper new_SlugOb;
  new_SlugOb.reconstructCluster(state);

  // advance both old and new objects by one additional timestep dt
  const double final_t = t + dt;

  SlugOb.advanceToTime(final_t);
  auto yields_last_timestep_old = SlugOb.getYieldsThisTimestep();

  new_SlugOb.advanceToTime(final_t);
  auto yields_last_timestep_new = new_SlugOb.getYieldsThisTimestep();

  // check whether incremental yields are equal to machine precision
  for(size_t i = 0; i < yields_last_timestep_new.size(); ++i) {
    double mean_y = 0.5*(yields_last_timestep_old[i] + yields_last_timestep_new[i]);
    double abs_error_bound = mean_y * 1.0e-13;

    EXPECT_NEAR(yields_last_timestep_old[i], yields_last_timestep_new[i], abs_error_bound)
      << "Element " << i << " differs by fractional amount "
      << std::abs((yields_last_timestep_old[i] - yields_last_timestep_new[i]) / mean_y);
  }

  // check whether old and new object are equal (does NOT compare cumulative yields)
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
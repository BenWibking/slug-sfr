#include <gtest/gtest.h>
#include "sfr.h"

#define PRINT_SIZE

TEST(SlugWrapperTest, SerializesDeserializes)
{
  // NOTE: currently this has only been tested for stochastic-only SLUG clusters
  //       (i.e., where do_stochastic_only = True [slug_wrapper.h]).

  // initialize slug_globals
  rng_type my_rng(42);
  slugWrapper::slug_globals = new slug_predefined(&my_rng);

  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6;            // years
  double t = 0.;                          // years
  slugWrapper SlugOb(particle_mass);
  std::cout << "Constructed cluster of mass = " << particle_mass << " Msun." << std::endl;

  // advance in time
  constexpr int max_timesteps = 10;
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    SlugOb.advanceToTime(t);
    auto yields = SlugOb.getYieldsThisTimestep();
  }
  std::cout << "Advanced star cluster to time t = " << t << " years." << std::endl;

  // serialize
  // NOTE: yields never need to be serialized!
  //       we only ever care about the incremental yield at a given timestep!

  slug_cluster_state_noyields state;
  SlugOb.serializeCluster(state);

#if defined(PRINT_SIZE)
  // print size of struct (in bytes) [should be 224 bytes]
  std::cout << "slug_cluster_state_noyields is " << sizeof(state) << " bytes." << std::endl;
#endif

  // deserialize
  slugWrapper new_SlugOb(state);

  // check ionizing photon luminosity
  // we have to check this prior to calling advanceToTime, since it models a use case in GIZMO
  EXPECT_DOUBLE_EQ(SlugOb.getPhotometryQH0(), new_SlugOb.getPhotometryQH0());

  // advance both old and new objects by one additional timestep dt
  const double final_t = t + dt;
  SlugOb.advanceToTime(final_t);
  auto yields_last_timestep_old = SlugOb.getYieldsThisTimestep();

  new_SlugOb.advanceToTime(final_t);
  auto yields_last_timestep_new = new_SlugOb.getYieldsThisTimestep();

  // check number of SNe this timestep
  EXPECT_EQ(SlugOb.getNumberSNeThisTimestep(), new_SlugOb.getNumberSNeThisTimestep());

  // check number of non-dead stars this timestep
  EXPECT_EQ(SlugOb.getNumberAliveStochasticStars(), new_SlugOb.getNumberAliveStochasticStars());

  // check ejecta mass this timestep
  constexpr double abs_err_tol_mass = 1.0e-13;
  EXPECT_NEAR(SlugOb.getEjectaMassThisTimestep(),
              new_SlugOb.getEjectaMassThisTimestep(), abs_err_tol_mass);

  // check ionizing photon luminosity (again, after a new timestep)
  EXPECT_DOUBLE_EQ(SlugOb.getPhotometryQH0(), new_SlugOb.getPhotometryQH0());

  // check yields this timestep
  for(size_t i = 0; i < yields_last_timestep_new.size(); ++i) {
    EXPECT_NEAR(yields_last_timestep_old[i], yields_last_timestep_new[i], abs_err_tol_mass)
      << "Element " << i << " differs by amount "
      << std::abs(yields_last_timestep_old[i] - yields_last_timestep_new[i]);
  }

  // check whether old and new object are equal (does NOT compare *cumulative* yields)
  auto oldData = SlugOb.cluster.make_tuple();
  auto newData = new_SlugOb.cluster.make_tuple();

  for_each2(oldData, newData, [](auto index, auto &&oldDataElem, auto &&newDataElem) {
    EXPECT_EQ(oldDataElem, newDataElem) << "Tuple elements at index "
                      << index
                      << " (type = "
                      << type_name<decltype(oldDataElem)>()
                      << ") are not equal!";
  });

}
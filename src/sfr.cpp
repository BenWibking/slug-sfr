#include <gtest/gtest.h>
#include "sfr.h"

TEST(SlugObjectTest, SerializesDeserializes)
{
  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6;            // years
  double t = 0.;                          // years

  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);
  std::cout << "Constructed cluster of mass = " << particle_mass << " Msun." << std::endl;

  constexpr int max_timesteps = 10;
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    slug_advance(SlugOb, t);
  }
  std::cout << "Advanced star cluster to time t = " << t << " years." << std::endl;

  // serialize
  slug_cluster_state<NISO_SUKHBOLD16> state;
  SlugOb->serialize_cluster_to_struct(state);

  // print sizes of structs (in bytes)
  slug_cluster_state<2> state_small;
  std::cout << "slug_cluster_state<" << NISO_SUKHBOLD16 << "> is "
            << sizeof(state) << " bytes." << std::endl;
  std::cout << "slug_cluster_state<2> is " << sizeof(state_small) << " bytes." << std::endl;

  // deserialize
  slug_object *new_SlugOb = slug_object_new();
  new_SlugOb->reconstruct_cluster_from_struct(state);

  // check whether old and new object are equal
  auto oldData = SlugOb->cluster->make_tuple();
  auto newData = new_SlugOb->cluster->make_tuple();

  for_each2(oldData, newData, [](auto index, auto &&oldDataElem, auto &&newDataElem) {
    EXPECT_EQ(oldDataElem, newDataElem) << "Tuple elements at index "
                      << index
                      << " (type = "
                      << type_name<decltype(oldDataElem)>()
                      << ") are not equal!";
  });

}
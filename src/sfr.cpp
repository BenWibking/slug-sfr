#include <gtest/gtest.h>
#include "sfr.h"

TEST(SlugObjectTest, SerializesDeserializes)
{
  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6; // years

  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);
  std::cout << "Constructed cluster of mass = " << particle_mass << " Msun." << std::endl;

  constexpr int max_timesteps = 10;
  for(int i = 0; i < max_timesteps; ++i) {
    std::cout << "Advancing star cluster by dt = " << dt << std::endl;
    slug_advance(SlugOb, dt);
  }

  // serialize using old method
  size_t dimBuf = slug_buffer_size(SlugOb);
  char *buf_slug = (char *)malloc(dimBuf);
  slug_pack_buffer(SlugOb, buf_slug);

  // serialize using new method
  slug_cluster_state<NISO_SUKHBOLD16> state;
  SlugOb->serialize_cluster_to_struct(state);

  // deserialize using new method
  slug_object *new_SlugOb = slug_object_new();
  new_SlugOb->reconstruct_cluster_from_struct(state);

  // serialize reconstructed object using old method
  size_t dimBufNew = slug_buffer_size(new_SlugOb);
  char *buf_slugNew = (char *) malloc(dimBufNew);
  slug_pack_buffer(new_SlugOb, buf_slugNew);

  // write properties to stdout
  //SlugOb->cluster->write_prop(std::cout, ASCII, 0);
  //new_SlugOb->cluster->write_prop(std::cout, ASCII, 0);

  // check whether new object and old object are equal by comparing
  // serialized outputs from the old method
  ASSERT_EQ(dimBuf, dimBufNew);
  for(size_t i=0; i < dimBuf; ++i) {
    ASSERT_EQ(buf_slug[i], buf_slugNew[i]) << i;
  }
  std::cout << "Passed buffer check!" << std::endl;

  // check whether old and new object are equal using operator==
  ASSERT_EQ((*new_SlugOb->cluster), (*SlugOb->cluster));
  std::cout << "Passed operator== check!" << std::endl;
}
#include <gtest/gtest.h>
#include "sfr.h"

TEST(SlugObjectTest, SerializesDeserializes)
{
  // Slug part
  double particle_mass = 100.0; // solar masses

  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);

  size_t dimBuf = slug_buffer_size(SlugOb);
  char *buf_slug = (char *)malloc(dimBuf);
  slug_pack_buffer(SlugOb, buf_slug);

  slug_object *new_SlugOb = slug_object_new();
  slug_reconstruct_cluster(new_SlugOb, buf_slug);
  size_t dimBufNew = slug_buffer_size(new_SlugOb);
  char *buf_slugNew = (char *) malloc(dimBufNew);
  slug_pack_buffer(new_SlugOb, buf_slugNew);

  ASSERT_EQ(dimBuf, dimBufNew);
  for(size_t i=0; i < dimBuf; ++i) {
    ASSERT_EQ(buf_slug[i], buf_slugNew[i]);
  }

  delete SlugOb;
  delete new_SlugOb;
}

// otherwise the SLUG main() function is used
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
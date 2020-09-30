#include <gtest/gtest.h>
#include "sfr.h"

TEST(SlugObjectTest, SerializesDeserializes)
{
  // create slug_cluster object
  double particle_mass = 100.0; // solar masses
  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);

  // serialize using old method
  size_t dimBuf = slug_buffer_size(SlugOb);
  char *buf_slug = (char *)malloc(dimBuf);
  slug_pack_buffer(SlugOb, buf_slug);

  // TODO: serialize using *new* method


  // TODO: deserialize using *new* method
  slug_object *new_SlugOb = slug_object_new();


  // serialize reconstructed object using old method
  size_t dimBufNew = slug_buffer_size(new_SlugOb);
  char *buf_slugNew = (char *) malloc(dimBufNew);
  slug_pack_buffer(new_SlugOb, buf_slugNew);

  // check whether new object and old object are equal
  ASSERT_EQ(dimBuf, dimBufNew);
  for(size_t i=0; i < dimBuf; ++i) {
    ASSERT_EQ(buf_slug[i], buf_slugNew[i]);
  }
}
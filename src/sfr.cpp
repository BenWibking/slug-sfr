#include <gtest/gtest.h>
#include "sfr.h"

TEST(SlugObjectTest, SerializesDeserializes)
{
  // create slug_cluster object
  constexpr double particle_mass = 5.0e3; // solar masses
  constexpr double dt = 1.0e6; // years
  double t = 0.; // years

  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);
  std::cout << "Constructed cluster of mass = " << particle_mass << " Msun." << std::endl;

  constexpr int max_timesteps = 5;
  for (int i = 0; i < max_timesteps; ++i)
  {
    t += dt;
    slug_advance(SlugOb, t);

    // cluster->get_yield() has side effects -- this is a bug
    SlugOb->cluster->get_yield(); // this *must* be called, otherwise
                                  // yields will not be calculated from the previous timestep!
  }
  std::cout << "Advanced star cluster to time t = " << t << std::endl;

  // serialize using new method
  slug_cluster_state<NISO_SUKHBOLD16> state;
  SlugOb->serialize_cluster_to_struct(state);

  // print sizes of structs (in bytes)
  slug_cluster_state<2> state_small;
  std::cout << "slug_cluster_state<" << NISO_SUKHBOLD16 << "> is "
            << sizeof(state) << " bytes." << std::endl;
  std::cout << "slug_cluster_state<2> is " << sizeof(state_small) << " bytes." << std::endl;

  // deserialize using new method
  slug_object *new_SlugOb = slug_object_new();
  new_SlugOb->reconstruct_cluster_from_struct(state);

  // check whether old and new object are equal
  //EXPECT_EQ((*new_SlugOb->cluster), (*SlugOb->cluster));
  auto oldData = SlugOb->cluster->tied();
  auto newData = new_SlugOb->cluster->tied();
  for_each2(oldData, newData, [](auto index, auto &&oldDataElem, auto &&newDataElem) {
    EXPECT_EQ(oldDataElem, newDataElem) << "Tuple elements at index "
                      << index
                      << " (type = "
                      << type_name<decltype(oldDataElem)>()
                      << ") are not equal!";
  });

#if 0
  // print out vector of stellar masses (stochastic part)
  constexpr auto stars_index = 32;
  auto printStars = [](std::vector<double> stars)
  {
    for (size_t i = 0; i < (stars.size() - 1); ++i)
    {
      std::cout << stars[i] << ", ";
    }
    std::cout << stars[stars.size()-1];
    std::cout << std::endl;
  };

  auto oldStars = std::get<0>(std::get<stars_index>(oldData));
  auto newStars = std::get<0>(std::get<stars_index>(newData));
  std::cout << "old stars = ";
  printStars(oldStars);
  std::cout << "new stars = ";
  printStars(newStars);
#endif

#if 0
  // check whether new object and old object are equal by comparing
  // serialized outputs from the old method
  
  // serialize original object using old method
  size_t dimBuf = slug_buffer_size(SlugOb);
  char *buf_slug = (char *)malloc(dimBuf);
  slug_pack_buffer(SlugOb, buf_slug);

  // serialize reconstructed object using old method
  size_t dimBufNew = slug_buffer_size(new_SlugOb);
  char *buf_slugNew = (char *) malloc(dimBufNew);
  slug_pack_buffer(new_SlugOb, buf_slugNew);

  EXPECT_EQ(dimBuf, dimBufNew);
  for(size_t i=0; i < dimBuf; ++i) {
    ASSERT_EQ(buf_slug[i], buf_slugNew[i]) << i;
  }
#endif

}
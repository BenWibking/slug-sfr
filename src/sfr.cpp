#include "sfr.h"

auto main(int argc, char **argv) -> int {
  // test runner
  bool result = test_slug();

  if (result == true) {
    return 0;
  } else {
    return 1;
  }
}

auto test_slug() -> bool {
  // Slug part
  double particle_mass = 100.0; // solar masses
  slug_object *SlugOb = slug_object_new();
  slug_construct_cluster(SlugOb, particle_mass);

  size_t sizeSlug = slug_buffer_size(SlugOb);
  auto SlugOb_size = sizeSlug;
  auto SlugMass = slug_get_stellar_mass(SlugOb); // solar masses

  std::cout << "Expected cluster mass = " << particle_mass << std::endl;
  std::cout << "Actual star cluster mass = " << SlugMass << std::endl;

  size_t dimBuf = slug_buffer_size(SlugOb);
  char *buf_slug = (char *)malloc(dimBuf);
  slug_pack_buffer(SlugOb, buf_slug);
  slug_object_delete(SlugOb);

  slug_object *new_SlugOb = slug_object_new();
  slug_reconstruct_cluster(new_SlugOb, buf_slug);

  std::cout << "Success!" << std::endl;

  return true;
}

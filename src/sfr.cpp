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

/* Writes a boost::random::mersenne_twister_engine to a @c std::ostream */
template <class CharT, class Traits>
friend std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os,
           const boost::random::mersenne_twister_engine &mt)
{
    UIntType data[n];
    for (std::size_t j = 0; j < i; ++j)
    {
        data[j + n - i] = x[j];
    }
    if (i != n)
    {
        rewind(&data[n - i - 1], n - i);
    }
    os.write(reinterpret_cast<char*>(&data[0]), sizeof(data[0]));
    for (std::size_t j = 1; j < n; ++j)
    {
        // write in binary format
        os.write(reinterpret_cast<char*>(&data[j]), sizeof(data[j]);
    }
    return os;
}

/* Reads a boost::random::mersenne_twister_engine from a @c std::istream */
template <class CharT, class Traits>
friend std::basic_istream<CharT, Traits> &
operator>>(std::basic_istream<CharT, Traits> &is,
           boost::random::mersenne_twister_engine &mt)
{
    for (std::size_t j = 0; j < mt.state_size; ++j)
    {
        // read from binary format
        is.read(reinterpret_cast<char*>(&mt.x[j]), sizeof(mt.x[j]));
    }
    
    mt.i = mt.state_size; // this is to work around a compiler bug w/r/t templates
    return is;
}


auto test_slug() -> bool {
  // Serialize RNG
  

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

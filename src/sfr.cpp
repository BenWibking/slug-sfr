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

using rng = boost::random::mt19937;

#if 0
/* Writes a boost::random::mt19937 to a @c std::ostream */
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os,
           const rng &mt)
{
    std::size_t n = mt.state_size;
    std::size_t i = mt.i;

    uint32_t data[n];
    for (std::size_t j = 0; j < i; ++j)
    {
        data[j + n - i] = mt.x[j];
    }
    if (i != n)
    {
        rewind(&data[n - i - 1], n - i);
    }
    os.write(reinterpret_cast<char*>(&data[0]), sizeof(data[0]));
    for (std::size_t j = 1; j < n; ++j)
    {
        // write in binary format
        os.write(reinterpret_cast<char*>(&data[j]), sizeof(data[j]));
    }
    return os;
}

/* Reads a boost::random::mt19937 from a @c std::istream */
template <class CharT, class Traits>
std::basic_istream<CharT, Traits> &
operator>>(std::basic_istream<CharT, Traits> &is,
           rng &mt)
{
    for (std::size_t j = 0; j < mt.state_size; ++j)
    {
        // read from binary format
        is.read(reinterpret_cast<char*>(&mt.x[j]), sizeof(mt.x[j]));
    }
    
    mt.i = mt.state_size; // this is to work around a compiler bug w/r/t templates
    return is;
}
#endif

auto test_slug() -> bool {
  
  // Serialize RNG
  int seed = 47;
  rng *my_rng = new rng(seed);
  std::stringstream buffer;
  buffer << (*my_rng);

  int seq_len = 2;
  std::vector<uint32_t> random_seq(seq_len);
  my_rng->generate(random_seq.begin(), random_seq.end());
  std::cout << random_seq[0] << " " << random_seq[1] << std::endl;
  delete my_rng;

  rng *my_new_rng = new rng();
  buffer >> (*my_new_rng);
  std::vector<uint32_t> new_random_seq(seq_len);
  my_new_rng->generate(new_random_seq.begin(), new_random_seq.end());
  std::cout << new_random_seq[0] << " " << new_random_seq[1] << std::endl;
  delete my_new_rng;

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

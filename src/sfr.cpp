#include <gtest/gtest.h>
#include "sfr.h"

using rng = pcg32;

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

TEST(RngTest, SerializesDeserializes)
{
  // Serialize RNG
  int seed = 47;
  rng my_rng(seed);
  std::stringstream buffer;
  buffer << my_rng;
  rng my_saved_rng = my_rng;
  
#if 0
  {
    std::stringstream testbuffer(buffer.str());
    testbuffer.flags(std::ios_base::dec | std::ios_base::skipws);
    uint64_t multiplier, increment, state;
    auto space = std::string();
    {
      using pcg_extras::operator>>;
      testbuffer >> multiplier >> increment >> state;
    }

    if (!testbuffer.fail())
    {
      std::cout << "success " << multiplier << ' ' << increment << ' ' << state << std::endl;
    }
    else
    {
      std::cout << "failure " << multiplier << ' ' << increment << ' ' << state << std::endl;
    }
  }
#endif

  int seq_len = 2;
  std::vector<uint32_t> random_seq(seq_len);
  for(size_t i=0; i < random_seq.size(); ++i) {
    random_seq[i] = my_rng();
  }

  rng my_new_rng;
  buffer >> my_new_rng;
  ASSERT_FALSE(buffer.fail());
  ASSERT_EQ(my_saved_rng, my_new_rng);

  std::vector<uint32_t> new_random_seq(seq_len);
  for(size_t i=0; i < new_random_seq.size(); ++i) {
    new_random_seq[i] = my_new_rng();
  }

  ASSERT_EQ(random_seq[0], new_random_seq[0]);
  ASSERT_EQ(random_seq[1], new_random_seq[1]);
}

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
#include <gtest/gtest.h>
#include "pcg_random.hpp"

TEST(RngTest, SerializesDeserializes)
{
  // serialize RNG
  int seed = 48;
  using rng = pcg64;
  rng my_rng(seed);

  using state_t = pcg_detail::pcg128_t;
  using output_t = uint64_t;

  pcg_detail::RngState<state_t> saved_state;
  my_rng.serialize_to_struct(&saved_state);
  rng my_saved_rng = my_rng;
  
  int seq_len = 3;
  std::vector<output_t> random_seq(seq_len);
  for(size_t i=0; i < random_seq.size(); ++i) {
    random_seq[i] = my_rng();
    std::cout << "\trandom_seq[" << i << "] = " << random_seq[i] << std::endl;
  }

  // deserialize RNG
  rng my_new_rng;
  my_new_rng.deserialize_from_struct(&saved_state);

  ASSERT_EQ(my_saved_rng, my_new_rng);

  std::vector<output_t> new_random_seq(seq_len);
  for(size_t i=0; i < new_random_seq.size(); ++i) {
    new_random_seq[i] = my_new_rng();
    std::cout << "\tnew_random_seq[" << i << "] = " << new_random_seq[i] << std::endl;
    // check outputs
    ASSERT_EQ(random_seq[i], new_random_seq[i]);
  }
}

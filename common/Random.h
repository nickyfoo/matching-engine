#pragma once
#include <chrono>
#include <random>

namespace Random {
inline std::mt19937 generate() {
  std::random_device rd{};

  // Create seed_seq with clock and 7 random numbers from std::random_device
  std::seed_seq ss{
      static_cast<std::seed_seq::result_type>(
          std::chrono::steady_clock::now().time_since_epoch().count()),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd()};

  return std::mt19937{ss};
}

// Here's our global std::mt19937 object.
// The inline keyword means we only have one global instance for our whole
// program.
inline std::mt19937 mt{generate()};  // generates a seeded std::mt19937 and
                                     // copies it into our global object

// Generate a random int between [min, max] (inclusive)
inline int get(int min, int max) {
  return std::uniform_int_distribution{min, max}(mt);
}

}  // namespace Random

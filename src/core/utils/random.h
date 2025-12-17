#pragma once

#include <random>

namespace euclide {

class RandomGenerator {
  public:
    explicit RandomGenerator(uint32_t seed) : engine(seed) {}

    float random(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }

  private:
    std::mt19937 engine;
};

} // namespace euclide
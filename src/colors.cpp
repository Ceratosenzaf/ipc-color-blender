
#include <random>

float* getRandomColor() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  const float R = dist(mt);
  const float G = dist(mt);
  const float B = dist(mt);

  return new float[3]{R, G, B};
}
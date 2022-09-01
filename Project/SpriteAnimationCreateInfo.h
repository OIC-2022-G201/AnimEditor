#pragma once
#include <cstdint>
#include <vector>

struct AnimationPattern {
  double wait = 1.0;
  uint32_t no = 0;
  uint32_t step = 0;
};

struct SpriteAnimationCreateInfo {
  double offset_x = 0.0;
  double offset_y = 0.0;
  double width = 0.0;
  double height = 0.0;
  bool loop = false;
  std::vector<AnimationPattern> pattern{30};
};
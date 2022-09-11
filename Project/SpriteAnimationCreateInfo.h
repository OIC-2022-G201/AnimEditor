#pragma once
#include <cstdint>
#include <format>
#include <vector>
#include <string>
#include "FormatExtends.h"

struct AnimationPattern {
  double wait = 1.0;
  uint32_t no = 0;
  uint32_t step = 0;
};

struct SpriteAnimationCreateInfo {
  std::string name;
  double offset_x = 0.0;
  double offset_y = 0.0;
  double width = 0.0;
  double height = 0.0;
  bool loop = false;
  std::vector<AnimationPattern> pattern{30};
};

template <>
struct std::formatter<AnimationPattern>
    : std::formatter<const char*> {
  auto format(AnimationPattern patterns, format_context& ctx) {
    auto out = ctx.out();
    *out = '{';
    ++out;
    out = std::format_to(out, "{},{},{}", patterns.wait, patterns.no,patterns.step);
    *out = '}';
    ++out;
    return out;
  }
};

template <>
struct std::formatter<SpriteAnimationCreateInfo> : std::formatter<const char*> {
  auto format(SpriteAnimationCreateInfo item, format_context& ctx) {
    auto out = ctx.out();
    *out = '{';
    ++out;
    out = std::format_to(out, "\"{}\",\n{},{},\n{},{},\n{},{}", item.name, item.offset_x, item.offset_y,
        item.width,item.height
        ,item.loop,item.pattern);
    *out = '}';
    ++out;
    return out;
  }
};
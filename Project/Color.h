#pragma once
#include <cstdint>

struct Color {
  using value_type = uint8_t;

  /// @brief 赤 | Red
  value_type r;

  /// @brief 緑 | Green
  value_type g;

  /// @brief 青 | Blue
  value_type b;

  /// @brief アルファ | Alpha
  value_type a;
};

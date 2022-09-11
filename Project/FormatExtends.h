#pragma once
#include <vector>
#include <format>
using namespace std;

template <class T>
struct std::formatter<std::vector<T>> : std::formatter<const char*> {
  auto format(std::vector<T> vec, format_context& ctx) {
    auto out = ctx.out();
    *out = '{';
    ++out;
    for (size_t i = 0; i < std::ssize(vec); ++i) {
      out = std::format_to(out, "{}", vec[i]);
      if (i + 1 != std::ssize(vec)) {
        *out = ',';
        ++out;
      }
    }
    *out = '}';
    ++out;
    return out;
  }
};
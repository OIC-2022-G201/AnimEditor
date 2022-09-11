#pragma once
#include <filesystem>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
template <class Archive>
void CEREAL_LOAD_MINIMAL_FUNCTION_NAME(const Archive&,
                                       std::filesystem::path& out,
                                       const std::string& in) {
  out = in;
}
template <class Archive>
std::string CEREAL_SAVE_MINIMAL_FUNCTION_NAME(const Archive& ar,
                                              const std::filesystem::path& p) {
  return p.string();
}
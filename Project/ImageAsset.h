#pragma once
#include <filesystem>
#include <string>
#include <cereal/types/string.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
namespace asset {
namespace fs = std::filesystem;
class ImageAsset {
  fs::path path_;

 public:
  ImageAsset() = default;
  explicit ImageAsset(const fs::path& path) { Load(path); }
  bool Load(const fs::path& path) {
    if (!fs::exists(path)) return false;
    if (path.extension() != ".png") return false;
    path_ = fs::relative(path);
    return true;
  }
  fs::path path() const
  { return path_; }
  template <class Archive>
  std::string save_minimal(Archive const&) const {
    return path_.string();
  }

  template <class Archive>
  void load_minimal(Archive const&, std::string const& value) {
    path_ = value;
  }
};
}  // namespace asset

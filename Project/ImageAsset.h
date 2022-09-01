#pragma once
#include <filesystem>
#include <string>
namespace asset {
namespace fs = std::filesystem;
class ImageAsset {
  fs::path path_;

 public:
  explicit ImageAsset(const fs::path& path) { Load(path); }
  bool Load(const fs::path& path) {
    if (fs::exists(path)) return false;
    if (path.extension() != ".png") return false;
    path_ = fs::relative(path);
    return true;
  }
  fs::path path()
  { return path_;
  }
};
}  // namespace asset

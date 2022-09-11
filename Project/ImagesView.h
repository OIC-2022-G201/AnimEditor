#pragma once
#include <Collision/Rectangle.h>
#include <Graphics/DirectX11/DX11Texture.h>
#include <Graphics/Texture.h>

#include <map>
#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

#include "ImageAsset.h"
#include "SpriteAnimationCreateInfo.h"

struct Vector2Int {
  uint32_t x = 0;
  uint32_t y = 0;

  Vector2Int operator*(Vector2Int other) const {
    return {x * other.x, y * other.y};
  }
  Vector2Int operator+(Vector2Int other) const {
    return {x + other.x, y + other.y};
  }

  Vector2Int operator-(Vector2Int other) const {
    return {x - other.x, y - other.y};
  }
  float LengthSqrt() const { return x * x + y * y; }

  bool operator==(const Vector2Int& left) const {
    return this->x == left.x && this->y == left.y;
  }
};
namespace std {
template <>
class hash<Vector2Int> {
 public:
  size_t operator()(const Vector2Int& p) const noexcept {
    return p.x * 1000 + p.y;
  }
};
}  // namespace std

class ImagesView {
  std::optional<png::image<png::rgba_pixel>> data_ = std::nullopt;
  std::unordered_map<Vector2Int, png::image<png::rgba_pixel>> pallet_;

  Mof::CTexture map_;

  Vector2Int max_size_{0, 0};
  Vector2Int bottom_right_index_{0, 0};

 public:
  ImagesView() {}

  Vector2Int MaxCellSize() const { return max_size_; }
  void Save(const std::string& filename) {
    auto image = GetCellMap();
    image->write(filename);
  }

  const std::optional<png::image<png::rgba_pixel>>& GetCellMap() {
    if (!data_) {
      Encode();
      if (data_->get_width() == 0 || data_->get_height() == 0) {
        data_->resize(128, 128);
      }
    }
    return data_;
  }
  void Encode() {
    {
      auto [x, y] = (bottom_right_index_ + Vector2Int(1, 1)) * max_size_;
      data_ = png::image<png::rgba_pixel>(x, y);
    }

    for (const auto& pallet : pallet_) {
      const auto [px, py] = pallet.first * max_size_;

      const auto& image = pallet.second;
      for (int y = 0; y < image.get_height(); ++y) {
        for (int x = 0; x < image.get_width(); ++x) {
          data_.value().set_pixel(px + x, py + y, image.get_pixel(x, y));
        }
      }
    }
  }
  void Remove(Vector2Int index) {
    if (!pallet_.contains(index)) return;

    data_ = std::nullopt;
    pallet_.erase(index);
  }
  void SetImage(const asset::ImageAsset& image, Vector2Int index) {
    png::image<png::rgba_pixel> png;
    png.read(image.path().string());
    SetImage(png, index);
  }

  void SetImage(const png::image<png::rgba_pixel>& png, Vector2Int index) {
    data_ = std::nullopt;
    Vector2Int size{png.get_width(), png.get_height()};
    if (max_size_.x < size.x) {
      max_size_.x = size.x;
    }

    if (max_size_.y < size.y) {
      max_size_.y = size.y;
    }

    if (bottom_right_index_.x < index.x) {
      bottom_right_index_.x = index.x;
    }

    if (bottom_right_index_.y < index.y) {
      bottom_right_index_.y = index.y;
    }
    pallet_.insert_or_assign(index, png);
  }
};

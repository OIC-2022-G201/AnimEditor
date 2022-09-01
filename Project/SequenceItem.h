#pragma once
#include <map>
#include <Graphics/SpriteAnimation.h>
#include <Graphics/Texture.h>

#include "ImageAsset.h"
#include "imgui.h"
#include "SpriteAnimationCreateInfo.h"

class SequenceItem
{
  SpriteAnimationCreateInfo animation_info_{};
  Mof::LPTexture texture_ = nullptr;
 public:
  explicit SequenceItem(Mof::LPTexture texture):texture_(texture) { }
  void Insert(uint32_t frame, asset::ImageAsset image) {
    image_views_.insert_or_assign(frame,image);
  }
  void Erase(uint32_t frame) { image_views_.erase(frame);
  }
  std::map<uint32_t, asset::ImageAsset> image_views_{};
};


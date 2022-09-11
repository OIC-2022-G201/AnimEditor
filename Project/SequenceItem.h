#pragma once
#include <Graphics/SpriteAnimation.h>
#include <Graphics/Texture.h>

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/vector.hpp>
#include <map>

#include "ImageAsset.h"
#include "ImagesView.h"
#include "imgui.h"
class SequenceItem {
  std::string name_ = "Animation";
  std::optional<uint32_t> end_frame_;
  ImagesView images_view_;
  bool is_loop_ = false;

 public:
  std::map<uint32_t, asset::ImageAsset> image_paths_{};

 public:
  SequenceItem(){}
  explicit SequenceItem(std::string name);

  void Insert(uint32_t frame, asset::ImageAsset image);
  void Erase(uint32_t frame);

  void SetEndFrame(const std::optional<uint32_t> end_frame);
  void SetLoop(const bool is_loop);
  bool GetLoop();
  [[nodiscard]] uint32_t GetEndFrame() const;
  void SetName(std::string name);
  std::string GetName() const;
  const std::optional<png::image<png::rgba_pixel>>& GetCellMap();

  SpriteAnimationCreateInfo GetAnimationCreateInfo();

  template <class Archive>
  void save(Archive& archive) const {
    archive(CEREAL_NVP(name_), CEREAL_NVP(image_paths_), CEREAL_NVP(end_frame_),
            CEREAL_NVP(is_loop_));
  }

  template <class Archive>
  void load(Archive& archive) {
    archive(CEREAL_NVP(name_), CEREAL_NVP(image_paths_), CEREAL_NVP(end_frame_),
            CEREAL_NVP(is_loop_));

    for (const auto& image_path : image_paths_) {
      SetImage(image_path.first, image_path.second);
    }
  }

 private:
  void SetImage(uint32_t frame, asset::ImageAsset image) {
    size_t d = std::distance(image_paths_.begin(), image_paths_.find(frame));
    images_view_.SetImage(image, {d, 0});
  }
};

CEREAL_CLASS_VERSION(SequenceItem, 1);

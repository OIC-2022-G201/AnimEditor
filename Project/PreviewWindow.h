#pragma once
#include <Graphics/DirectX11/DX11Texture.h>
#include <Graphics/SpriteMotionController.h>

#include <optional>
#include <png++/image.hpp>
#include <Utilities/GraphicsUtilities.h>

#include "SpriteAnimationCreateInfo.h"
#include "imgui.h"

class PreviewWindow {
 public:
  void Update();
  void Render()
  {
    if (is_create_) {
      image_.Render(100, 100, motion_.GetSrcRect());
      Mof::CGraphicsUtilities::RenderRect(
          100, 100, 100+motion_.GetSrcRect().GetWidth(),
          100+motion_.GetSrcRect().GetHeight(), MOF_COLOR_WHITE);
    }
  };
  void SetAnimation(
      const std::optional<png::image<png::rgba_pixel>>& image,
      const std::vector<SpriteAnimationCreateInfo>& animation_create_info);
  ~PreviewWindow();

  void Play(int number);
  void Stop();

private:
  bool is_play_ = false;
  bool is_create_ = false;
  Mof::CSpriteMotionController motion_;
  Mof::CTexture image_;
  ImVec2 canvas_size;
  ImVec2 canvas_pos;
};

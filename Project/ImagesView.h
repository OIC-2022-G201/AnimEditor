#pragma once
#include <Collision/Rectangle.h>
#include <Graphics/DirectX11/DX11Texture.h>
#include <Graphics/Texture.h>

#include <png++/image.hpp>
#include <png++/rgba_pixel.hpp>

#include "SpriteAnimationCreateInfo.h"

class ImagesView {
  png::image<png::rgba_pixel> data_;
  Mof::CTexture map_;

 public:
  ImagesView()
  {
      
  }



};

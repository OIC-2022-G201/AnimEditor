#pragma once
#include <array>
#include <vector>
#include <Graphics/DirectX11/DX11Texture.h>

#include "imfilebrowser.h"
#include "PayloadParameter.h"
#include "SequenceItem.h"
#include "imgui.h"
#include "imgui_internal.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
static ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
  return ImVec2(a.x + b.x, a.y + b.y);
}
static ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
  return ImVec2(a.x - b.x, a.y - b.y);
}
#endif

class Sequencer {
 public:
  class KeyFrame {
    ImRect rect_;
    int color_;
    bool pointing;

   public:
    void Update() {
      ImGuiIO& io = ImGui::GetIO();
      pointing = rect_.Contains(io.MousePos);
      color_ = pointing ? 0xFFAAAAAA : 0x77A3B2AA;
    }
    void Render(ImDrawList* draw_list) {
      ImVec2 extension_pixel;
      if (pointing) extension_pixel = {5, 5};
      draw_list->AddRect(rect_.Min - extension_pixel,
                         rect_.Max + extension_pixel, color_, 4);
    }
  };

 private:
  bool is_open_popup_ = false;

  ImGui::FileBrowser file_dialog_{ImGuiFileBrowserFlags_MultipleSelection};
 public:
  void KeyFrameDraw() const
  {
      for (int y = 0;y<items_.size();++y)
      {
      for (auto image_view : items_[y].image_views_)
      {
          auto cx = ContentXPosition(image_view.first);
          auto cy = ContentYPosition(y);
          draw_list->AddRectFilled(
              {cx, cy},
              {cx + ItemHeight,
               cy + ItemHeight},
                                   0xff29f8ff, ItemHeight/2);
      }
      } 
  }
  void KeyFrameClickPopup() {
    ImGuiIO& io = ImGui::GetIO();
    is_open_popup_ = false;

    if (io.MouseClicked[1]) ImGui::OpenPopup("my_select_popup");
    if (ImGui::BeginPopup("my_select_popup")) {
      is_open_popup_ = true;
      ImGui::Text("keyFrameAction");
      ImGui::Separator();
      for (int i = 0; i < popup_keyframe_messages_.size(); i++)
        if (ImGui::Selectable(popup_keyframe_messages_[i].data()))
          selected_popup_keyframe_ = i;
      ImGui::EndPopup();
    }
  }
  void KeyFrameEvent() {
    if (selected_popup_keyframe_) {
      switch (selected_popup_keyframe_.value()) {
        case 0:
//          items_[cursor_.cursor_y].Insert(0, );
          if (!file_dialog_.IsOpened())
              file_dialog_.Open();
          break;
        case 1:;

          selected_popup_keyframe_ = std::nullopt;
          break;
      }
    }
  }
  bool Update();

 private:
  Mof::CTexture texture;
  ImDrawList* draw_list = nullptr;
  ImVec2 canvas_size;
  ImVec2 canvas_pos;

  float framePixelWidth = 60.f;
  float framePixelWidthTarget = 60.f;
  float frame_min = 0;
  float frame_max = 120;

  int mod_frame_count = 10;
  int firstFrameUsed = 0;
  int legendWidth = 200;

  void DrawLine(int i, int region_height) const;

 private:
  std::array<std::string_view, 2> popup_keyframe_messages_ = {"Add", "Delete"};
  std::optional<int> selected_popup_keyframe_ = std::nullopt;
  bool hasScrollBar;
  ImVec2 contentMin;
  ImVec2 contentMax;
  ImVec2 headerSize;
  ImVec2 scrollBarSize;
  int ItemHeight = 20;

  std::vector<SequenceItem> items_ = {SequenceItem(&texture)};
  void DrawLineContent(int i) const;

  ImVec2 childFramePos;

  ImVec2 childFrameSize;
  ImRect contentRect;
  float contentHeight;
  float controlHeight = 20;
  void InitialContent();

  struct KeyFrameCursor {
    int cursor_x;
    int cursor_y;
    bool in_canvas;
  };
  KeyFrameCursor cursor_ = {};
  void CursorSetting() {
    ImGuiIO& io = ImGui::GetIO();
    
    auto px = PositionXToIndex(io.MousePos.x);
    auto py = PositionYToIndex(io.MousePos.y);

    if (!(is_open_popup_ || selected_popup_keyframe_)) {
      if (px < 0 || py < 0) {
        cursor_.in_canvas = false;
        return;
      } else {
      }
      cursor_ = {px, py, true};
    }
    draw_list->AddRectFilled({ContentXPosition(cursor_.cursor_x),
                              ContentYPosition(cursor_.cursor_y)},
        {ContentXPosition(cursor_.cursor_x) + ItemHeight,
                              ContentYPosition(cursor_.cursor_y) + ItemHeight},
        0xff00ff00);
  }
  void ContentRender() const;

 private:
  float header_height_ = 20;
  int frameStep = 1;
  float ContentXPosition(int i) const {
    return (int)canvas_pos.x + int(i * framePixelWidth) + legendWidth -
           int(firstFrameUsed * framePixelWidth);
  }
  float ContentYPosition(int i) const {
    return contentMin.y + ItemHeight * i + 1;
  }
  int PositionXToIndex(int x) const {
    return firstFrameUsed + -(canvas_pos.x + legendWidth - x) / framePixelWidth;
  }

  int PositionYToIndex(int y) const { return (-contentMin.y + y) / ItemHeight; }
  void HeaderRender() const;
};

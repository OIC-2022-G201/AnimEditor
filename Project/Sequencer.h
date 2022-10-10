#pragma once
#include <Graphics/DirectX11/DX11Texture.h>

#include <array>
#include <vector>

#include "SequenceItem.h"
#include "imfilebrowser.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

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
  Sequencer();

public:

 private:
  bool is_open_popup_ = false;
  uint32_t current_frame_ = 0;
public:
  uint32_t GetCurrentFrame() const;

private:
  ImGui::FileBrowser file_dialog_{};
 std::string project_name_;
 public:
  void KeyFrameDraw() const;
  void KeyFrameClickPopup();
  void KeyFrameEvent();
  void TimelineEvent();
  bool Update();
  void SetPlayButtonEvent(const std::function<void(bool,std::optional<int>)>& button_event);

  std::optional<png::image<png::rgba_pixel>> GetCellMap();

  std::vector<SpriteAnimationCreateInfo> GetAnimationCreateInfo();

private:
  bool is_play_ = false;
  std::vector<std::function<void(bool, std::optional<int>)>> player_events_;
  void PlayerRender();
  void Play();
  void KeyFrameDelete();
  void InsertEndKeyFrame();

  void RemoveEndKeyFrame();

private:
  Mof::CTexture texture;
  ImDrawList* draw_list = nullptr;
  ImVec2 canvas_size;
  ImVec2 canvas_pos;

  float framePixelWidth = 30;
  float framePixelWidthTarget = 60.f;
  float frame_min = 0;
  float frame_max = 300;

  int mod_frame_count = 10;
  int firstFrameUsed = 0;
  int legendWidth = 200;

  void DrawLine(int i, int region_height) const;

 private:
  std::array<std::string_view, 4> popup_keyframe_messages_ = {
      "Add", "Delete", "End Key Frame", "End Key Remove"};
  std::array<std::pair<int, std::string_view>, 5> popup_timeline_messages_ = {
      std::make_pair(0, "Rename"), std::make_pair(1, "Insert"),
      std::make_pair(4, "Remove"),
      std::make_pair(2, "Up"), std::make_pair(3, "Down")};
  std::optional<int> selected_popup_keyframe_ = std::nullopt;
  std::optional<int> selected_popup_timeline_ = std::nullopt;
  bool hasScrollBar;
  ImVec2 contentMin;
  ImVec2 contentMax;
  ImVec2 headerSize;
  ImVec2 scrollBarSize;
  ImVec2 scroll_position_;
  int ItemSize = 20;

  std::vector<SequenceItem> items_ = {SequenceItem( "Animation1")};

  std::optional<int> current_hover_item_ = std::nullopt;
  std::optional<int> current_select_item_ = std::nullopt;
  void DrawLineContent(int i) const;

  ImVec2 childFramePos;

  ImVec2 childFrameSize;
  ImRect contentRect;
  float contentHeight;
  float controlHeight = 20;

  std::string rename_buf_;

  void InitialContent();

  struct KeyFrameCursor {
    int x;
    int y;
  };
  std::optional<KeyFrameCursor> cursor_ = std::nullopt;
  void CursorSetting();
  void ContentRender() const;

 private:
  float header_height_ = 20;
  int frameStep = 1;
  float ContentXPosition(int i) const;

  float ContentYPosition(int i) const;

  int PositionXToIndex(int x) const;

  int PositionYToIndex(int y) const;
  void HeaderRender() const;
public:
  template <class Archive>
 void serialize(Archive& archive, std::uint32_t const version) {
    archive(CEREAL_NVP(items_));
  }
};
CEREAL_CLASS_VERSION(Sequencer, 1);

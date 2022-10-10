#include "Sequencer.h"

#include "ColorScheme.h"
#include "imGui/cpp/imgui_stdlib.h"

Sequencer::Sequencer() {
  file_dialog_ = ImGui::FileBrowser{};
  file_dialog_.SetTypeFilters({".png"});
}

uint32_t Sequencer::GetCurrentFrame() const { return current_frame_; }

void Sequencer::KeyFrameDraw() const {
  for (int y = 0; y < items_.size(); ++y) {
    for (auto image_view : items_[y].image_paths_) {
      auto cx = ContentXPosition(image_view.first);
      auto cy = ContentYPosition(y);
      draw_list->AddRectFilled({cx, cy}, {cx + ItemSize, cy + ItemSize},
                               0xff29f8ff, ItemSize / 2);
    }
  }
}

void Sequencer::SetPlayButtonEvent(
    const std::function<void(bool, std::optional<int>)>& button_event) {
  player_events_.emplace_back(button_event);
}

std::optional<png::image<png::rgba_pixel>> Sequencer::GetCellMap() {
  ImagesView images_view;

  for (size_t i = 0; i < items_.size(); ++i) {
    images_view.SetImage(items_[i].GetCellMap().value(), Vector2Int{0, i});
  }
  return images_view.GetCellMap();
}

std::vector<SpriteAnimationCreateInfo> Sequencer::GetAnimationCreateInfo() {
  std::vector<SpriteAnimationCreateInfo> animation_create_infos;
  for (size_t i = 0; i < items_.size(); ++i) {
    auto info = items_[i].GetAnimationCreateInfo();
    info.offset_y += i * info.height;
    animation_create_infos.emplace_back(info);
  }
  return animation_create_infos;
}

void Sequencer::InsertEndKeyFrame() {
  if (!cursor_) return;
  if (cursor_.value().y >= items_.size()) return;
  if (cursor_.value().y < 0) return;
  items_.at(cursor_.value().y).SetEndFrame(cursor_.value().x);
}

void Sequencer::RemoveEndKeyFrame() {
  if (!cursor_) return;

  if (cursor_.value().y >= items_.size()) return;
  if (cursor_.value().y < 0) return;
  items_.at(cursor_.value().y).SetEndFrame(std::nullopt);
}

float Sequencer::ContentXPosition(int i) const {
  return static_cast<int>(canvas_pos.x + i * framePixelWidth + legendWidth -
                          firstFrameUsed * framePixelWidth);
}

float Sequencer::ContentYPosition(int i) const {
  return contentMin.y + ItemSize * i + 1;
}

int Sequencer::PositionXToIndex(int x) const {
  return firstFrameUsed + -(canvas_pos.x + legendWidth - x) / framePixelWidth;
}

int Sequencer::PositionYToIndex(int y) const {
  return (-contentMin.y + y) / ItemSize;
}

void Sequencer::KeyFrameClickPopup() {
  ImGuiIO& io = ImGui::GetIO();
  is_open_popup_ = false;

  if (cursor_ && io.MouseClicked[1])
    ImGui::OpenPopup("my_keyframe_select_popup");
  if (current_hover_item_ && io.MouseClicked[1])
    ImGui::OpenPopup("my_item_select_popup");
  if (selected_popup_timeline_ && selected_popup_timeline_.value() == 0)
    ImGui::OpenPopup("Timeline_Rename");
  if (ImGui::BeginPopup("my_keyframe_select_popup")) {
    is_open_popup_ = true;
    ImGui::Text("keyFrameAction");
    ImGui::Separator();
    for (int i = 0; i < popup_keyframe_messages_.size(); i++)
      if (ImGui::Selectable(popup_keyframe_messages_[i].data()))
        selected_popup_keyframe_ = i;
    ImGui::EndPopup();
    selected_popup_timeline_ = std::nullopt;
  }

  if (current_select_item_) {
    auto&& item = items_[current_select_item_.value()];
    if (ImGui::BeginPopup("my_item_select_popup")) {
      is_open_popup_ = true;
      ImGui::Text(item.GetName().data());
      ImGui::Separator();
      auto flg = item.GetLoop();
      ImGui::Checkbox("loop", &flg);
      item.SetLoop(flg);
      for (auto& popup_timeline_message : popup_timeline_messages_)
        if (ImGui::Selectable(popup_timeline_message.second.data()))
          selected_popup_timeline_ = popup_timeline_message.first;
      ImGui::EndPopup();
    }
  }

  if (ImGui::BeginPopup("Timeline_Rename")) {
    is_open_popup_ = true;
    ImGui::Text("Timeline_Rename");
    ImGui::Separator();
    if (current_hover_item_ && items_.size() >= current_hover_item_.value()) {
      ImGui::BeginGroup();
      ImGui::InputText("animation name", &rename_buf_);

      // ImGui::SameLine();
      // if (ImGui::Button("Cancel")) {
      //  ImGui::CloseCurrentPopup();
      //}

      ImGui::SameLine();
      selected_popup_timeline_ = std::nullopt;
      if (ImGui::Button("OK")) {
        selected_popup_timeline_ = std::nullopt;
        items_[current_hover_item_.value()].SetName(rename_buf_);
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }
}

void Sequencer::KeyFrameEvent() {
  if (!selected_popup_keyframe_) return;
  switch (selected_popup_keyframe_.value()) {
    case 0:
      if (!file_dialog_.IsOpened()) file_dialog_.Open();
      break;
    case 1:
      KeyFrameDelete();
      selected_popup_keyframe_ = std::nullopt;
      break;
    case 2:
      InsertEndKeyFrame();
      selected_popup_keyframe_ = std::nullopt;
      break;
    case 3:
      RemoveEndKeyFrame();
      selected_popup_keyframe_ = std::nullopt;
      break;
    default:;
  }
}

void Sequencer::TimelineEvent() {
  if (!selected_popup_timeline_) return;
  switch (selected_popup_timeline_.value()) {
    case 0:
      rename_buf_ = items_[current_select_item_.value()].GetName();
      break;
    case 1:
      items_.emplace_back(SequenceItem("NewAnimation"));
      selected_popup_timeline_ = std::nullopt;
      break;
    case 2:
      if (current_select_item_ && current_select_item_.value() > 0) {
        std::swap(items_[current_select_item_.value()],
                  items_[current_select_item_.value() - 1]);
      }
      selected_popup_timeline_ = std::nullopt;
      break;
    case 3:

      if (current_select_item_ && current_select_item_.value() < items_.size() - 1) {
        std::swap(items_[current_select_item_.value()],
                  items_[current_select_item_.value() + 1]);
      }
      selected_popup_timeline_ = std::nullopt;
      break;
    case 4:
      items_.erase(items_.begin() + current_select_item_.value());
      if (items_.empty()) {
        items_.emplace_back(SequenceItem("NewAnimation"));
      }
      current_select_item_ = std::nullopt;
      selected_popup_timeline_ = std::nullopt;
      break;
    default:;
  }
}
bool Sequencer::Update() {
  auto io = ImGui::GetIO();
  ImGuiWindowFlags window_flags = 0;
  // window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
  window_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

  ImGui::SetNextWindowSize({io.DisplaySize.x, 250});
  ImGui::SetNextWindowPos({0, io.DisplaySize.y - 250});
  ImGui::Begin("Timeline", nullptr, window_flags);

  ImGui::BeginGroup();
  canvas_pos =
      ImGui::GetCursorScreenPos();  // ImDrawList API uses screen coordinates!
  canvas_size =
      ImGui::GetContentRegionAvail();  // Resize canvas to what's available
  draw_list = ImGui::GetWindowDrawList();
  canvas_pos.y += 30;
  PlayerRender();
  Play();
  InitialContent();
  {
    auto start = canvas_pos;
    start.y += scroll_position_.y;
    auto end = start + canvas_size + scroll_position_;

    draw_list->AddRectFilled(start, end, kBackgroundColor, 0);
  }
  HeaderRender();
  ContentRender();
  CursorSetting();
  KeyFrameClickPopup();
  KeyFrameEvent();
  TimelineEvent();
  file_dialog_.Display();

  if (file_dialog_.HasSelected()) {
    auto path = file_dialog_.GetSelected();
    file_dialog_.ClearSelected();
    if (cursor_ && selected_popup_keyframe_ &&
        selected_popup_keyframe_.value() == 0) {
      auto&& item = items_.at(cursor_.value().y);
      auto png_path = asset::ImageAsset{path};
      item.Insert(cursor_.value().x, png_path);
    }
    selected_popup_keyframe_ = std::nullopt;
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::EndGroup();

  ImGui::End();
  return true;
}

void Sequencer::PlayerRender() {
  if (!current_select_item_) ImGui::BeginDisabled();
  if (ImGui::Button(is_play_ ? "Stop" : "Play")) {
    is_play_ = !is_play_;
    auto image = items_[current_select_item_.value()].GetCellMap();
    for (const auto& player_event : player_events_) {
      player_event(is_play_, current_select_item_);
    }
  }
  if (!current_select_item_) ImGui::EndDisabled();
}

void Sequencer::Play() {
  if (!is_play_) return;
  current_frame_++;
  if (auto&& item = items_[current_select_item_.value()];
      current_frame_ > item.GetEndFrame()) {
    current_frame_ = 0;
    if (!item.GetLoop()) {
      is_play_ = false;
      for (const auto& player_event : player_events_) {
        player_event(is_play_, current_select_item_);
      }
    }
  }
}

void Sequencer::KeyFrameDelete() {
  if (!cursor_) return;

  if (cursor_.value().y >= items_.size()) return;
  if (cursor_.value().y < 0) return;
  items_.at(cursor_.value().y).Erase(cursor_.value().x);
}

void Sequencer::DrawLine(int i, int region_height) const {
  auto half_mod_frame_count = mod_frame_count / 2;
  bool baseIndex =
      ((i % mod_frame_count) == 0) || (i == frame_max || i == frame_min);
  bool halfIndex = (i % half_mod_frame_count) == 0;
  int px = ContentXPosition(i);
  int tiretStart = baseIndex ? 4 : (halfIndex ? 10 : 14);
  int tiretEnd = baseIndex ? region_height : header_height_;
  int sx = scroll_position_.x;
  if (px <= (canvas_size.x + canvas_pos.x + sx) &&
      px >= (canvas_pos.x + legendWidth)) {
    draw_list->AddLine(ImVec2((float)px, canvas_pos.y + scroll_position_.y +
                                             (float)tiretStart),
                       ImVec2((float)px, canvas_pos.y + scroll_position_.y +
                                             (float)tiretEnd - 1),
                       0xFF606060, 1);

    draw_list->AddLine(ImVec2((float)px, canvas_pos.y + scroll_position_.y +
                                             (float)header_height_),
                       ImVec2((float)px, canvas_pos.y + scroll_position_.y +
                                             (float)region_height - 1),
                       0x30606060, 1);
  }

  if (baseIndex && px >= (canvas_pos.x + legendWidth)) {
    char tmps[512];
    ImFormatString(tmps, IM_ARRAYSIZE(tmps), "%d", i);
    draw_list->AddText(
        ImVec2((float)px + 3.f, canvas_pos.y + scroll_position_.y), 0xFFBBBBBB,
        tmps);
  }
}

void Sequencer::DrawLineContent(int i) const {
  int px = (int)canvas_pos.x + int(i * framePixelWidth) + legendWidth -
           int(firstFrameUsed * framePixelWidth);
  int tiretStart = int(contentMin.y);
  int tiretEnd = int(contentMin.y + items_.size() * ItemSize);

  if (px <= (canvas_size.x + canvas_pos.x + scroll_position_.x) &&
      px >= (canvas_pos.x + legendWidth + scroll_position_.x)) {
    draw_list->AddLine(ImVec2(float(px), float(tiretStart)),
                       ImVec2(float(px), float(tiretEnd)), 0x30606060, 1);
  }
}

void Sequencer::InitialContent() {
  hasScrollBar = true;
  headerSize = {canvas_size.x, (float)ItemSize};
  scroll_position_ = {ImGui::GetScrollX(), ImGui::GetScrollY()};
  scrollBarSize = {canvas_size.x, 14.f};
  ImGui::InvisibleButton("topBar", headerSize);
  childFramePos = ImGui::GetCursorScreenPos();
  childFrameSize = {canvas_size.x, canvas_size.y - 8.f - headerSize.y -
                                       (hasScrollBar ? scrollBarSize.y : 0)};
  ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
  auto ypos = ContentYPosition(items_.size() + 1) - childFramePos.y;
  ImVec2 cs = ImVec2{ContentXPosition(frame_max * 2), ypos};
  ImGui::BeginChild("scrolling", cs, false,
                    ImGuiWindowFlags_HorizontalScrollbar |
                        ImGuiWindowFlags_AlwaysVerticalScrollbar);
  contentMin = ImGui::GetItemRectMin();
  contentMax = ImGui::GetItemRectMax();
  contentRect = {contentMin, contentMax};
  contentHeight = contentMax.y - contentMin.y;
}

void Sequencer::CursorSetting() {
  ImGuiIO& io = ImGui::GetIO();

  auto px = PositionXToIndex(io.MousePos.x);
  auto py = PositionYToIndex(io.MousePos.y);

  if (!(is_open_popup_ || selected_popup_keyframe_)) {
    if (px < 0 || py < 0 || py >= items_.size()) {
      cursor_ = std::nullopt;
    } else {
      cursor_ = {px, py};
    }
    if (px < 0 && py >= 0 && py < items_.size()) {
      current_hover_item_ = py;
      if (io.MouseClicked[0]) {
        current_select_item_ = py;
      }
    } else {
      current_hover_item_ = std::nullopt;
      if (io.MouseClicked[0]) {
        current_hover_item_ = std::nullopt;
      }
    }
  }
  if (cursor_)
    draw_list->AddRectFilled({ContentXPosition(cursor_.value().x),
                              ContentYPosition(cursor_.value().y)},
                             {ContentXPosition(cursor_.value().x) + ItemSize,
                              ContentYPosition(cursor_.value().y) + ItemSize},
                             0xff00ff00);
}

void Sequencer::ContentRender() const {
  draw_list->PushClipRect(childFramePos + scroll_position_,
                          childFramePos + childFrameSize + scroll_position_);
  int sequenceCount = items_.size();
  for (int i = 0; i < sequenceCount; i++) {
    int type;
    ImVec2 tpos(contentMin.x + 3, contentMin.y + i * ItemSize + 2);
    ImU32 col;
    if ((current_hover_item_ && current_hover_item_.value() == i))
      col = 0xFFDC9D71;
    else
      col = current_select_item_ && current_select_item_.value() == i
                ? 0xFFBC7D51
                : kBackgroundColor;
    draw_list->AddRectFilled(tpos,
                             tpos + ImVec2{static_cast<float>(legendWidth - 40),
                                           static_cast<float>(ItemSize - 1)},
                             col);
    draw_list->AddText(tpos, 0xFFFFFFFF, items_[i].GetName().data());
  }
  for (int i = 0; i < sequenceCount; i++) {
    unsigned int col = (i & 1) ? 0xFF3A3636 : 0xFF413D3D;

    ImVec2 pos = ImVec2(contentMin.x + legendWidth, ContentYPosition(i));
    ImVec2 sz = ImVec2(canvas_size.x + canvas_pos.x + scroll_position_.x,
                       pos.y + scroll_position_.y + ItemSize - 1);

    draw_list->AddRectFilled(pos, sz, col, 0);
    draw_list->AddLine(
        {ContentXPosition(items_[i].GetEndFrame()) + ItemSize + 2, pos.y},
        {ContentXPosition(items_[i].GetEndFrame()) + ItemSize + 2, sz.y},
        0xff0000ff, 2);
  }

  KeyFrameDraw();

  for (int i = frame_min; i <= frame_max; i += frameStep) {
    DrawLineContent(i);
  }

  draw_list->PopClipRect();
}

void Sequencer::HeaderRender() const {
  auto start = canvas_pos;
  start.y += scroll_position_.y;
  draw_list->AddRectFilled(
      start,
      ImVec2(canvas_size.x + canvas_pos.x + scroll_position_.x,
             canvas_pos.y + scroll_position_.y + header_height_),
      0xFF3D3837, 0);

  for (int i = frame_min; i <= frame_max; i += frameStep) {
    DrawLine(i, header_height_);
  }

  DrawLine(frame_min, header_height_);
  DrawLine(frame_max, header_height_);
}

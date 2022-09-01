#include "Sequencer.h"

bool Sequencer::Update()
{
    ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_MenuBar);

    ImGui::BeginGroup();
    canvas_pos =
        ImGui::GetCursorScreenPos();  // ImDrawList API uses screen coordinates!
    canvas_size =
        ImGui::GetContentRegionAvail();  // Resize canvas to what's available
    draw_list = ImGui::GetWindowDrawList();
    InitialContent();
    draw_list->AddRectFilled(canvas_pos, canvas_pos + canvas_size, 0xFF242424,
                             0);
    HeaderRender();
    ContentRender();
    CursorSetting();
    KeyFrameClickPopup();
    KeyFrameEvent();
    KeyFrameDraw();
    file_dialog_.Display();

    if (file_dialog_.HasSelected()) {
      auto paths = file_dialog_.GetMultiSelected();
      file_dialog_.ClearSelected();
      selected_popup_keyframe_ = std::nullopt;
      for (auto path : paths)
      {
        
        auto&& item = items_.at(cursor_.cursor_y);
        item.Insert(cursor_.cursor_x, asset::ImageAsset{path});

      }
    }
    
    ImGui::EndChildFrame();
    ImGui::PopStyleColor();

    ImGui::EndGroup();

    ImGui::End();
    return true;
}

void Sequencer::DrawLine(int i, int region_height) const
{
    auto half_mod_frame_count = mod_frame_count / 2;
    bool baseIndex =
        ((i % mod_frame_count) == 0) || (i == frame_max || i == frame_min);
    bool halfIndex = (i % half_mod_frame_count) == 0;
    int px = ContentXPosition(i);
    int tiretStart = baseIndex ? 4 : (halfIndex ? 10 : 14);
    int tiretEnd = baseIndex ? region_height : header_height_;

    if (px <= (canvas_size.x + canvas_pos.x) &&
        px >= (canvas_pos.x + legendWidth)) {
        draw_list->AddLine(ImVec2((float)px, canvas_pos.y + (float)tiretStart),
                           ImVec2((float)px, canvas_pos.y + (float)tiretEnd - 1),
                           0xFF606060, 1);

        draw_list->AddLine(
            ImVec2((float)px, canvas_pos.y + (float)header_height_),
            ImVec2((float)px, canvas_pos.y + (float)region_height - 1),
            0x30606060, 1);
    }

    if (baseIndex && px >= (canvas_pos.x + legendWidth)) {
        char tmps[512];
        ImFormatString(tmps, IM_ARRAYSIZE(tmps), "%d", i);
        draw_list->AddText(ImVec2((float)px + 3.f, canvas_pos.y), 0xFFBBBBBB,
                           tmps);
    }
}

void Sequencer::DrawLineContent(int i) const
{
    int px = (int)canvas_pos.x + int(i * framePixelWidth) + legendWidth -
        int(firstFrameUsed * framePixelWidth);
    int tiretStart = int(contentMin.y);
    int tiretEnd = int(contentMax.y);

    if (px <= (canvas_size.x + canvas_pos.x) &&
        px >= (canvas_pos.x + legendWidth)) {
        // draw_list->AddLine(ImVec2((float)px, canvas_pos.y + (float)tiretStart),
        // ImVec2((float)px, canvas_pos.y + (float)tiretEnd - 1), 0xFF606060, 1);

        draw_list->AddLine(ImVec2(float(px), float(tiretStart)),
                           ImVec2(float(px), float(tiretEnd)), 0x30606060, 1);
    }
}

void Sequencer::InitialContent()
{
    hasScrollBar = true;
    headerSize = {canvas_size.x, (float)ItemHeight};
    scrollBarSize = {canvas_size.x, 14.f};
    ImGui::InvisibleButton("topBar", headerSize);
    draw_list->AddRectFilled(canvas_pos, canvas_pos + headerSize, 0xFFFF0000,
                             0);
    childFramePos = ImGui::GetCursorScreenPos();
    childFrameSize = {canvas_size.x, canvas_size.y - 8.f - headerSize.y -
        (hasScrollBar ? scrollBarSize.y : 0)};
    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
    ImGui::BeginChildFrame(889, childFrameSize);
    contentMin = ImGui::GetItemRectMin();
    contentMax = ImGui::GetItemRectMax();
    contentRect = {contentMin, contentMax};
    contentHeight = contentMax.y - contentMin.y;
}

void Sequencer::ContentRender() const
{
    draw_list->PushClipRect(childFramePos, childFramePos + childFrameSize);
  int sequenceCount = items_.size();
    for (int i = 0; i < sequenceCount; i++) {
        int type;
        ImVec2 tpos(contentMin.x + 3,
                    contentMin.y + i * ItemHeight + 2);
        draw_list->AddText(tpos, 0xFFFFFFFF, "abcdefg");
      
    }
    for (int i = 0; i < sequenceCount; i++) {
        unsigned int col = (i & 1) ? 0xFF3A3636 : 0xFF413D3D;
      
        ;
        ImVec2 pos = ImVec2(contentMin.x + legendWidth, ContentYPosition(i));
        ImVec2 sz = ImVec2(canvas_size.x + canvas_pos.x,
                           pos.y + ItemHeight - 1 );

        draw_list->AddRectFilled(pos, sz, col, 0);
    }

    draw_list->PushClipRect(childFramePos + ImVec2(float(legendWidth), 0.f),
                            childFramePos + childFrameSize);

    // vertical frame lines in content area
    for (int i = frame_min; i <= frame_max; i += frameStep) {
        DrawLineContent(i);
    }

    DrawLineContent(frame_min);
    DrawLineContent(frame_max);

    draw_list->PopClipRect();
}

void Sequencer::HeaderRender() const
{
    draw_list->AddRectFilled(
        canvas_pos,
        ImVec2(canvas_size.x + canvas_pos.x, canvas_pos.y + header_height_),
        0xFF3D3837, 0);

    for (int i = frame_min; i <= frame_max; i += frameStep) {
        DrawLine(i, header_height_);
    }

    DrawLine(frame_min, header_height_);
    DrawLine(frame_max, header_height_);
}

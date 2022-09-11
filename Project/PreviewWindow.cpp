#include "PreviewWindow.h"

#include "PNGConvert.h"
void PreviewWindow::Update()
{
    auto io = ImGui::GetIO();

  ImVec2 offset = {0, 0};
  if (ImGui::BeginMainMenuBar()) {
    offset = ImGui::GetWindowSize();
    ImGui::EndMainMenuBar();
  }
    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::SetNextWindowSize(
        {io.DisplaySize.x, io.DisplaySize.y - 250 - offset.y});
    ImGui::SetNextWindowPos({0, offset.y});
    ImGui::Begin("Preview", nullptr, window_flags);
    
    canvas_pos =
        ImGui::GetCursorScreenPos();  // ImDrawList API uses screen coordinates!
    canvas_size =
        ImGui::GetContentRegionAvail();  // Resize canvas to what's available
    
    ImGui::End();

    if (is_play_)
    {
      motion_.AddTimer(1.0f/60.0f);
    }
}

void PreviewWindow::SetAnimation(const std::optional<png::image<png::rgba_pixel>>& image,
    const std::vector<SpriteAnimationCreateInfo>& animation_create_info)
{
    png::image<png::rgba_pixel> image_copy;
    image_copy = image.value();
    image_copy.get_pixbuf();
    std::vector<ui8> output;
    WritePngToMemory(image_copy, &output);
    if (!image_.Load("preview", output.data(), output.size())) {
    }
    std::vector<Mof::SpriteAnimationCreate> create(
        animation_create_info.size());
    for (int i = 0; i < animation_create_info.size(); ++i) {
        create[i].Name = animation_create_info[i].name.c_str();
        create[i].OffsetX = animation_create_info[i].offset_x;
        create[i].OffsetY = animation_create_info[i].offset_y;
        create[i].Width = animation_create_info[i].width;
        create[i].Height = animation_create_info[i].height;
        create[i].bLoop = animation_create_info[i].loop;
        for (int f = 0; f < min(30, animation_create_info[i].pattern.size());
             ++f) {
            const auto& p = animation_create_info[i].pattern[f];
            create[i].Pattern[f].No = p.no;
            create[i].Pattern[f].Step = p.step;
            create[i].Pattern[f].Wait = p.wait;
        }
    }
    motion_.Create(create.data(), create.size());
    is_create_ = true;
}

PreviewWindow::~PreviewWindow()
{
    motion_.Release();
    image_.Release();
}

void PreviewWindow::Play(int number)
{
    motion_.ChangeMotion(number);
    is_play_ = true;
}

void PreviewWindow::Stop()
{ is_play_ = false; }

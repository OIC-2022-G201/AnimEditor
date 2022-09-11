#include "TextBoxDialog.h"

#include "imGui/cpp/imgui_stdlib.h"

void TextBoxDialog::Display()
{
    auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    is_ok_ = false;
    if (!is_open_) return;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    if (ImGui::Begin("ProjectSave", &is_open_, flags)) {
      ImGui::Text(head_message_.c_str());
        ImGui::BeginGroup();
        ImGui::InputText(label_.c_str(), &value_);

        if (ImGui::Button("OK")) {
            is_ok_ = true;
          is_open_ = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Close")) {
          is_open_ = false;
        }
        ImGui::EndGroup();
    }

    ImGui::End();
}

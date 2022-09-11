/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

// INCLUDE
#include "GameApp.h"
#include <format>
#include "PreviewWindow.h"
#include "Sequencer.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imfilebrowser.h"
#include "imgui.h"
#include "imgui_neo_sequencer.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <png.h>
#include <windows.h>
#include <wrl/client.h>

#include <png++/png.hpp>
#include "SpriteAnimationCreateInfo.h"
#include "FormatExtends.h"

bool my_tool_active = true;
LPTexture g_texture;
/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
  CUtilities::SetCurrentDirectoryA("Resource");
  auto handle = Mof::CUtilities::GetFramework()->GetWindow()->GetWindowHandle();
  SetWindowText(handle, project_name_.data());
  
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  if (false)
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  if (false)
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  ImGui::StyleColorsDark();
  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(g_pMainWindow->GetWindowHandle());
  ImGui_ImplDX11_Init(g_pGraphics->GetDevice(),
                      g_pGraphics->GetDeviceContext());
  sequencer_ = new Sequencer();
  animation_previewer_ = new PreviewWindow();

  sequencer_->SetPlayButtonEvent([&](bool is_play, std::optional<int> item) {
    if (is_play) {
      if (!item) return;
      animation_previewer_->SetAnimation(sequencer_->GetCellMap(),
                                         sequencer_->GetAnimationCreateInfo());
      animation_previewer_->Play(item.value());
    } else {
      animation_previewer_->Stop();
    }
  });
  g_texture = new CTexture;
  save_dialog_.SetTitleText("ProjectSave");
  save_dialog_.SetHeadLabel("Project Save");
  save_dialog_.SetInputLabel(".json");

  export_dialog_.SetTitleText("ProjectExport");
  export_dialog_.SetHeadLabel("Project Export");
  export_dialog_.SetInputLabel(".ae");
  return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
using namespace std;

MofBool CGameApp::Update(void) {
  //キーの更新
  g_pInput->RefreshKey();

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  sequencer_->Update();
  animation_previewer_->Update();
  if (ImGui::BeginMainMenuBar()) {
    ImGui::GetWindowSize();
    if (ImGui::BeginMenu("File")) {
      ShowMainMenuFile();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  file_dialog_.Display();
  if (file_dialog_.HasSelected()) {
    std::stringstream stream;
    auto selected = file_dialog_.GetSelected();
    std::ifstream inputFile(selected);
    project_name_ = selected.stem().string();
    auto handle =
        Mof::CUtilities::GetFramework()->GetWindow()->GetWindowHandle();
    SetWindowText(handle, project_name_.data());
    cereal::JSONInputArchive jsonInputArchive(inputFile);
    jsonInputArchive(cereal::make_nvp("AnimationKeyFrame", *sequencer_));
  }
  save_dialog_.Display();
  if (save_dialog_.IsOk())
  {
    project_name_ = save_dialog_.Value();
    auto handle =
        Mof::CUtilities::GetFramework()->GetWindow()->GetWindowHandle();
    SetWindowText(handle, project_name_.data());
    Save();
  }

  export_dialog_.Display();
  if (export_dialog_.IsOk()) {
    std::string filename = export_dialog_.Value().data();
    std::ofstream outputFile( filename+ ".ae", std::ios::out);
    auto animation_info = sequencer_->GetAnimationCreateInfo();
    std::string t = std::format("{}", animation_info);
    outputFile << t;
    outputFile.close();
    sequencer_->GetCellMap().value().write(filename + "_cellmap.png");
  }
    return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
  //描画開始
  g_pGraphics->RenderStart();
  //画面のクリア
  g_pGraphics->ClearTarget(0.047f, 0.047f, 0.047f, 0.0f, 1.0f, 0);
  animation_previewer_->Render();
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  // g_texture->Render(0, 0);
  //描画の終了
  g_pGraphics->RenderEnd();
  return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  g_texture->Release();
  delete sequencer_;
  delete animation_previewer_;
  return TRUE;
}

void CGameApp::ShowMainMenuFile() {
  if (ImGui::MenuItem("Open")) {
    file_dialog_.SetTypeFilters({".json"});
    file_dialog_.Open();
  }

  if (ImGui::MenuItem("Save")) {
    if (project_name_ == "new_project"){save_dialog_.Open(project_name_);}else
    {
      Save();
    }
  }

  if (ImGui::MenuItem("New Save")) {
    save_dialog_.Open(project_name_);
  }

    
  if (ImGui::MenuItem("Export")) {
    export_dialog_.Open(project_name_);
  }
}

void CGameApp::Save()
{
    std::stringstream stream;
    {
        cereal::JSONOutputArchive jsonOutArchive(stream);
        jsonOutArchive(cereal::make_nvp("AnimationKeyFrame", *sequencer_));
    }
    std::ofstream outputFile(project_name_ + ".json", std::ios::out);
    outputFile << stream.str();
    outputFile.close();
    stream.clear();
}

/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

// INCLUDE
#include "GameApp.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "Sequencer.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"

#include <wincodec.h>

#include "imfilebrowser.h"
#include "imgui_neo_sequencer.h"
#include "PNGConvert.h"
bool my_tool_active = true;
LPTexture g_texture;

#include <wrl/client.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <windows.h>

#include <png.h>
#include <png++/png.hpp>


/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
  CUtilities::SetCurrentDirectoryA("Resource");
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
  g_texture = new CTexture;

  png::image<png::rgba_pixel> image(128, 128);
  for (png::uint_32 y = 0; y < image.get_height(); ++y) {
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      image[y][x] = png::rgba_pixel(x*2,y*2,x+y,255);
      // non-checking equivalent of image.set_pixel(x, y, ...);
    }
  }
  auto n = image.get_pixbuf();
  auto p = n.get_row(0).data();
  std::vector<ui8> output;
      WritePngToMemory(image , &output);
  if (!g_texture->Load("aaa", output.data(), output.size())) {
    int n = 3;
  }
  
  
  return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
using namespace std;

uint32_t currentFrame = 0;
uint32_t startFrame = 0;
uint32_t endFrame = 64;
bool m_pTransformOpen = true;
std::vector<uint32_t> keys = {0, 10, 24};
const char* names[] = {"Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"};
int selected_fish = 0;
MofBool CGameApp::Update(void) {
  //キーの更新
  g_pInput->RefreshKey();

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  
  sequencer_->Update();
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
  g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  //g_texture->Render(0, 0);
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
  return TRUE;
}
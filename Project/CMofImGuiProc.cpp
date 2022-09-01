#include "CMofImGuiProc.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);
;
MofProcResult CMofImGuiProc::WindowProc(MofWindowHandle hWnd, MofUInt msg,
                                        MofProcParamW wpar,
                                        MofProcParamL lpar) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wpar, lpar)) return true;
  return CDefWindowProc::WindowProc(hWnd, msg, wpar, lpar);
}
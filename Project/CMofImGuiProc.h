#pragma once

#include "imgui.h"

#include "Mof.h"
class CMofImGuiProc : public CDefWindowProc {
 public:
  virtual MofProcResult WindowProc(MofWindowHandle hWnd, MofUInt msg,
                                   MofProcParamW wpar,
                                   MofProcParamL lpar) override;
};
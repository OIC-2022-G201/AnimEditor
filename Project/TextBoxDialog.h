#pragma once
#include <string>

#include "imgui.h"

class TextBoxDialog {
  bool is_open_ = false;
  bool is_ok_ = false;
  std::string value_;
  std::string title_;
  std::string head_message_;
  std::string label_;

 public:
  void SetTitleText(const std::string& text) { title_ = text;}
  void SetHeadLabel(const std::string& text) { head_message_ = text; }
  void SetInputLabel(const std::string& text) { label_ = text; }
  void Display();

  void Open(std::string_view msg) {
    is_open_ = true;
    value_ = msg;
  }
  bool IsOk() { return is_ok_; }
  std::string_view Value() { return value_; }
};

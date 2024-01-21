#pragma once

#include "imgui_internal.h"

ImVec2 operator-(const ImVec2& l, const ImVec2& r);
ImVec2 operator+(const ImVec2& l, const ImVec2& r);
ImVec2 operator*(const ImVec2& l, const ImVec2& r);
ImVec2 operator*(const ImVec2& l, const float& scalar);
ImVec2 operator/(const ImVec2& l, const float& scalar);
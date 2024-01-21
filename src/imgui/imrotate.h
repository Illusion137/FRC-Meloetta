#pragma once

#include <imgui_internal.h>
#include "imvec2.h"

extern int rotation_start_index;
extern void ImRotateStart();
extern ImVec2 ImRotationCenter();
extern void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter());
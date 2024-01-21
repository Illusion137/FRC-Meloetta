#pragma once

#include <imgui.h>
#include <vector>

#include "widgets.h"
#include "vendor/zorua/textures.h"
#include "nerds/network_table_utils.h"

namespace nerds {
    namespace gui {
        void setup_imgui_styles();
        void render(bool* show_window);
    }
}
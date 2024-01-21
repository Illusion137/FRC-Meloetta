#pragma once

#include <vector>
#include <cmath>
#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include <units/length.h>

#include "nerds/network_table_utils.h"
#include "nerds/apriltag_utils.h"

#include "imgui/imvec2.h"
#include "imgui/imrotate.h"

#include "widgets_interfaces.h"
#include "utils/fs.h"
#include "vendor/zorua/textures.h"

constexpr float CM_PER_INCH = 2.54;
constexpr int PPI = 30;
constexpr float INCHES_PER_PIXEL = (CM_PER_INCH / PPI);

using namespace units::literals;

namespace nerds {
    typedef float px_t;
    class Widgets {
    public:
        Widgets(const Widgets& obj) = delete;
        static Widgets* get_instance();
        void render_debug_info();
        void render_alerts();
        void render_network_tables();
        void render_navx();
        void render_swerve_drive();
        void render_game_field();
    private:
        Widgets() {}
        static Widgets* instance;
        //Constants
        // 
        //All
        bool network_table_connected;
        ImVec2 get_scaled_texture_size(const ImVec2& texture_size, const ImVec2& region_available, const float aspect_ratio);
        float get_scale_factor(const ImVec2& texture_size, const ImVec2& scaled_texture_size);
        void render_arrow(ImDrawList* draw_list, const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const float dist, const float angle_radians, ImU32 color);
        void render_point_angle(ImDrawList* draw_list, const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const float angle_radians, ImU32 color);
        //GameField
        float gf_scale_factor;
        float gf_field_inch_hovering_over;
        float gf_inch_to_px(const units::inch_t inch);
        units::inch_t gf_px_to_inch(const px_t px){}
        float gf_invert_y();
    };
}
#pragma once

#include <vector>
#include <cmath>
#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include <frc/geometry/Pose3d.h>
#include <units/length.h>
#include <units/angle.h>
#include <units/velocity.h>

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
        Widgets() { 
            network_table_connected = false; current_scale_factor = 1; current_draw_list = NULL;
            field_inch_hovering_over = { 0_in, 0_in }; show_april_tag_on_field_hover = false;
        }
        static inline Widgets* instance;
        typedef float pixel_t;
        struct InchVec2 { 
            units::inch_t x, y; 
            InchVec2() {}
            InchVec2(units::inch_t _x, units::inch_t _y) : x(_x), y(_y) {} 
            InchVec2(pixel_t _x, pixel_t _y) : x(_x), y(_y) {}
            InchVec2(ImVec2 v) : x(v.x), y(v.y) {}
        };
        //Constants
        //All
        bool        network_table_connected;
        float       current_scale_factor;
        ImDrawList* current_draw_list;
        pixel_t  inch_to_px             (const units::inch_t inch);
        ImVec2   point_inch_to_px       (const InchVec2& point_inch, bool invert_field_y = false);
        ImVec2   get_scaled_texture_size(const ImVec2& texture_size, const ImVec2& region_available, const float aspect_ratio);
        float    get_scale_factor       (const ImVec2& texture_size, const ImVec2& scaled_texture_size);
        void     render_arrow           (const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const pixel_t dist, const units::radian_t angle_radians, ImU32 color = IM_COL32(255, 255, 255, 255));
        void     render_point_angle     (const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const units::radian_t angle_radians, ImU32 color = IM_COL32(255, 255, 255, 255));
        //AprilTag
        //GameField
        const static std::vector<frc::AprilTag> field_apriltags;
        //const static std::vector<FieldObstacle> field_obstacles;
        bool     show_april_tag_on_field_hover;
        ImVec2   scaled_field_texture_size;
        InchVec2 field_inch_hovering_over;
        pixel_t game_field_invert_y_px(pixel_t y);
    };
}
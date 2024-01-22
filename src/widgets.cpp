#include "Widgets.h"

nerds::Widgets *nerds::Widgets::get_instance() {
    if (instance == NULL) instance = new Widgets();
    return instance;
}

nerds::Widgets::pixel_t nerds::Widgets::inch_to_px(const units::inch_t inch) {
    return (inch.value() / INCHES_PER_PIXEL) * this->current_scale_factor;
}
ImVec2 nerds::Widgets::point_inch_to_px(const InchVec2& point_inch, bool invert_field_y) {
    return { inch_to_px(point_inch.x), invert_field_y ? game_field_invert_y_px(inch_to_px(point_inch.y)) : inch_to_px(point_inch.y)};
}
ImVec2 nerds::Widgets::get_scaled_texture_size(const ImVec2& texture_size, const ImVec2& region_available, const float aspect_ratio){
    if (aspect_ratio * region_available.y <= region_available.x)
        return ImVec2(aspect_ratio * region_available.y, region_available.y);
    const float reverse_aspect_ratio = (float)texture_size.y / texture_size.x;
    return ImVec2(region_available.x, reverse_aspect_ratio * region_available.x);
}
float nerds::Widgets::get_scale_factor(const ImVec2& texture_size, const ImVec2& scaled_texture_size){
    return scaled_texture_size.x / texture_size.x;
}
void nerds::Widgets::render_arrow(const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const pixel_t dist, const units::radian_t angle_radians, ImU32 color){
    const ImVec2 screen_point = begin_cursor_screen_pos + point;
    const ImVec2 line = ImVec2(dist * cos(angle_radians.value()), dist * sin(angle_radians.value()));
    const ImVec2 end_line = ImVec2(screen_point.x + line.x, screen_point.y - line.y);
    this->current_draw_list->AddLine(screen_point, end_line, color);

    constexpr float arrow_radians_dist = 6;
    float arrow_length_dist = dist - (dist * .20);
    const ImVec2 arrow_line_left = ImVec2(arrow_length_dist * cos(angle_radians.value() - arrow_radians_dist), arrow_length_dist * sin(angle_radians.value() - arrow_radians_dist));
    const ImVec2 arrow_line_right = ImVec2(arrow_length_dist * cos(angle_radians.value() + arrow_radians_dist), arrow_length_dist * sin(angle_radians.value() + arrow_radians_dist));

    this->current_draw_list->AddLine(end_line, ImVec2(screen_point.x + arrow_line_left.x, screen_point.y - arrow_line_left.y), color);
    this->current_draw_list->AddLine(end_line, ImVec2(screen_point.x + arrow_line_right.x, screen_point.y - arrow_line_right.y), color);
}
void nerds::Widgets::render_point_angle(const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const units::radian_t angle_radians, ImU32 color){
    constexpr float dist = 15;
    const ImVec2 screen_point = begin_cursor_screen_pos + point;
    this->current_draw_list->AddCircle(screen_point, 5, color);
    render_arrow(begin_cursor_screen_pos, point, dist, angle_radians, color);
}
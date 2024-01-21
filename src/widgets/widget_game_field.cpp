#include "../widgets.h"

#define PI 3.14159265358979323846
#define CM_PER_INCH 2.54
#define PPI 30
#define INCHES_PER_PIXEL (CM_PER_INCH / PPI)

ImVec2 get_scaled_field_texture_size(const ImVec2 &field_texture_size, const ImVec2& region_available, const float aspect_ratio) {
    if (aspect_ratio * region_available.y <= region_available.x)
        return ImVec2(aspect_ratio * region_available.y, region_available.y);
    const float reverse_aspect_ratio = (float)field_texture_size.y / field_texture_size.x;
    return ImVec2(region_available.x, reverse_aspect_ratio * region_available.x);
}
float get_scale_factor(const ImVec2& field_texture_size, const ImVec2& scaled_field_texture_size) {
    return scaled_field_texture_size.x / field_texture_size.x;
}
float inch_to_px(const float inch, const float scale_factor) {
    return ((inch / INCHES_PER_PIXEL) * scale_factor);
}
ImVec2 point_inch_to_px(const ImVec2 &point, const float scale_factor) {
    return ImVec2(inch_to_px(point.x, scale_factor), inch_to_px(point.y, scale_factor));
}

void render_arrow(ImDrawList* draw_list, const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const float dist, const float angle_radians, ImU32 color) {
    const ImVec2 screen_point = begin_cursor_screen_pos + point;
    const ImVec2 line = ImVec2(dist * cos(angle_radians), dist * sin(angle_radians));
    const ImVec2 end_line = ImVec2(screen_point.x + line.x, screen_point.y - line.y);
    draw_list->AddLine(screen_point, end_line, color);

    constexpr float arrow_radians_dist = 6;
    float arrow_length_dist = dist - (dist * .20);
    const ImVec2 arrow_line_left = ImVec2(arrow_length_dist * cos(angle_radians - arrow_radians_dist), arrow_length_dist * sin(angle_radians - arrow_radians_dist));
    const ImVec2 arrow_line_right = ImVec2(arrow_length_dist * cos(angle_radians + arrow_radians_dist), arrow_length_dist * sin(angle_radians + arrow_radians_dist));

    draw_list->AddLine(end_line, ImVec2(screen_point.x + arrow_line_left.x, screen_point.y - arrow_line_left.y), color);
    draw_list->AddLine(end_line, ImVec2(screen_point.x + arrow_line_right.x, screen_point.y - arrow_line_right.y), color);
}

void render_point_angle(ImDrawList* draw_list, const ImVec2& begin_cursor_screen_pos, const ImVec2& point, const float angle_radians) {
    constexpr float dist = 15;
    const ImVec2 screen_point = begin_cursor_screen_pos + point;
    draw_list->AddCircle(screen_point, 5, IM_COL32(255, 0, 100, 255));
    render_arrow(draw_list, begin_cursor_screen_pos, point, dist, angle_radians, IM_COL32(255, 100, 100, 255));
}

struct Robot {
    const float scale_factor;
    ImVec2 size_inches;
    ImVec2 position_inches;
    float rotation_radians;
    ImVec2 size_px() { return point_inch_to_px(size_inches, scale_factor); }
    ImVec2 position_px() { return point_inch_to_px(position_inches, scale_factor); }
};

void nerds::gui::widget_game_field(bool* show_widget) {
    static json game_json = fs::get_json_from_file("res/user/fields/2024-cresendo.json");
    static zorua::textures::Texture game_piece_texture = zorua::textures::load_texture("res/img/game_piece.png");
    static zorua::textures::Texture field_texture = zorua::textures::load_texture("res/user/fields/2024-field.png");
    static float aspect_ratio = (float)field_texture.width / field_texture.height;

    if (nt_inst.IsConnected())
        std::cout << nt_fgear_table->GetEntry(".test").SetDouble(3);

    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX), [](ImGuiSizeCallbackData* data) {
        float aspect_ratio = *(float*)data->UserData;
        data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio);
    }, (void*)&aspect_ratio);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0x444348));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImVec2 field_pixel_hovering_over;
    ImVec2 field_inch_hovering_over;
    ImVec2 begin_cursor_screen_pos;
    ImVec2 mouse_pos;
    ImVec2 mouse_pos_on_field;

    if (ImGui::Begin("Game Field", show_widget, ImGuiWindowFlags_NoCollapse)) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        begin_cursor_screen_pos = ImGui::GetCursorScreenPos();
        mouse_pos = ImGui::GetMousePos();
        mouse_pos_on_field = ImVec2(mouse_pos.x - begin_cursor_screen_pos.x, mouse_pos.y - begin_cursor_screen_pos.y);

        const ImVec2 field_texture_size = ImVec2(field_texture.width, field_texture.height);
        const ImVec2 region_available = ImGui::GetContentRegionAvail();

        const ImVec2 scaled_field_texture_size = get_scaled_field_texture_size(field_texture_size, region_available, aspect_ratio);
        const float scale_factor = get_scale_factor(field_texture_size, scaled_field_texture_size);

        ImGui::Image((ImTextureID)(intptr_t)field_texture.texture, scaled_field_texture_size);

        field_pixel_hovering_over = mouse_pos_on_field / scale_factor;
        
        field_inch_hovering_over = field_pixel_hovering_over * INCHES_PER_PIXEL;

        //const ImVec2 game_piece_texture_size = ImVec2(game_piece_texture.width, game_piece_texture.height);
        //const ImVec2 scaled_game_piece_texture_size = ImVec2(game_piece_texture.width * scale_factor, game_piece_texture.height * scale_factor);
        for (const json& game_piece : game_json["field_initial_game_pieces"]) {
            const ImVec2 game_peice_p1 = ImVec2(game_piece["point_1"]["x"].get<float>(), game_piece["point_1"]["y"].get<float>());
            const ImVec2 game_peice_p2 = ImVec2(game_piece["point_2"]["x"].get<float>(), game_piece["point_2"]["y"].get<float>());

            const ImVec2 game_piece_px_pos = point_inch_to_px(game_peice_p1, scale_factor);
            const ImVec2 game_piece_px_pos_2 = point_inch_to_px(game_peice_p2, scale_factor);

            ImGui::SetCursorScreenPos(begin_cursor_screen_pos + game_piece_px_pos);
            ImGui::Image((ImTextureID)(intptr_t)game_piece_texture.texture, game_piece_px_pos_2 - game_piece_px_pos);
            ImGui::SetCursorScreenPos(begin_cursor_screen_pos + game_piece_px_pos);
            draw_list->AddRect(begin_cursor_screen_pos + game_piece_px_pos, begin_cursor_screen_pos + game_piece_px_pos_2, IM_COL32(255, 255, 0, 255));
        }
        for (const json& obstacle : game_json["field_obstacles"]) {
            const ImVec2 obstacle_p1 = ImVec2(obstacle["point_1"]["x"].get<float>(), obstacle["point_1"]["y"].get<float>());
            const ImVec2 obstacle_p2 = ImVec2(obstacle["point_2"]["x"].get<float>(), obstacle["point_2"]["y"].get<float>());
            const std::string obstacle_type = obstacle["type"].get<std::string>();

            const ImVec2 obstacle_px_pos_1 = point_inch_to_px(obstacle_p1, scale_factor);
            const ImVec2 obstacle_px_pos_2 = point_inch_to_px(obstacle_p2, scale_factor);

            const ImVec2 obstacle_px_screen_pos_1 = begin_cursor_screen_pos + obstacle_px_pos_1;
            const ImVec2 obstacle_px_screen_pos_2 = begin_cursor_screen_pos + obstacle_px_pos_2;

            if (obstacle_type == "box")
                draw_list->AddRect(obstacle_px_screen_pos_1, obstacle_px_screen_pos_2, IM_COL32(0, 255, 0, 255));
            else if (obstacle_type == "line")
                draw_list->AddLine(obstacle_px_screen_pos_1, obstacle_px_screen_pos_2, IM_COL32(0, 255, 0, 255));
        }
        for (const json& april_tag : game_json["april_tags"]) {
            const ImVec2 april_tag_pos = ImVec2(april_tag["translation"]["x"].get<float>(), april_tag["translation"]["y"].get<float>());
            const float april_tag_angle = april_tag["rotation"].get<float>();

            render_point_angle(draw_list, begin_cursor_screen_pos, point_inch_to_px(april_tag_pos, scale_factor), april_tag_angle);
        }
        //render robot
        {
            Robot robot = { .scale_factor = scale_factor, .size_inches = ImVec2(30, 30), .position_inches = ImVec2(50 + (0.0f * (float)ImGui::GetTime()), 50), .rotation_radians = 2.0f * (float)ImGui::GetTime() };
            ImRotateStart();
            draw_list->AddCircle(begin_cursor_screen_pos + robot.position_px(), 1.0f, IM_COL32(255, 100, 255, 255));
            draw_list->AddRect(begin_cursor_screen_pos + robot.position_px() - (robot.size_px() / 2), begin_cursor_screen_pos + robot.position_px() + robot.size_px() - (robot.size_px() / 2), IM_COL32(255, 255, 255, 255));
            ImRotateEnd(robot.rotation_radians);
            render_arrow(draw_list, begin_cursor_screen_pos, robot.position_px(), 15.0f, robot.rotation_radians, IM_COL32(255, 0, 0, 255));
        }
    } ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::Begin("debug");
    ImGui::Text("field_inch: %f : %f", field_inch_hovering_over.x, field_inch_hovering_over.y);
    ImGui::Text("network tables connected: %s", nt_inst.IsConnected() ? "true" : "false");
    if (ImGui::Button("Start Network Client")) { nt_inst.StartClient4("First-CGear Client"); }
    if (ImGui::Button("Stop Network Client")) { nt_inst.StopClient(); }
    //ImGui::Text("field_px:        %f : %f", field_pixel_hovering_over.x, field_pixel_hovering_over.y);
    //ImGui::Text("mouse_pos_field: %f : %f", mouse_pos_on_field.x, mouse_pos_on_field.y);
    ImGui::End();

}

void nerds::gui::widget_game_field_editor(bool* show_widget) {
    /*
        camera's focal length in pixels; fx and fy
        camera's focal center in pixels; be close to unage cebter
    */
}
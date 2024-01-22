#include "../widgets.h"

const std::vector<frc::AprilTag> nerds::Widgets::field_apriltags = {
    { 1,  frc::Pose3d(592.68_in, 9.68_in  , 53.38_in, frc::Rotation3d(0_deg, 0_deg, 120_deg))},
    { 2,  frc::Pose3d(637.21_in, 34.79_in , 53.38_in, frc::Rotation3d(0_deg, 0_deg, 120_deg))},
    { 3,  frc::Pose3d(652.73_in, 196.17_in, 57.13_in, frc::Rotation3d(0_deg, 0_deg, 180_deg))},
    { 4,  frc::Pose3d(652.73_in, 218.42_in, 57.13_in, frc::Rotation3d(0_deg, 0_deg, 180_deg))},
    { 5,  frc::Pose3d(578.77_in, 323.00_in, 53.38_in, frc::Rotation3d(0_deg, 0_deg, 270_deg))},
    { 6,  frc::Pose3d(72.50_in , 323.00_in, 53.38_in, frc::Rotation3d(0_deg, 0_deg, 270_deg))},
    { 7,  frc::Pose3d(-1.50_in , 218.42_in, 57.13_in, frc::Rotation3d(0_deg, 0_deg, 0_deg))},
    { 8,  frc::Pose3d(-1.50_in , 196.17_in, 57.13_in, frc::Rotation3d(0_deg, 0_deg, 0_deg))},
    { 9,  frc::Pose3d(14.02_in , 34.79_in , 53.38_in, frc::Rotation3d(0_deg, 0_deg, 60_deg))},
    { 10, frc::Pose3d(57.54_in , 9.68_in  , 53.38_in, frc::Rotation3d(0_deg, 0_deg, 60_deg))},
    { 11, frc::Pose3d(468.69_in, 146.19_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 300_deg))},
    { 12, frc::Pose3d(468.69_in, 177.10_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 60_deg))},
    { 13, frc::Pose3d(441.74_in, 161.62_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 180_deg))},
    { 14, frc::Pose3d(209.48_in, 161.62_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 0_deg))},
    { 15, frc::Pose3d(182.73_in, 177.10_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 120_deg))},
    { 16, frc::Pose3d(182.73_in, 146.19_in, 52.00_in, frc::Rotation3d(0_deg, 0_deg, 240_deg))},
};

nerds::Widgets::pixel_t nerds::Widgets::game_field_invert_y_px(pixel_t y) {
    const float half_field_y = this->scaled_field_texture_size.y / 2;
    return half_field_y - (y - half_field_y);
}

void nerds::Widgets::render_game_field() {
    static json game_json = fs::get_json_from_file("res/user/fields/2024-cresendo.json");
    static zorua::textures::Texture game_piece_texture = zorua::textures::load_texture("res/img/game_piece.png");
    static zorua::textures::Texture field_texture = zorua::textures::load_texture("res/user/fields/2024-field.png");
    static float aspect_ratio = (float)field_texture.width / field_texture.height;

    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX), [](ImGuiSizeCallbackData* data) {
        float aspect_ratio = *(float*)data->UserData;
        data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio);
    }, (void*)&aspect_ratio);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0x444348));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    if (ImGui::Begin("Game Field", NULL, ImGuiWindowFlags_NoCollapse)) {
        this->current_draw_list = ImGui::GetWindowDrawList();
        const ImVec2 begin_cursor_screen_pos = ImGui::GetCursorScreenPos();
        const ImVec2 mouse_pos = ImGui::GetMousePos();
        const ImVec2 mouse_pos_on_field = ImVec2(mouse_pos.x - begin_cursor_screen_pos.x, mouse_pos.y - begin_cursor_screen_pos.y);

        const ImVec2 field_texture_size = ImVec2(field_texture.width, field_texture.height);
        const ImVec2 region_available = ImGui::GetContentRegionAvail();
        
        this->scaled_field_texture_size = get_scaled_texture_size(field_texture_size, region_available, aspect_ratio);
        this->current_scale_factor = get_scale_factor(field_texture_size, this->scaled_field_texture_size);

        ImGui::Image((ImTextureID)(intptr_t)field_texture.texture, this->scaled_field_texture_size);

        const ImVec2 field_pixel_hovering_over = mouse_pos_on_field / this->current_scale_factor;
        this->field_inch_hovering_over = field_pixel_hovering_over * INCHES_PER_PIXEL;

        //for (const json& game_piece : game_json["field_initial_game_pieces"]) {
        //    const ImVec2 game_peice_p1 = ImVec2(game_piece["point_1"]["x"].get<float>(), game_piece["point_1"]["y"].get<float>());
        //    const ImVec2 game_peice_p2 = ImVec2(game_piece["point_2"]["x"].get<float>(), game_piece["point_2"]["y"].get<float>());

        //    const ImVec2 game_piece_px_pos = point_inch_to_px(game_peice_p1, scale_factor);
        //    const ImVec2 game_piece_px_pos_2 = point_inch_to_px(game_peice_p2, scale_factor);

        //    ImGui::SetCursorScreenPos(begin_cursor_screen_pos + game_piece_px_pos);
        //    ImGui::Image((ImTextureID)(intptr_t)game_piece_texture.texture, game_piece_px_pos_2 - game_piece_px_pos);
        //    ImGui::SetCursorScreenPos(begin_cursor_screen_pos + game_piece_px_pos);
        //    draw_list->AddRect(begin_cursor_screen_pos + game_piece_px_pos, begin_cursor_screen_pos + game_piece_px_pos_2, IM_COL32(255, 255, 0, 255));
        //}
        //for (const json& obstacle : game_json["field_obstacles"]) {
        //    const ImVec2 obstacle_p1 = ImVec2(obstacle["point_1"]["x"].get<float>(), obstacle["point_1"]["y"].get<float>());
        //    const ImVec2 obstacle_p2 = ImVec2(obstacle["point_2"]["x"].get<float>(), obstacle["point_2"]["y"].get<float>());
        //    const std::string obstacle_type = obstacle["type"].get<std::string>();

        //    const ImVec2 obstacle_px_pos_1 = point_inch_to_px(obstacle_p1, scale_factor);
        //    const ImVec2 obstacle_px_pos_2 = point_inch_to_px(obstacle_p2, scale_factor);

        //    const ImVec2 obstacle_px_screen_pos_1 = begin_cursor_screen_pos + obstacle_px_pos_1;
        //    const ImVec2 obstacle_px_screen_pos_2 = begin_cursor_screen_pos + obstacle_px_pos_2;

        //    if (obstacle_type == "box")
        //        draw_list->AddRect(obstacle_px_screen_pos_1, obstacle_px_screen_pos_2, IM_COL32(0, 255, 0, 255));
        //    else if (obstacle_type == "line")
        //        draw_list->AddLine(obstacle_px_screen_pos_1, obstacle_px_screen_pos_2, IM_COL32(0, 255, 0, 255));
        //}
        for (const frc::AprilTag& april_tag : nerds::Widgets::field_apriltags) {
            const InchVec2        april_tag_pos   = { april_tag.pose.Translation().X(), april_tag.pose.Translation().Y() };
            const units::radian_t april_tag_angle = april_tag.pose.Rotation().Z();

            render_point_angle(begin_cursor_screen_pos, point_inch_to_px(april_tag_pos, true), april_tag_angle, IM_COL32(255, 0, 0, 255));
        }
        //render robot
        //{
            //Robot robot = { .scale_factor = scale_factor, .size_inches = ImVec2(30, 30), .position_inches = ImVec2(50 + (0.0f * (float)ImGui::GetTime()), 50), .rotation_radians = 2.0f * (float)ImGui::GetTime() };
            //ImRotateStart();
            //draw_list->AddCircle(begin_cursor_screen_pos + robot.position_px(), 1.0f, IM_COL32(255, 100, 255, 255));
            //draw_list->AddRect(begin_cursor_screen_pos + robot.position_px() - (robot.size_px() / 2), begin_cursor_screen_pos + robot.position_px() + robot.size_px() - (robot.size_px() / 2), IM_COL32(255, 255, 255, 255));
            //ImRotateEnd(robot.rotation_radians);
            //render_arrow(draw_list, begin_cursor_screen_pos, robot.position_px(), 15.0f, robot.rotation_radians, IM_COL32(255, 0, 0, 255));
        //}
    } ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    //ImGui::Begin("debug");
    //ImGui::Text("field_inch: %f : %f", field_inch_hovering_over.x, field_inch_hovering_over.y);
    //ImGui::Text("network tables connected: %s", nt_inst.IsConnected() ? "true" : "false");
    //if (ImGui::Button("Start Network Client")) { nt_inst.StartClient4("First-CGear Client"); }
    //if (ImGui::Button("Stop Network Client")) { nt_inst.StopClient(); }
    //ImGui::Text("field_px:        %f : %f", field_pixel_hovering_over.x, field_pixel_hovering_over.y);
    //ImGui::Text("mouse_pos_field: %f : %f", mouse_pos_on_field.x, mouse_pos_on_field.y);
    //ImGui::End();

}
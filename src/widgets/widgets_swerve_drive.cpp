#include "../widgets.h"

void nerds::Widgets::render_swerve_drive() {
    static SwerveDrive swerve_drive = { units::degree_t(FLT_MIN) };
    bool use_radians = true;
    if (nt_inst.IsConnected())
        swerve_drive = SwerveDrive::from_nt();
    if(ImGui::Begin("Swerve Drive", NULL, ImGuiWindowFlags_NoCollapse)) {
        const ImVec2 begin_cursor_screen_pos = ImGui::GetCursorScreenPos();
        const ImVec2 size = ImVec2(50, 50);
        const ImVec2 scaled_size = get_scaled_texture_size(size, ImGui::GetContentRegionAvail(), 1.0);
        this->current_scale_factor = get_scale_factor(size, scaled_size);
        this->current_draw_list = ImGui::GetWindowDrawList();
        this->current_draw_list->AddRect(begin_cursor_screen_pos + ImVec2(10, 10) * this->current_scale_factor,
                                         begin_cursor_screen_pos + ImVec2(40, 40) * this->current_scale_factor, IM_COL32(255, 255, 255, 255));
        if (nt_inst.IsConnected() && swerve_drive.robot_heading.value() != FLT_MIN) {
            if (ImGui::Button(use_radians ? "Use Degrees" : "Use Radians")) use_radians = !use_radians;

            this->render_point_angle(begin_cursor_screen_pos, ImVec2(25, 25) * this->current_scale_factor, swerve_drive.robot_heading, IM_COL32(0, 255, 0, 255));

            this->render_point_angle(begin_cursor_screen_pos, ImVec2(10, 10) * this->current_scale_factor, swerve_drive.front_left.absolute_encoder_value, IM_COL32(255, 0, 0, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(40, 10) * this->current_scale_factor, swerve_drive.front_right.absolute_encoder_value, IM_COL32(255, 0, 0, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(10, 40) * this->current_scale_factor, swerve_drive.back_left.absolute_encoder_value, IM_COL32(255, 0, 0, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(40, 40) * this->current_scale_factor, swerve_drive.back_right.absolute_encoder_value, IM_COL32(255, 0, 0, 255));

            this->render_point_angle(begin_cursor_screen_pos, ImVec2(10, 10) * this->current_scale_factor, swerve_drive.front_left.state.rotation, IM_COL32(0, 0, 255, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(40, 10) * this->current_scale_factor, swerve_drive.front_right.state.rotation, IM_COL32(0, 0, 255, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(10, 40) * this->current_scale_factor, swerve_drive.back_left.state.rotation, IM_COL32(0, 0, 255, 255));
            this->render_point_angle(begin_cursor_screen_pos, ImVec2(40, 40) * this->current_scale_factor, swerve_drive.back_right.state.rotation, IM_COL32(0, 0, 255, 255));
        }
        else {
            ImGui::Text("Not connected to network tables");
        }
    } ImGui::End();
}
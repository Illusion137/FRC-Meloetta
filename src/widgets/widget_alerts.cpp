#include "../widgets.h"

//void nerds::gui::widget_alerts_box(bool *show_widget, std::vector<Alert> alerts) {
//    static zorua::textures::Texture success_texture = zorua::textures::load_texture("res/img/success.png");
//    static zorua::textures::Texture warning_texture = zorua::textures::load_texture("res/img/warning.png");
//    static zorua::textures::Texture error_texture   = zorua::textures::load_texture("res/img/error.png");
//
//    const ImVec2 alert_icon_size = ImVec2(24, 24);
//    const ImVec2 min_size = ImVec2(200, 70);
//    const ImVec2 max_size = ImVec2(800, INFINITY);
//    ImGui::SetNextWindowSizeConstraints(min_size, max_size);
//
//    if (ImGui::Begin("Alerts", show_widget, ImGuiWindowFlags_NoCollapse)) {
//        const int alerts_count = alerts.size();
//        for (int i = 0; i < alerts_count; i++)
//        {
//            switch (alerts[i].alert_type) {
//            case ALERT_SUCCESS:
//                ImGui::Image((ImTextureID)(intptr_t)success_texture.texture, alert_icon_size);
//                break;
//            case ALERT_WARNING:
//                ImGui::Image((ImTextureID)(intptr_t)warning_texture.texture, alert_icon_size);
//                break;
//            case ALERT_ERROR:
//                ImGui::Image((ImTextureID)(intptr_t)error_texture.texture, alert_icon_size);
//                break;
//            default: IM_ASSERT(false && "alert type unknown");
//            }
//            ImGui::SameLine();
//            ImGui::Text(alerts[i].msg.c_str());
//        }
//    } ImGui::End();
//}
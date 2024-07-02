#include "menu.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx12.h"
#include "../../imgui/imgui.h"
#include <d3d12.h>
#include "esp-preview.h"

ImVec4 mainColor = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
ImVec4 accentColor = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
ImVec4 textColor = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
ImVec4 darkColor = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
ImVec4 lightColor = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
ImVec4 buttonColor = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);

bool showESPPreview = false;
bool enableBoxESP = true;
bool enableHealthESP = true;
bool enableNameESP = true;
bool enableDistanceESP = true;
bool enablePingESP = true;

void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.FramePadding = ImVec2(4.0f, 2.0f);
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.IndentSpacing = 12.0f;

    ImGui::StyleColorsDark();
}

void RenderCustomTitleBar(const char* title, ImVec2 windowSize) {
    ImGui::BeginChild("##TitleBar", ImVec2(windowSize.x, 50), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetCursorPosX((windowSize.x - ImGui::CalcTextSize(title).x) / 2);
    ImGui::SetCursorPosY(15);
    ImGui::Text(title);
    ImGui::EndChild();
}

void RenderMenu() {
    ImVec2 windowSize = ImVec2(905, 624);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

    if (ImGui::Begin("##ZeeZ Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("ESP")) {
                ImGui::BeginChild("ESP Settings", ImVec2(0, 0), true);

                auto settingStyle = []() {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, mainColor);
                    ImGui::PushStyleColor(ImGuiCol_Border, accentColor);
                    };

                auto endSettingStyle = []() {
                    ImGui::PopStyleColor(2);
                    ImGui::PopStyleVar();
                    };

                auto customCheckbox = [](const char* label, bool* v) {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, darkColor);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, darkColor);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, darkColor);
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.88f, 0.88f, 0.88f, 1.00f));
                    ImGui::Checkbox(label, v);
                    ImGui::PopStyleColor(4);
                    ImGui::PopStyleVar();
                    };

                settingStyle();
                ImGui::BeginChild("preview ESP", ImVec2(0, 50), true);
                customCheckbox("Preview ESP", &showESPPreview);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Box", ImVec2(0, 50), true);
                customCheckbox("Box", &enableBoxESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Health", ImVec2(0, 50), true);
                customCheckbox("Health", &enableHealthESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Name", ImVec2(0, 50), true);
                customCheckbox("Name", &enableNameESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Distance", ImVec2(0, 50), true);
                customCheckbox("Distance", &enableDistanceESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                settingStyle();
                ImGui::BeginChild("Ping", ImVec2(0, 50), true);
                customCheckbox("Ping", &enablePingESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    if (showESPPreview) {
        preview();
    }
}

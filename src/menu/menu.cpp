#include "menu.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx12.h"
#include "../../imgui/imgui.h"
#include <d3d12.h>
#include "esp-preview.h" // Include the header for the ESP preview

// Custom colors
ImVec4 mainColor = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
ImVec4 accentColor = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
ImVec4 textColor = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
ImVec4 darkColor = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
ImVec4 lightColor = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
ImVec4 buttonColor = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);

// ESP settings
bool showESPPreview = false;
float espDistance = 100.0f; // ESP distance setting
bool enableBoxESP = true;   // Toggle for Box ESP
bool enableHealthESP = true; // Toggle for Health ESP
bool enableNameESP = true;   // Toggle for Name ESP

void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 7.0f;
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
    // Set a standard size for the window
    ImVec2 windowSize = ImVec2(905, 624);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

    if (ImGui::Begin("##ZeeZ Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        // Create the tab bar
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {

            // Create the ESP tab
            if (ImGui::BeginTabItem("ESP")) {
                ImGui::BeginChild("ESP Settings", ImVec2(0, 0), true); // Create a frame to hold all ESP settings

                // Render each setting with custom styled checkboxes
                auto settingStyle = []() {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.13f, 0.14f, 0.15f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.29f, 0.29f, 0.29f, 1.00f));
                    };

                auto endSettingStyle = []() {
                    ImGui::PopStyleColor(2);
                    ImGui::PopStyleVar();
                    };

                // Checkbox with custom style to match the image
                auto customCheckbox = [](const char* label, bool* v) {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.25f, 0.78f, 0.25f, 1.00f));
                    ImGui::Checkbox(label, v);
                    ImGui::PopStyleColor(4);
                    ImGui::PopStyleVar();
                    };

                settingStyle();
                ImGui::BeginChild("preview ESP", ImVec2(0, 50), true);
                customCheckbox("preview ESP", &showESPPreview);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Enable Box ESP", ImVec2(0, 50), true);
                customCheckbox("Enable Box ESP", &enableBoxESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Enable Health ESP", ImVec2(0, 50), true);
                customCheckbox("Enable Health ESP", &enableHealthESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("Enable Name ESP", ImVec2(0, 50), true);
                customCheckbox("Enable Name ESP", &enableNameESP);
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::Spacing();

                settingStyle();
                ImGui::BeginChild("ESP Distance", ImVec2(0, 50), true);
                ImGui::PushItemWidth(150);
                ImGui::SliderFloat("ESP Distance", &espDistance, 50.0f, 1000.0f, "%.1f");
                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImGui::EndChild();
                endSettingStyle();

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            // Create more tabs as needed
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    // Render the ESP preview window if the checkbox is checked
    if (showESPPreview) {
        preview(); // Call the preview function from esp-preview.cpp
    }
}

#include "menu.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx12.h"
#include <d3d12.h>

// Custom colors
ImVec4 mainColor = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
ImVec4 accentColor = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
ImVec4 textColor = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
ImVec4 darkColor = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
ImVec4 lightColor = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
ImVec4 buttonColor = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);


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

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_WindowBg] = mainColor;
    colors[ImGuiCol_ChildBg] = mainColor;
    colors[ImGuiCol_PopupBg] = darkColor;
    colors[ImGuiCol_Border] = darkColor;
    colors[ImGuiCol_FrameBg] = accentColor;
    colors[ImGuiCol_FrameBgHovered] = buttonColor;
    colors[ImGuiCol_FrameBgActive] = buttonColor;
    colors[ImGuiCol_TitleBg] = darkColor;
    colors[ImGuiCol_TitleBgActive] = darkColor;
    colors[ImGuiCol_TitleBgCollapsed] = darkColor;
    colors[ImGuiCol_CheckMark] = textColor;
    colors[ImGuiCol_SliderGrab] = textColor;
    colors[ImGuiCol_SliderGrabActive] = textColor;
    colors[ImGuiCol_Button] = buttonColor;
    colors[ImGuiCol_ButtonHovered] = accentColor;
    colors[ImGuiCol_ButtonActive] = textColor;
    colors[ImGuiCol_Header] = accentColor;
    colors[ImGuiCol_HeaderHovered] = buttonColor;
    colors[ImGuiCol_HeaderActive] = textColor;
    colors[ImGuiCol_Separator] = darkColor;
    colors[ImGuiCol_SeparatorHovered] = buttonColor;
    colors[ImGuiCol_SeparatorActive] = textColor;
    colors[ImGuiCol_Tab] = darkColor;
    colors[ImGuiCol_TabHovered] = buttonColor;
    colors[ImGuiCol_TabActive] = textColor;
    colors[ImGuiCol_TabUnfocused] = darkColor;
    colors[ImGuiCol_TabUnfocusedActive] = buttonColor;
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
    ImVec2 windowSize = ImVec2(800, 600);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

    if (ImGui::Begin("##ZeeZ Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {

        RenderCustomTitleBar("ZeeZ Menu", windowSize);

        ImGui::Columns(2, NULL, false);
        ImGui::SetColumnWidth(0, 200);

        // Vertical Tabs
        ImGui::BeginChild("##LeftSide", ImVec2(0, 0), true);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        if (ImGui::Selectable("Rage", true, 0, ImVec2(0, 40))) {}
        ImGui::Selectable("Anti-hit", false, 0, ImVec2(0, 40));
        ImGui::Selectable("Visuals", false, 0, ImVec2(0, 40));
        ImGui::Selectable("Misc", false, 0, ImVec2(0, 40));
        ImGui::Selectable("Skins", false, 0, ImVec2(0, 40));
        ImGui::Selectable("Configs", false, 0, ImVec2(0, 40));
        ImGui::PopStyleVar(2);
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("##Content", ImVec2(0, 0), true);

        // Main Section
        ImGui::Text("Main");
        ImGui::Separator();
        static bool rage_enabled = false;
        ImGui::Checkbox("Enable", &rage_enabled);

        static int yaw_add = 0;
        ImGui::SliderInt("Yaw add", &yaw_add, 0, 360);

        // Misc Section
        ImGui::Text("Misc.");
        ImGui::Separator();
        static bool misc_disable = false;
        ImGui::Checkbox("Disable", &misc_disable);

        static const char* multi_options[] = { "None", "Option1", "Option2" };
        static int multi_selected = 0;
        ImGui::Combo("Multi", &multi_selected, multi_options, IM_ARRAYSIZE(multi_options));

        static bool tracer_enabled = false;
        ImGui::Checkbox("Tracer", &tracer_enabled);

        // Fakelag Section
        ImGui::Text("Fakelag");
        ImGui::Separator();
        static const char* fakelag_types[] = { "On frame", "Type1", "Type2" };
        static int fakelag_type = 0;
        ImGui::Combo("Type", &fakelag_type, fakelag_types, IM_ARRAYSIZE(fakelag_types));

        static float jitter_amount = 0.0f;
        ImGui::SliderFloat("Jitter", &jitter_amount, 0.0f, 1.0f);

        ImGui::Text("Keybind: ");
        ImGui::SameLine();
        if (ImGui::Button("None")) {
            // Keybind logic here
        }

        ImGui::EndChild();

        ImGui::End();
    }
}

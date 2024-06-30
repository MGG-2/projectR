#include "esp.h"
#include "../../imgui/imgui.h"

ESP::ESP() {}

ESP::~ESP() {}

void ESP::Render()
{
    // Setup ImGui style and colors
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);  // Dark background
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);      // Light text color

    // Setting up the window and layout with specific size
    ImGui::SetNextWindowSize(ImVec2(250, 300)); // Adjusted window size
    ImGui::Begin("ESP Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Draw Nickname
    ImVec2 p = ImGui::GetCursorScreenPos();
    float centerX = p.x + 125;  // Center of the window
    ImGui::SetCursorScreenPos(ImVec2(centerX - ImGui::CalcTextSize("NICKNAME").x / 2, p.y));
    ImGui::Text("NICKNAME");

    // Draw custom Health Progress Bar (set at 100% for now)
    float health_percent = 1.0f;  // Health at 100%
    float progressBarWidth = 100.0f;
    float progressBarHeight = 5.0f;
    ImVec2 progressBarPos = ImVec2(centerX - progressBarWidth / 2, p.y + 20);
    ImGui::GetWindowDrawList()->AddRectFilled(progressBarPos, ImVec2(progressBarPos.x + progressBarWidth, progressBarPos.y + progressBarHeight), IM_COL32(0, 50, 0, 255)); // Background
    ImGui::GetWindowDrawList()->AddRectFilled(progressBarPos, ImVec2(progressBarPos.x + progressBarWidth * health_percent, progressBarPos.y + progressBarHeight), IM_COL32(0, 255, 0, 255)); // Foreground

    // Coordinates for the box
    float box_top_y = p.y + 40;
    float box_bottom_y = p.y + 160;
    float box_left_x = centerX - 50;
    float box_right_x = centerX + 50;

    // Draw corner lines
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x + 20, box_top_y), IM_COL32(255, 255, 255, 255), 2.0f); // Top left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - 20, box_top_y), ImVec2(box_right_x, box_top_y), IM_COL32(255, 255, 255, 255), 2.0f); // Top right corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y), ImVec2(box_left_x + 20, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - 20, box_bottom_y), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom right corner

    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x, box_top_y + 20), IM_COL32(255, 255, 255, 255), 2.0f); // Top left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_top_y), ImVec2(box_right_x, box_top_y + 20), IM_COL32(255, 255, 255, 255), 2.0f); // Top right vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y - 20), ImVec2(box_left_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_bottom_y - 20), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom right vertical

    // Adjust cursor for left and right columns
    float spacing = 15.0f;
    float column_start_y = box_top_y + 10;
    float left_column_x = box_left_x - 60; // Adjusted for alignment
    float right_column_x = box_right_x + 40; // Adjusted for alignment

    // Draw Left Column
    ImGui::SetCursorScreenPos(ImVec2(left_column_x, column_start_y));
    ImGui::Text("DISTANCE");
    ImGui::SetCursorScreenPos(ImVec2(left_column_x, column_start_y + spacing));
    ImGui::Text("FLASH");
    ImGui::SetCursorScreenPos(ImVec2(left_column_x, column_start_y + spacing * 2));
    ImGui::Text("BLIND");
    ImGui::SetCursorScreenPos(ImVec2(left_column_x, column_start_y + spacing * 3));
    ImGui::Text("LC");

    // Draw Right Column
    ImGui::SetCursorScreenPos(ImVec2(right_column_x, column_start_y));
    ImGui::Text("KD");
    ImGui::SetCursorScreenPos(ImVec2(right_column_x, column_start_y + spacing));
    ImGui::Text("MONEY");
    ImGui::SetCursorScreenPos(ImVec2(right_column_x, column_start_y + spacing * 2));
    ImGui::Text("PING");
    ImGui::SetCursorScreenPos(ImVec2(right_column_x, column_start_y + spacing * 3));
    ImGui::Text("SCOPE");

    // Draw Bottom Line
    ImGui::SetCursorScreenPos(ImVec2(centerX - 50, box_bottom_y + 10));
    ImGui::GetWindowDrawList()->AddLine(ImVec2(centerX - 50, box_bottom_y + 10), ImVec2(centerX + 50, box_bottom_y + 10), IM_COL32(0, 0, 255, 255), 2.0f);

    // Draw Weapon
    ImGui::SetCursorScreenPos(ImVec2(centerX - ImGui::CalcTextSize("WEAPON").x / 2, box_bottom_y + 30));
    ImGui::Text("WEAPON");

    ImGui::End();
}

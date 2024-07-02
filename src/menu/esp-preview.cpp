#include "esp-preview.h"
#include "editor.h"
#include "../../imgui/imgui.h"

void RenderESP()
{
    // Setup ImGui style and colors
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);  // Dark background
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);      // Light text color

    // ESP Preview window
    ImGui::SetNextWindowSize(ImVec2(300, 400)); // Adjusted window size
    ImGui::Begin("ESP Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Get the current window position and size for further calculations
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerX = windowPos.x + windowSize.x / 2 + offsetX;  // Center of the window with offset

    // Coordinates for the box
    float box_top_y = windowPos.y + windowSize.y / 2 - boxHeight / 2 + offsetY;
    float box_bottom_y = box_top_y + boxHeight;
    float box_left_x = centerX - (boxWidth / 2);
    float box_right_x = centerX + (boxWidth / 2);

    // Draw corner lines
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x + cornerLength, box_top_y), IM_COL32(255, 255, 255, 255), lineThickness); // Top left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - cornerLength, box_top_y), ImVec2(box_right_x, box_top_y), IM_COL32(255, 255, 255, 255), lineThickness); // Top right corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y), ImVec2(box_left_x + cornerLength, box_bottom_y), IM_COL32(255, 255, 255, 255), lineThickness); // Bottom left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - cornerLength, box_bottom_y), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), lineThickness); // Bottom right corner

    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x, box_top_y + cornerLength), IM_COL32(255, 255, 255, 255), lineThickness); // Top left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_top_y), ImVec2(box_right_x, box_top_y + cornerLength), IM_COL32(255, 255, 255, 255), lineThickness); // Top right vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y - cornerLength), ImVec2(box_left_x, box_bottom_y), IM_COL32(255, 255, 255, 255), lineThickness); // Bottom left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_bottom_y - cornerLength), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), lineThickness); // Bottom right vertical

    // Health bar
    float healthBarTopY = box_bottom_y + healthBarOffset; // Position below the box with offset
    ImVec2 healthBarTopLeft = ImVec2(box_left_x, healthBarTopY);
    ImVec2 healthBarBottomRight = ImVec2(box_left_x + (boxWidth * (healthPercent / 100.0f)), healthBarTopY + healthBarHeight);

    // Set color based on health percentage (green to red)
    ImU32 healthColor = IM_COL32(255 * (1.0f - healthPercent / 100.0f), 255 * (healthPercent / 100.0f), 0, 255);

    ImGui::GetWindowDrawList()->AddRectFilled(healthBarTopLeft, healthBarBottomRight, healthColor);

    // Nickname, Distance, Ping labels
    ImGui::GetWindowDrawList()->AddText(ImVec2(centerX - 20, box_top_y + nicknameOffsetY), IM_COL32(255, 255, 255, 255), "NICKNAME");
    ImGui::GetWindowDrawList()->AddText(ImVec2(box_left_x - 60, box_top_y + distanceOffsetY), IM_COL32(255, 255, 255, 255), "DISTANCE");
    ImGui::GetWindowDrawList()->AddText(ImVec2(box_right_x + 20, box_top_y + pingOffsetY), IM_COL32(255, 255, 255, 255), "PING");

    ImGui::End();
}

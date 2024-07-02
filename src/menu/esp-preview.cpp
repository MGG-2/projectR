#include "../../imgui/imgui.h"
#include "esp-preview.h"

void preview()
{
    // Setup ImGui style and colors
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

    // Setting up the window and layout with specific size
    ImGui::SetNextWindowSize(ImVec2(250, 400)); // Adjusted window size to be taller
    ImGui::Begin("ESP Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Get the current window size and position
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();

    // Calculate center of the window
    float centerX = windowPos.x + windowSize.x / 2.0f;
    float centerY = windowPos.y + windowSize.y / 2.0f;

    // Coordinates for the box (taller)
    float box_half_width = 50.0f;
    float box_half_height = 100.0f; // Adjusted height

    float box_top_y = centerY - box_half_height;
    float box_bottom_y = centerY + box_half_height;
    float box_left_x = centerX - box_half_width;
    float box_right_x = centerX + box_half_width;

    // Draw corner lines
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x + 15, box_top_y), IM_COL32(255, 255, 255, 255), 2.0f); // Top left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - 15, box_top_y), ImVec2(box_right_x, box_top_y), IM_COL32(255, 255, 255, 255), 2.0f); // Top right corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y), ImVec2(box_left_x + 15, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom left corner
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x - 15, box_bottom_y), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom right corner

    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_top_y), ImVec2(box_left_x, box_top_y + 15), IM_COL32(255, 255, 255, 255), 2.0f); // Top left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_top_y), ImVec2(box_right_x, box_top_y + 15), IM_COL32(255, 255, 255, 255), 2.0f); // Top right vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_left_x, box_bottom_y - 15), ImVec2(box_left_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom left vertical
    ImGui::GetWindowDrawList()->AddLine(ImVec2(box_right_x, box_bottom_y - 15), ImVec2(box_right_x, box_bottom_y), IM_COL32(255, 255, 255, 255), 2.0f); // Bottom right vertical

    // Adjust cursor for left and right columns
    float spacing = 20.0f;
    float column_start_y = box_top_y + 10;
    float left_column_x = box_left_x - 70; // Adjusted for alignment
    float right_column_x = box_right_x + 20; // Adjusted for alignment

    ImGui::End();
}
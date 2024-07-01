#include "esp.h"
#include "../../imgui/imgui.h"
#include <iostream>

// Default values for the box configuration
static float boxWidth = 100.0f;
static float boxHeight = 120.0f;
static float gap = 15.0f;
static float offsetX = 0.0f;
static float offsetY = 0.0f;
static float lineThickness = 2.0f;
static float cornerLength = 15.0f;

// Function to print current values to the console
void PrintCurrentValues()
{
    std::cout << "static float boxWidth = " << boxWidth << ";\n";
    std::cout << "static float boxHeight = " << boxHeight << ";\n";
    std::cout << "static float gap = " << gap << ";\n";
    std::cout << "static float offsetX = " << offsetX << ";\n";
    std::cout << "static float offsetY = " << offsetY << ";\n";
    std::cout << "static float lineThickness = " << lineThickness << ";\n";
    std::cout << "static float cornerLength = " << cornerLength << ";\n";
}

// Function to reset values to their defaults
void ResetValues()
{
    boxWidth = 100.0f;
    boxHeight = 120.0f;
    gap = 15.0f;
    offsetX = 0.0f;
    offsetY = 0.0f;
    lineThickness = 2.0f;
    cornerLength = 15.0f;
    PrintCurrentValues();
}

void RenderESP()
{
    // Setup ImGui style and colors
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);  // Dark background
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);      // Light text color

    // Settings window
    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Variables to track changes
    static float prevBoxWidth = boxWidth;
    static float prevBoxHeight = boxHeight;
    static float prevGap = gap;
    static float prevOffsetX = offsetX;
    static float prevOffsetY = offsetY;
    static float prevLineThickness = lineThickness;
    static float prevCornerLength = cornerLength;

    // Input fields to adjust the box parameters
    if (ImGui::SliderFloat("Box Width", &boxWidth, 50.0f, 200.0f) && boxWidth != prevBoxWidth)
    {
        prevBoxWidth = boxWidth;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Box Height", &boxHeight, 50.0f, 200.0f) && boxHeight != prevBoxHeight)
    {
        prevBoxHeight = boxHeight;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Gap", &gap, 5.0f, 30.0f) && gap != prevGap)
    {
        prevGap = gap;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Offset X", &offsetX, -100.0f, 100.0f) && offsetX != prevOffsetX)
    {
        prevOffsetX = offsetX;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Offset Y", &offsetY, -100.0f, 100.0f) && offsetY != prevOffsetY)
    {
        prevOffsetY = offsetY;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Line Thickness", &lineThickness, 1.0f, 5.0f) && lineThickness != prevLineThickness)
    {
        prevLineThickness = lineThickness;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Corner Length", &cornerLength, 5.0f, 30.0f) && cornerLength != prevCornerLength)
    {
        prevCornerLength = cornerLength;
        PrintCurrentValues();
    }

    // Reset button
    if (ImGui::Button("Reset"))
    {
        ResetValues();
    }

    ImGui::End();

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

    ImGui::End();
}

#include "editor.h"
#include "config.h"
#include "../../imgui/imgui.h"
#include <iostream>

// Default values for the box configuration
float boxWidth = 100.0f;
float boxHeight = 120.0f;
float gap = 15.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;
float lineThickness = 2.0f;
float cornerLength = 15.0f;

// Default values for the health bar configuration
float healthPercent = 100.0f; // Health percentage, initially set to 100%
float healthBarHeight = 5.0f;
float healthBarOffset = 5.0f; // Offset from the bottom of the box

// Default values for labels
float nicknameOffsetY = -20.0f;
float distanceOffsetY = 30.0f;
float pingOffsetY = 30.0f;

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
    std::cout << "static float healthPercent = " << healthPercent << ";\n";
    std::cout << "static float healthBarHeight = " << healthBarHeight << ";\n";
    std::cout << "static float healthBarOffset = " << healthBarOffset << ";\n";
    std::cout << "static float nicknameOffsetY = " << nicknameOffsetY << ";\n";
    std::cout << "static float distanceOffsetY = " << distanceOffsetY << ";\n";
    std::cout << "static float pingOffsetY = " << pingOffsetY << ";\n";
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
    healthPercent = 100.0f;
    healthBarHeight = 5.0f;
    healthBarOffset = 5.0f;
    nicknameOffsetY = -20.0f;
    distanceOffsetY = 30.0f;
    pingOffsetY = 30.0f;
    PrintCurrentValues();
}

void RenderSettings()
{
    // Settings window
    ImGui::SetNextWindowSize(ImVec2(300, 600));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Box Settings Frame
    ImGui::Text("Box Settings");
    ImGui::Separator();
    ImGui::BeginChild("BoxSettingsFrame", ImVec2(280, 220), true);

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

    ImGui::EndChild();

    // Health Settings Frame
    ImGui::Text("Health Settings");
    ImGui::Separator();
    ImGui::BeginChild("HealthSettingsFrame", ImVec2(280, 120), true);

    // Variables to track changes
    static float prevHealthPercent = healthPercent;
    static float prevHealthBarHeight = healthBarHeight;
    static float prevHealthBarOffset = healthBarOffset;

    // Input fields to adjust the health parameters
    if (ImGui::SliderFloat("Health %", &healthPercent, 0.0f, 100.0f) && healthPercent != prevHealthPercent)
    {
        prevHealthPercent = healthPercent;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Health Bar Height", &healthBarHeight, 1.0f, 20.0f) && healthBarHeight != prevHealthBarHeight)
    {
        prevHealthBarHeight = healthBarHeight;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Health Bar Offset", &healthBarOffset, 0.0f, 20.0f) && healthBarOffset != prevHealthBarOffset)
    {
        prevHealthBarOffset = healthBarOffset;
        PrintCurrentValues();
    }

    ImGui::EndChild();

    // Label Settings Frame
    ImGui::Text("Label Settings");
    ImGui::Separator();
    ImGui::BeginChild("LabelSettingsFrame", ImVec2(280, 150), true);

    // Variables to track changes
    static float prevNicknameOffsetY = nicknameOffsetY;
    static float prevDistanceOffsetY = distanceOffsetY;
    static float prevPingOffsetY = pingOffsetY;

    // Input fields to adjust the label parameters
    if (ImGui::SliderFloat("Nickname Offset Y", &nicknameOffsetY, -50.0f, 50.0f) && nicknameOffsetY != prevNicknameOffsetY)
    {
        prevNicknameOffsetY = nicknameOffsetY;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Distance Offset Y", &distanceOffsetY, 0.0f, 100.0f) && distanceOffsetY != prevDistanceOffsetY)
    {
        prevDistanceOffsetY = distanceOffsetY;
        PrintCurrentValues();
    }
    if (ImGui::SliderFloat("Ping Offset Y", &pingOffsetY, 0.0f, 100.0f) && pingOffsetY != prevPingOffsetY)
    {
        prevPingOffsetY = pingOffsetY;
        PrintCurrentValues();
    }

    ImGui::EndChild();

    // Save and Load buttons
    if (ImGui::Button("Save Config"))
    {
        SaveConfig();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Config"))
    {
        LoadConfig();
    }

    // Reset button
    if (ImGui::Button("Reset"))
    {
        ResetValues();
    }

    ImGui::End();
}

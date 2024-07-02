#include "../../imgui/imgui.h"
#include "esp-preview.h"
#include "menu.h"
#include <map>
#include <string>

// Define a structure to hold the position state of each draggable item
struct DragDropItem {
    std::string id;
    ImVec2 position;
    bool isDefault;
};

std::map<std::string, DragDropItem> itemPositions;

// Offset management for each column
struct ColumnOffset {
    float offset;
};

std::map<std::string, ColumnOffset> columnOffsets;

void configureStyle() {
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
}

void espBox(ImDrawList* drawList, float left, float right, float top, float bottom) {
    ImU32 color = IM_COL32(255, 255, 255, 255);
    float lineLength = 15.0f;
    float lineWidth = 2.0f;

    // Top corners
    drawList->AddLine(ImVec2(left, top), ImVec2(left + lineLength, top), color, lineWidth);
    drawList->AddLine(ImVec2(right - lineLength, top), ImVec2(right, top), color, lineWidth);
    drawList->AddLine(ImVec2(left, top), ImVec2(left, top + lineLength), color, lineWidth);
    drawList->AddLine(ImVec2(right, top), ImVec2(right, top + lineLength), color, lineWidth);

    // Bottom corners
    drawList->AddLine(ImVec2(left, bottom), ImVec2(left + lineLength, bottom), color, lineWidth);
    drawList->AddLine(ImVec2(right - lineLength, bottom), ImVec2(right, bottom), color, lineWidth);
    drawList->AddLine(ImVec2(left, bottom - lineLength), ImVec2(left, bottom), color, lineWidth);
    drawList->AddLine(ImVec2(right, bottom - lineLength), ImVec2(right, bottom), color, lineWidth);
}

void drawESPText(ImDrawList* drawList, const char* text, ImVec2 position) {
    drawList->AddText(position, IM_COL32(255, 255, 255, 255), text);
}

void drawHealthBar(ImDrawList* drawList, float centerX, float bottomY, float width, float health) {
    ImVec4 healthColor = ImVec4(1.0f - health / 100.0f, health / 100.0f, 0.0f, 1.0f);
    float healthBarWidth = width * (health / 100.0f);
    float healthBarStartX = centerX - (healthBarWidth / 2.0f); // Center the health bar
    drawList->AddRectFilled(ImVec2(healthBarStartX, bottomY), ImVec2(healthBarStartX + healthBarWidth, bottomY + 5), ImGui::ColorConvertFloat4ToU32(healthColor));
}

void preview() {
    configureStyle();

    ImGui::SetNextWindowSize(ImVec2(250, 400));
    ImGui::Begin("ESP Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    float centerX = windowPos.x + windowSize.x / 2.0f;
    float centerY = windowPos.y + windowSize.y / 2.0f;

    float box_half_width = 50.0f;
    float box_half_height = 100.0f;

    float box_left_x = centerX - box_half_width;
    float box_right_x = centerX + box_half_width;
    float box_top_y = centerY - box_half_height;
    float box_bottom_y = centerY + box_half_height;

    if (enableBoxESP) {
        espBox(drawList, box_left_x, box_right_x, box_top_y, box_bottom_y);
    }

    float columnLeftX = box_left_x - 50;
    float columnRightX = box_right_x + 10;
    float columnTopY = box_top_y - 20;
    float columnBottomY = box_bottom_y + 15;
    float elementSpacing = 20;

    // Initialize default positions for items and column offsets
    if (itemPositions.empty()) {
        itemPositions["DND_NAME"] = { "DND_NAME", ImVec2(centerX - 20, columnTopY), true };
        itemPositions["DND_DISTANCE"] = { "DND_DISTANCE", ImVec2(columnLeftX, box_top_y + 10), true };
        itemPositions["DND_PING"] = { "DND_PING", ImVec2(columnLeftX, box_top_y + 10 + elementSpacing), true };
        itemPositions["DND_HEALTH"] = { "DND_HEALTH", ImVec2(centerX - (box_right_x - box_left_x) / 2.0f, columnBottomY), true };
    }

    if (columnOffsets.empty()) {
        columnOffsets["left_col"] = { 0.0f };
        columnOffsets["right_col"] = { 0.0f };
        columnOffsets["top_col"] = { 0.0f };
        columnOffsets["bottom_col"] = { 0.0f };
    }

    // Display and handle dragging for each item
    for (auto& item : itemPositions) {
        ImGui::SetCursorPos(ImVec2(item.second.position.x - windowPos.x, item.second.position.y - windowPos.y));
        if (item.first == "DND_NAME") {
            ImGui::Text("Player");
        }
        else if (item.first == "DND_DISTANCE") {
            ImGui::Text("100m");
        }
        else if (item.first == "DND_PING") {
            ImGui::Text("Ping");
        }
        else if (item.first == "DND_HEALTH") {
            float health = 75.0f; // Example health value
            ImGui::ProgressBar(health / 100.0f, ImVec2(box_right_x - box_left_x, 5), "");
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload(item.second.id.c_str(), item.second.id.c_str(), item.second.id.size() + 1);
            ImGui::Text("%s", item.second.id.c_str());
            ImGui::EndDragDropSource();
        }
    }

    // Create drop targets for each column
    auto processDropTarget = [&](const char* columnId, ImVec2 dropPos, float& columnOffset, ImVec2 size) {
        ImGui::InvisibleButton(columnId, size);
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_NAME")) {
                itemPositions["DND_NAME"].position = ImVec2(dropPos.x, dropPos.y + columnOffset);
                itemPositions["DND_NAME"].isDefault = false;
                columnOffset += elementSpacing;
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DISTANCE")) {
                itemPositions["DND_DISTANCE"].position = ImVec2(dropPos.x, dropPos.y + columnOffset);
                itemPositions["DND_DISTANCE"].isDefault = false;
                columnOffset += elementSpacing;
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PING")) {
                itemPositions["DND_PING"].position = ImVec2(dropPos.x, dropPos.y + columnOffset);
                itemPositions["DND_PING"].isDefault = false;
                columnOffset += elementSpacing;
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HEALTH")) {
                itemPositions["DND_HEALTH"].position = ImVec2(dropPos.x, dropPos.y + columnOffset);
                itemPositions["DND_HEALTH"].isDefault = false;
                columnOffset += elementSpacing;
            }
            ImGui::EndDragDropTarget();
        }
        };

    // Adjust the drop target positions and sizes
    ImGui::SetCursorPos(ImVec2(columnLeftX - windowPos.x, box_top_y - windowPos.y));
    processDropTarget("left_col", ImVec2(columnLeftX, box_top_y), columnOffsets["left_col"].offset, ImVec2(40, box_bottom_y - box_top_y));

    ImGui::SetCursorPos(ImVec2(columnRightX - windowPos.x, box_top_y - windowPos.y));
    processDropTarget("right_col", ImVec2(columnRightX, box_top_y), columnOffsets["right_col"].offset, ImVec2(40, box_bottom_y - box_top_y));

    // Move the top column to the top of the ESP box and adjust its width
    ImGui::SetCursorPos(ImVec2(box_left_x - windowPos.x, box_top_y - 30 - windowPos.y));
    processDropTarget("top_col", ImVec2(box_left_x, box_top_y - 30), columnOffsets["top_col"].offset, ImVec2(box_right_x - box_left_x, 30));

    ImGui::SetCursorPos(ImVec2(box_left_x - windowPos.x, columnBottomY - windowPos.y));
    processDropTarget("bottom_col", ImVec2(box_left_x, columnBottomY), columnOffsets["bottom_col"].offset, ImVec2(box_right_x - box_left_x, 30));



    ImGui::End();
}

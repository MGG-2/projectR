#include "../../imgui/imgui.h"
#include "esp-preview.h"
#include "menu.h"

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
    ImGui::Begin("ESP Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

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

    if (enableNameESP) {
        ImGui::SetCursorPos(ImVec2(centerX - 20 - windowPos.x, columnTopY - windowPos.y));
        ImGui::Text("Player");
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            const char* payload = "Player";
            ImGui::SetDragDropPayload("DND_NAME", payload, strlen(payload) + 1);
            ImGui::Text("Player");
            ImGui::EndDragDropSource();
        }
    }

    float leftYOffset = box_top_y + 10;
    if (enableDistanceESP) {
        ImGui::SetCursorPos(ImVec2(columnLeftX - windowPos.x, leftYOffset - windowPos.y));
        ImGui::Text("100m");
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            const char* payload = "100m";
            ImGui::SetDragDropPayload("DND_DISTANCE", payload, strlen(payload) + 1);
            ImGui::Text("100m");
            ImGui::EndDragDropSource();
        }
        leftYOffset += elementSpacing;
    }
    if (enablePingESP) {
        ImGui::SetCursorPos(ImVec2(columnLeftX - windowPos.x, leftYOffset - windowPos.y));
        ImGui::Text("Ping");
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            const char* payload = "Ping";
            ImGui::SetDragDropPayload("DND_PING", payload, strlen(payload) + 1);
            ImGui::Text("Ping");
            ImGui::EndDragDropSource();
        }
        leftYOffset += elementSpacing;
    }

    if (enableHealthESP) {
        float health = 75.0f; // Example health value
        ImGui::SetCursorPos(ImVec2(centerX - (box_right_x - box_left_x) / 2.0f - windowPos.x, columnBottomY - windowPos.y));
        ImGui::ProgressBar(health / 100.0f, ImVec2(box_right_x - box_left_x, 5), "");
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("DND_HEALTH", &health, sizeof(health));
            ImGui::ProgressBar(health / 100.0f, ImVec2(box_right_x - box_left_x, 5), "");
            ImGui::EndDragDropSource();
        }
    }

    // Create drop targets for each column
    ImGui::SetCursorPos(ImVec2(columnLeftX - windowPos.x, box_top_y - windowPos.y));
    ImGui::InvisibleButton("left_col", ImVec2(40, windowSize.y));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DISTANCE")) {
            IM_ASSERT(payload->DataSize == strlen("100m") + 1);
            const char* payload_data = (const char*)payload->Data;
            // Process the payload data
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PING")) {
            IM_ASSERT(payload->DataSize == strlen("Ping") + 1);
            const char* payload_data = (const char*)payload->Data;
            // Process the payload data
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SetCursorPos(ImVec2(columnRightX - windowPos.x, box_top_y - windowPos.y));
    ImGui::InvisibleButton("right_col", ImVec2(40, windowSize.y));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_OTHER")) {
            IM_ASSERT(payload->DataSize == strlen("Other") + 1);
            const char* payload_data = (const char*)payload->Data;
            // Process the payload data
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SetCursorPos(ImVec2(box_left_x - windowPos.x, columnTopY - windowPos.y));
    ImGui::InvisibleButton("top_col", ImVec2(box_right_x - box_left_x, 30));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_NAME")) {
            IM_ASSERT(payload->DataSize == strlen("Player") + 1);
            const char* payload_data = (const char*)payload->Data;
            // Process the payload data
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SetCursorPos(ImVec2(box_left_x - windowPos.x, columnBottomY - windowPos.y));
    ImGui::InvisibleButton("bottom_col", ImVec2(box_right_x - box_left_x, 30));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HEALTH")) {
            IM_ASSERT(payload->DataSize == sizeof(float));
            const float* payload_data = (const float*)payload->Data;
            // Process the payload data
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndGroup();


    ImGui::End();
}

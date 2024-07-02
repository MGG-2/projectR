#pragma once
#ifndef MENU_H
#define MENU_H

#include "../../imgui/imgui.h"

void SetupImGuiStyle();
void RenderMenu();
void RenderCustomTitleBar(const char* title, ImVec2 windowSize);

extern bool showESPPreview;
extern bool enableBoxESP;
extern bool enableHealthESP;
extern bool enableNameESP;
extern bool enableDistanceESP;
extern float espDistance;
extern bool enablePingESP;


#endif // MENU_H

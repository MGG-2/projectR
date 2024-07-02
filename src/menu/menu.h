#pragma once
#ifndef MENU_H
#define MENU_H

#include "../../imgui/imgui.h"

void SetupImGuiStyle();
void RenderMenu();
void RenderCustomTitleBar(const char* title, ImVec2 windowSize);


#endif // MENU_H
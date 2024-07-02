#ifndef ESP_PREVIEW_H
#define ESP_PREVIEW_H

void preview();
void configureStyle();
void espBox(ImDrawList* drawList, float left, float right, float top, float bottom);
void drawESPText(ImDrawList* drawList, const char* text, ImVec2 position);
void drawHealthBar(ImDrawList* drawList, float centerX, float bottomY, float width, float health);
void updateColumnPositions(const char* draggedItemId, ImVec2 dropPos);

#endif // ESP_PREVIEW_H

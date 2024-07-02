#ifndef EDITOR_H
#define EDITOR_H

void RenderSettings();

// Extern declarations for variables to be used in esp-preview.cpp
extern float boxWidth;
extern float boxHeight;
extern float gap;
extern float offsetX;
extern float offsetY;
extern float lineThickness;
extern float cornerLength;
extern float healthPercent;
extern float healthBarHeight;
extern float healthBarOffset;

extern float nicknamePosX, nicknamePosY;
extern float distancePosX, distancePosY;
extern float pingPosX, pingPosY;
extern float healthBarPosX, healthBarPosY;

#endif // EDITOR_H

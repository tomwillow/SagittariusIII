#pragma once
#include <vector>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define ApplyEditBackground() glColor4f(0.0f, 0.5f, 0.8f, 0.5f)
#define ApplyEditBorder() glColor4f(1.0f, 1.0f, 1.0f, 0.5f)
#define ApplyButtonBackground() glColor4ub(83, 117, 238, 255)
#define ApplyButtonDisable() glColor4f(0.5f,0.5f,0.5f,1.0f)
#define ApplyButtonHover() glColor4ub(83, 117, 238, 128)


#define COLOR_WHITE	0
#define COLOR_KYON	1
#define COLOR_HARUHI	2
#define COLOR_KOIZUMI	3
#define COLOR_MIKURU	4
#define COLOR_YUKI	5
#define COLOR_DIE	6
#define COLOR_LUP	7
#define COLOR_IRA	8
#define COLOR_MUS	9
#define COLOR_EQU	10

extern const std::vector<glm::vec4> default_colors;
void SetColor(int index);
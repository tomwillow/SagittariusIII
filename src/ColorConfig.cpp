#include "ColorConfig.h"

#include <Windows.h>
#include <gl\GL.h>

const std::vector<glm::vec4> default_colors =
{
	glm::vec4(1.0f, 1.0f, 1.0f, 1),//white
	glm::vec4(0.176f, 0.341f, 0.984f, 1),//blue
	glm::vec4(1.0f, 0.027f, 0.039f, 1),//red
	glm::vec4(0.137f, 0.608f, 0.353f, 1),//green
	glm::vec4(0.984f, 0.282f, 0.6f, 1),//pink
	glm::vec4(0.765f, 0.631f, 0.808f, 1),//
	glm::vec4(0.016f, 0.067f, 0.0f, 1),//black
	glm::vec4(0.596f, 0.333f, 0.545f, 1), //red
	glm::vec4(0.816f, 0.824f, 0.475f, 1),//yellow
	glm::vec4(0.376f, 0.533f, 0.624f, 1),//blue
	glm::vec4(0.627f, 0.408f, 0.573f, 1) //purple
};

void SetColor(int index)
{
	glColor4f(default_colors[index].r, default_colors[index].g, default_colors[index].b, default_colors[index].a);
}

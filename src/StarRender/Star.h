#pragma once

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Star
{
private:
public:
	glm::vec3 p0;
	glm::vec3 p;
	float vz;
	float t0;
	enum StarType {ST_POINT,ST_TEXTURE};
	StarType type;
	unsigned int texture_id;
	glm::vec4 color;

	Star(glm::vec3 p0, float vz, float t0, StarType type = ST_POINT, unsigned int texture_id = 0, glm::vec4 color = { 1,1,1,1 }) :
		p0(p0),p(p0), vz(vz), t0(t0),type(type),texture_id(texture_id),color(color) {}
	void RefreshPos(float t)
	{
		float dt = t - t0;
		if (dt < 1.0f)
		{
			color.a = dt / 1.0f;
		}
		p.z = p0.z + dt * vz;
	}

};
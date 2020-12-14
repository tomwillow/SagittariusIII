#pragma once
#include <Windows.h>
#include <gl\GL.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <random>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define ZeroOne2Clip(a) ((a)*2.0f-1.0f)

inline float GetRandomColor()
{
	return (float)(rand() % 256) / 256.0f;
}

extern std::default_random_engine e;

inline float RandFloat(float start = -1.0f, float end = 1.0f)
{
	std::uniform_real_distribution<float> dist(start,end);
	return dist(e);
};

inline int RandInt(int start = 0, int end = 1)
{
	std::uniform_int_distribution<int> dist(start, end);
	return dist(e);
}

inline void DrawCircle(float x, float y, float r)
{
	const int n = 32;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i = 0; i < n+1; ++i)
	{
		float theta = 2.0f * (float)M_PI / (float)n*(float)i;
		glColor3f(GetRandomColor(), GetRandomColor(), GetRandomColor());
		glVertex2f(x+r * cos(theta), y+r * sin(theta));
	}
	glEnd();
}


inline void MyPerspective(GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar)
{
	// 使用glu库函数，需要添加glu.h头文件
	//gluPerspective( fov, aspectRatio, zNear, zFar );


	// 使用OpenGL函数，但是需要添加math.h头文件
	GLdouble rFov = fov * M_PI / 180.0;
	glFrustum(-zNear * tan(rFov / 2.0) * aspectRatio,
		zNear * tan(rFov / 2.0) * aspectRatio,
		-zNear * tan(rFov / 2.0),
		zNear * tan(rFov / 2.0),
		zNear, zFar);
}


auto WorldToClip = [](glm::vec3 p, glm::mat4& projection)->glm::vec2
{
	p = projection * glm::vec4(p, 1.0f);
	return { p.x / p.z,p.y / p.z };
};

auto ClipToWorld = [](glm::vec2 clip, float world_z, glm::mat4& projection)->glm::vec3
{
	glm::vec4 pos(clip * world_z, -world_z, -world_z);//
	glm::mat4 invM = glm::inverse(projection);
	return invM * pos;
};

inline void EnableTexture()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline void EnableGeometry()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

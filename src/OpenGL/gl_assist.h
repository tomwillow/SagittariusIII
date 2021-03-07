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

inline void DrawRect(GLenum mode, glm::vec4 v)
{
	glBegin(mode);
	glVertex2f(v[0], v[1]);
	glVertex2f(v[2], v[1]);
	glVertex2f(v[2], v[3]);
	glVertex2f(v[0], v[3]);
	glEnd();
}

inline void DrawRect(GLenum mode,float x1, float y1, float x2, float y2)
{
	glBegin(mode);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
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

inline void StartDrawMaskLayer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

inline void StartDrawMaskedLayer()
{
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

inline void EndMask()
{
	glDisable(GL_STENCIL_TEST);
}

inline void StartDrawMaskLayer2()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);//GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
}

inline void StartDrawMaskedLayer2()
{
	glBlendFunc(GL_DST_ALPHA, GL_ZERO);//GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA
}

inline void EndMask2()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
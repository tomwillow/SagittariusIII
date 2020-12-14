#pragma once
#include <Windows.h>
#include <gl\GL.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "tstring.h"

#include <stdexcept>

struct Character {
	GLuint     TextureID;  // 字形纹理ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 字形基于基线和起点的位置
	GLuint     Advance;    // 起点到下一个字形起点的距离，单位1/64像素
};

//DrawByPixel 渲染方法
//glMatrixMode(GL_PROJECTION);
//glLoadIdentity();
//glOrtho(0, w, 0, h, -1.0, 100);

//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();

//EnableTextureAndBlend();

//glColor4f(0, 1, 1, 0.5);
//text->DrawByPixel(10, 10);

class TFreeType
{
protected:
	float pixel_size;
	std::vector<Character> Characters;
public:
	//class no_char_error :public std::runtime_error
	//{
	//public:
	//	no_char_error(std::string s) :std::runtime_error(s) {}
	//};

	TFreeType(std::tstring s,std::string font_name,int pixel_size=48);

	//按照像素位置显示文字，x,y以左下角为原点
	void DrawByPixel(GLfloat x, GLfloat y, GLfloat xscale=1.0f, GLfloat yscale=1.0f);

};


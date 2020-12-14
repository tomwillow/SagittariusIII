#pragma once
#include <Windows.h>
#include <gl\GL.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "tstring.h"

#include <stdexcept>

struct Character {
	GLuint     TextureID;  // ��������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // ���λ��ڻ��ߺ�����λ��
	GLuint     Advance;    // ��㵽��һ���������ľ��룬��λ1/64����
};

//DrawByPixel ��Ⱦ����
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

	//��������λ����ʾ���֣�x,y�����½�Ϊԭ��
	void DrawByPixel(GLfloat x, GLfloat y, GLfloat xscale=1.0f, GLfloat yscale=1.0f);

};


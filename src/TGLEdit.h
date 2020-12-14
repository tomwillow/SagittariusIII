#pragma once

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include "TBoxFreeType.h"

#include "ExternResource.h"

class TGLEdit
{
private:
	std::tstring s;
	int W, H;
	float x1, y1, x2, y2;
	bool is_hover;
	glm::vec4 border_color, color, hover_color;
	std::unique_ptr<TBoxFreeType> text;
public:
	TGLEdit(std::tstring s, std::string font_name, int pixel_size = 48);
	void DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2);
	//virtual void OnMouseMove(WPARAM mk_code, int x, int y);
	//virtual int OnLButtonDown(WPARAM mk_code, int x, int y);
	virtual void OnChar(TCHAR tc, LPARAM lParam);
};


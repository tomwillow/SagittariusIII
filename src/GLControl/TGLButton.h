#pragma once

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include "TBoxFreeType.h"

class TGLButton
{
private:
	int W, H;
	float x1, y1, x2, y2;
	bool is_hover;
	glm::vec4 border_color, color, hover_color;
protected:
	bool enable;
	const std::string font_name;
	const int pixel_size;
	std::unique_ptr<TBoxFreeType> text;
public:
	TGLButton(std::tstring s, std::string font_name,int pixel_size = 48);
	virtual void DrawByClipCoord(int w,int h,float x1, float y1, float x2, float y2);
	virtual void OnMouseMove(WPARAM mk_code, int x, int y);
	virtual int OnLButtonDown(WPARAM mk_code, int x, int y);
	virtual int OnRButtonDown(WPARAM mk_code, int x, int y);
	void SetFontSizeScale(float i = 1.0f);
	float GetClipCoordHeight(int H);

	void SetEnable(bool enable = true);
};


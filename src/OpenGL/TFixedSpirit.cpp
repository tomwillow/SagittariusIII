#include "TFixedSpirit.h"

#include <Windows.h>
#include <gl/GL.h>

TFixedSpirit::TFixedSpirit(float x_per, float y_per, float len_per, bool base_is_width):
	x_per(x_per), y_per(y_per), len_per(len_per), base_is_width(base_is_width)
{
}

void TFixedSpirit::BindAndDraw_inner(float x1, float y1, float x2, float y2, int W, int H)
{
	Bind();

	/** 开始绘制四边形 */
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(x1, y1);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x2, y1);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x2, y2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x1, y2);
	glEnd();
}

void TFixedSpirit::Calc(float& x1, float& y1, float& x2, float& y2, int W, int H)
{
	float w_per, h_per;
	if (base_is_width)
	{
		w_per = len_per;
		h_per = w_per * height / width;
		x1 = 2.0f * x_per - w_per - 1.0f;
		x2 = x1 + w_per * 2.0f;
		y1 = (2.0f * y_per - h_per * (float)W / (float)H - 1.0f);
		y2 = (y1 + h_per * 2.0f * (float)W / (float)H);
	}
	else
	{
		h_per = len_per;
		w_per = h_per * width / height;
		y1 = 2.0f * y_per - h_per - 1.0f;
		y2 = y1 + h_per * 2.0f;
		x1 = 2.0f * x_per - w_per * (float)H / (float)W - 1.0f;
		x2 = x1 + w_per * 2.0f * (float)H / (float)W;
	}
}

TFixedSpirit::TFixedSpirit(std::string filename, float x_per, float y_per, float len_per, bool base_is_width)
{
	this->TFixedSpirit::TFixedSpirit(x_per,y_per,len_per,base_is_width);
	this->TTexture::TTexture(filename);
}

void TFixedSpirit::Draw(int W, int H)
{
	float x1, x2, y1, y2;
	Calc(x1, y1, x2, y2, W, H);

	BindAndDraw_inner(x1, y1, x2, y2, W, H);
}

bool TFixedSpirit::PosInTexture(int x, int y, int W, int H)
{
	float xpos = (float)x / (float)W;
	xpos = xpos * 2.0f - 1.0f;
	float ypos = ((float)H-(float)y) / (float)H;
	ypos = ypos * 2.0f - 1.0f;

	float x1, x2, y1, y2;
	Calc(x1, y1, x2, y2, W, H);

	return xpos >= x1 && xpos <= x2 && ypos >= y1 && ypos <= y2;
}
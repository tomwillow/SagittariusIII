#include "TGLButton.h"

#include <Windows.h>
#include <gl\GL.h>
#include "gl_assist.h"

using namespace std;

TGLButton::TGLButton(std::tstring s, std::string font_name, int pixel_size):
	text(make_unique<TBoxFreeType>(s,font_name,pixel_size)),is_hover(false)
{
}

void TGLButton::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	W = w, H = h;
	this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2;
	//x1 -= 0.1; x2 += 0.1;
	//y1 -= 0.1; y2 += 0.1;


	//btn background
	EnableGeometry();

	if (is_hover)
		glColor4ub(83, 117, 238, 128);
	else
		glColor4ub(83, 117, 238, 255);	

	/** 开始绘制四边形 */
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(x1, y1);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x2, y1);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x2, y2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x1, y2);
	glEnd();

	//button text
	EnableTexture();

	glColor4f(1, 1, 1, 1);
	text->DrawCenterByClipCoord(w, h, x1,y1,x2,y2);
}

void TGLButton::OnMouseMove(WPARAM mk_code, int x, int y)
{
	float x_clip = ZeroOne2Clip((float)x / (float)W);
	float y_clip = ZeroOne2Clip((float)(H-y) / (float)H);
	is_hover = (x_clip >= x1 && x_clip <= x2 && y_clip >= y1 && y_clip <= y2);
	//printf("x: %f >= %f && %f <= %f\n", x_clip, x1, x_clip, x2);
	//printf("%f %f\n", x_clip, y_clip);
	//printf("%d\n", (x_clip >= x1 && x_clip <= x2));
}

int TGLButton::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	return is_hover;
}

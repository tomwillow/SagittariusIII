#include "TGLColorCombo.h"

#include "gl_assist.h"

#include "ColorConfig.h"

void TGLColorCombo::DrawColor(int index, float x1, float y1, float x2, float y2)
{
	glm::vec4& color = colors[index];
	glColor4f(color.r, color.g, color.b, color.a);
	DrawRect(GL_QUADS, x1, y1, x2, y2);
}

int TGLColorCombo::GetHover(int x, int y)
{
	float x_clip = ZeroOne2Clip((float)x / (float)W);
	float y_clip = ZeroOne2Clip((float)(H - y) / (float)H);

	if (x_clip >= x1 && x_clip <= x2 && y_clip >= y1 && y_clip <= y2)
	{
		return -1;
	}

	if (spread)
	{
		float color_y2 = y1;
		for (int i = 0; i < colors.size(); ++i)
		{
			float color_y1 = color_y2 - item_height;
			if (x_clip >= x1 && x_clip <= x2 && y_clip >= color_y1 && y_clip <= color_y2)
			{
				return i;
			}
			color_y2 = color_y1;
		}
	}

	return -2;
}

TGLColorCombo::TGLColorCombo() :
	colors(
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
		}
	),
	spread(false),
	cur(0),
	hover(-2)
{
}

void TGLColorCombo::OnMouseMove(WPARAM mk_code, int x, int y)
{
	hover = GetHover(x, y);
	return;
}

int TGLColorCombo::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	int expected_hover = GetHover(x, y);
	if (expected_hover == -2)
	{
		spread = false;
	}

	if (expected_hover == -1)
	{
		spread = !spread;
	}
	if (expected_hover >= 0)
	{
		cur = expected_hover;
		spread = false;
	}

	hover = expected_hover;
	return hover != -2;
}

int TGLColorCombo::OnRButtonDown(WPARAM mk_code, int x, int y)
{
	return 0;
}

void TGLColorCombo::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	auto ToPixel = [](float W, float width)->float
	{
		return width * W / 2.0f;
	};
	auto ToClip = [](float W, float pixel)->float
	{
		return pixel / W * 2.0f;
	};

	W = w; H = h;
	this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2;
	item_height = y2 - y1;

	float item_height_pixel = ToPixel(h, item_height);
	float width = x2 - x1;

	EnableGeometry();

	//background
	if (hover == -1)//hover
		ApplyButtonHover();
	else
		ApplyButtonBackground();
	DrawRect(GL_QUADS, x1, y1, x2, y2);

	float rect_left = x1 + item_height;

	//triangle
	float tri_width = ToClip(w, item_height_pixel * 0.6f);
	float y_margin = ToClip(h, item_height_pixel * 0.2f);
	float x_margin = ToClip(w, item_height_pixel * 0.2f);

	float tri_top = y2 - y_margin;
	float tri_bottom = y1 + y_margin;
	float tri[][2] = {
		x1 + x_margin,tri_top,
		x1 + x_margin + tri_width,tri_top,
		x1 + x_margin + tri_width * 0.5f,tri_bottom
	};

	glColor4f(1, 1, 1, 1);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3; ++i)
		glVertex2f(tri[i][0], tri[i][1]);
	glEnd();

	//
	float color_x1 = x1 + x_margin + tri_width + x_margin;
	float color_x2 = x2 - x_margin;
	float color_y1 = y1 + y_margin;
	float color_y2 = y2 - y_margin;

	DrawColor(cur, color_x1, color_y1, color_x2, color_y2);

	//spread
	if (spread)
	{
		float top = y1;
		float bottom = y1 - item_height * colors.size();
		ApplyButtonBackground();
		DrawRect(GL_QUADS, x1, bottom, x2, top);

		//color
		float color_y2 = y1;
		for (int i = 0; i < colors.size(); ++i)
		{
			float color_y1 = color_y2 - item_height;
			DrawColor(i, x1 + x_margin, color_y1 + y_margin, x2 - x_margin, color_y2 - y_margin);

			if (hover == i)
			{
				glColor4f(1, 1, 1, 1);
				DrawRect(GL_LINE_LOOP, x1 + x_margin, color_y1 + y_margin, x2 - x_margin, color_y2 - y_margin);
			}

			color_y2 = color_y1;
		}
	}
}

glm::vec4 TGLColorCombo::GetColor()
{
	return colors[cur];
}

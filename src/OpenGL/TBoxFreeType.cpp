#include "TBoxFreeType.h"

#include <algorithm>

using namespace std;

TBoxFreeType::TBoxFreeType(std::tstring s, std::string font_name, int pixel_size) :
	TFreeType(s, font_name, pixel_size)
{
}

void TBoxFreeType::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	float x_size = (float)pixel_size / w * 2.0f, y_size = (float)pixel_size / h * 2.0f;
	float x = x1, y = y2 - y_size;

	float xscale = 2.0f / w, yscale = 2.0f / h;

	// 对文本中的所有字符迭代
	for (auto& ch : Characters)
	{
		GLfloat xpos = x + ch.Bearing.x * xscale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * yscale;

		GLfloat w = ch.Size.x * xscale;
		GLfloat h = ch.Size.y * yscale;

		// 当前字符的VBO
		GLfloat vertices[4][2] = {
			{ xpos,     ypos + h },//,   0.0, 0.0
			{ xpos,     ypos},//,       0.0, 1.0 
			{ xpos + w, ypos},//,       1.0, 1.0 
			{ xpos + w, ypos + h}//,   1.0, 0.0 
		};
		// 在方块上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);


		/** 开始绘制四边形 */
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(vertices[0][0], vertices[0][1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(vertices[3][0], vertices[3][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(vertices[2][0], vertices[2][1]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(vertices[1][0], vertices[1][1]);
		glEnd();

		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * xscale; //(2^6 = 64)

		if (x + x_size > x2)
		{
			x = x1;
			y -= y_size;
		}
		if (y < y1)
			break;
	}
}

void TBoxFreeType::DrawCenterByClipCoord(int w, int h, float cx, float cy)
{
	int width = 0, height = 0;
	int center_line_y = 0;
	{
		int height_max = INT_MIN;
		int xmin = INT_MAX, xmax = INT_MIN;
		int ymin = INT_MAX, ymax = INT_MIN;

		xmin = min(xmin, Characters[0].Bearing.x);

		int x = 0, y = 0;

		// 对文本中的所有字符迭代
		for (auto& ch : Characters)
		{
			int xpos = x + ch.Bearing.x;
			int ypos = y - (ch.Size.y - ch.Bearing.y);

			int w = ch.Size.x;
			int h = ch.Size.y;

			xmax = max(xmax, xpos + w);
			ymin = min(ymin, ypos);
			ymax = max(ymax, ypos + h);

			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += ch.Advance >> 6; //(2^6 = 64)

			if (h > height_max)
			{
				height_max = h;
				center_line_y = (ymax + ymin) / 2;
			}
		}
		width = xmax - xmin;
		height = ymax - ymin;
	}
	float clip_height = (float)height / (float)h * 2.0f;

	//
	float x_size = (float)pixel_size / w * 2.0f, y_size = (float)pixel_size / h * 2.0f;
	float x = cx - (float)width / (float)w, y = cy- (float)(height-center_line_y)/h;// /2.0f

	float xscale = 2.0f / w, yscale = 2.0f / h;

	// 对文本中的所有字符迭代
	for (auto& ch : Characters)
	{
		GLfloat xpos = x + ch.Bearing.x * xscale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * yscale;

		GLfloat w = ch.Size.x * xscale;
		GLfloat h = ch.Size.y * yscale;

		// 当前字符的VBO
		GLfloat vertices[4][2] = {
			{ xpos,     ypos + h },//,   0.0, 0.0
			{ xpos,     ypos},//,       0.0, 1.0 
			{ xpos + w, ypos},//,       1.0, 1.0 
			{ xpos + w, ypos + h}//,   1.0, 0.0 
		};
		// 在方块上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);


		/** 开始绘制四边形 */
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(vertices[0][0], vertices[0][1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(vertices[3][0], vertices[3][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(vertices[2][0], vertices[2][1]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(vertices[1][0], vertices[1][1]);
		glEnd();

		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * xscale; //(2^6 = 64)
	}
}

#include "TFreeType.h"

#include <stdexcept>
#include <algorithm>

#include <freetype/freetype.h>
#include <freetype/ftbitmap.h>

#ifdef _DEBUG
#pragma comment(lib,"Debug/freetyped.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\bzip2\\bzip2-1.0.6\\Debug\\bzip2.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\libpng\\libpng-1.6.35\\out\\Debug\\libpng16_staticd.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\zlib\\zlib-1.2.11\\out\\Debug\\zlibstaticd.lib")
#else
#pragma comment(lib,"Release/freetype.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\bzip2\\bzip2-1.0.6\\Release\\bzip2.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\libpng\\libpng-1.6.35\\out\\Release\\libpng16_static.lib")
#pragma comment(lib,"D:\\REFER\\Program\\lib\\zlib\\zlib-1.2.11\\out\\Release\\zlibstatic.lib")
#endif

using namespace std;


TFreeType::TFreeType(std::tstring s, std::string font_name, int pixel_size):
	pixel_size(pixel_size),width_pixel(0),height_pixel(0),max_bearingY_pixel(INT_MIN),xmin_pixel(INT_MAX),scale(1.0f)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		throw runtime_error("Could not init FreeType Library");

	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face))
		throw runtime_error("Failed to load font: "+font_name);

	if (FT_Set_Pixel_Sizes(face, 0, pixel_size))
	throw runtime_error("FT_Set_Pixel_Sizes error");

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用byte-alignment限制

	int xmax = INT_MIN;
	int ymin = INT_MAX, ymax = INT_MIN;

	int x_pixel = 0, y_pixel = 0;
	for (auto c:s)
	{
		FT_UInt glyph_index = FT_Get_Char_Index(face, c);
		if (glyph_index == 0)
		{
			//未定义字符会显示成方框
			//throw no_char_error("FT_Get_Char_Index error: no this character");
		}
		if (FT_Load_Glyph(
			face, /* handle to face object */
			glyph_index, /* glyph index */
			FT_LOAD_COLOR)) /* load flags, see below */
			throw runtime_error("FT_Load_Glyph error");

		if (FT_Render_Glyph(face->glyph, /* glyph slot */
			FT_RENDER_MODE_NORMAL)) /* render mode */
			throw runtime_error("FT_Render_Glyph error");

		//以灰度映射RGBA的所有通道
		int pixels = face->glyph->bitmap.width * face->glyph->bitmap.rows;
		unsigned char* data = new unsigned char[4 * pixels];
		unsigned char* src = face->glyph->bitmap.buffer;
		for (int i = 0; i < pixels; ++i)
		{
			data[4 * i + 0] = src[i];
			data[4 * i + 1] = src[i];
			data[4 * i + 2] = src[i];
			data[4 * i + 3] = src[i];
		}

		/* 生成字形纹理 */
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 设置纹理选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,//GL_RED
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RGBA,//GL_RED
			GL_UNSIGNED_BYTE,
			data//face->glyph->bitmap.buffer
		);

		delete[] data;
		/* 纹理绑定完成 */

		// 将字符存储到字符表中备用
		int w = face->glyph->bitmap.width;
		int h = face->glyph->bitmap.rows;

		Character ch = {
			texture,
			glm::ivec2(w, h),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			c==TEXT('\n')
		};
		Characters.push_back(ch);

		/* 坐标计算 */
		int xpos = x_pixel + ch.Bearing.x;
		int ypos = y_pixel - (ch.Size.y - ch.Bearing.y);
		max_bearingY_pixel = max(max_bearingY_pixel, ch.Bearing.y);


		xmin_pixel = min(xmin_pixel, ch.Bearing.x);
		xmax = max(xmax, xpos + w);
		ymin = min(ymin, ypos);
		ymax = max(ymax, ypos + h);

		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x_pixel += ch.Advance >> 6; //(2^6 = 64)
	}
	width_pixel = xmax - xmin_pixel;
	height_pixel = ymax - ymin;

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TFreeType::DrawByPixel(GLfloat x, GLfloat y, GLfloat xscale,GLfloat yscale)
{
	// 对文本中的所有字符迭代
	for (auto& ch : Characters)
	{
		GLfloat xpos = x + ch.Bearing.x * xscale*scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * yscale*scale;

		GLfloat w = ch.Size.x * xscale*scale;
		GLfloat h = ch.Size.y * yscale*scale;

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
		x += (ch.Advance >> 6) * xscale*scale; //(2^6 = 64)
	}
}

void TFreeType::SetFontSizeScale(float i)
{
	scale = i;
}

float TFreeType::GetFontSizeScale()
{
	return scale;
}


float TFreeType::GetClipCoordHeight(int H)
{
	float yscale = 2.0f / H;
	return pixel_size * yscale * scale;
}
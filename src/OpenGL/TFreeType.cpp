#include "TFreeType.h"

#include <stdexcept>

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


TFreeType::TFreeType(std::tstring s, std::string font_name, int pixel_size):pixel_size(pixel_size)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		throw runtime_error("Could not init FreeType Library");

	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face))
		throw runtime_error("Failed to load font: "+font_name);

	if (FT_Set_Pixel_Sizes(face, 0, pixel_size))
	throw runtime_error("FT_Set_Pixel_Sizes error");

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //����byte-alignment����

	int now_x = 0, now_y = 0;
	for (auto c:s)
	{
		FT_UInt glyph_index = FT_Get_Char_Index(face, c);
		if (glyph_index == 0)
		{
			//δ�����ַ�����ʾ�ɷ���
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

		//�ԻҶ�ӳ��RGBA������ͨ��
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

		// ������������
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// ��������ѡ��
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
		// ���ַ��洢���ַ����б���
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.push_back(character);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TFreeType::DrawByPixel(GLfloat x, GLfloat y, GLfloat xscale,GLfloat yscale)
{
	// ���ı��е������ַ�����
	for (auto& ch : Characters)
	{
		GLfloat xpos = x + ch.Bearing.x * xscale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * yscale;

		GLfloat w = ch.Size.x * xscale;
		GLfloat h = ch.Size.y * yscale;

			// ��ǰ�ַ���VBO
			GLfloat vertices[4][2] = {
				{ xpos,     ypos + h },//,   0.0, 0.0
				{ xpos,     ypos},//,       0.0, 1.0 
				{ xpos + w, ypos},//,       1.0, 1.0 
				{ xpos + w, ypos + h}//,   1.0, 0.0 
		};
		// �ڷ����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);


		/** ��ʼ�����ı��� */
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(vertices[0][0], vertices[0][1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(vertices[3][0], vertices[3][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(vertices[2][0], vertices[2][1]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(vertices[1][0], vertices[1][1]);
		glEnd();

		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.Advance >> 6) * xscale; //(2^6 = 64)
	}
}
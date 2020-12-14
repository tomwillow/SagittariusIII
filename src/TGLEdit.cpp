#include "TGLEdit.h"

#include "gl_assist.h"

using namespace std;

TGLEdit::TGLEdit(std::tstring s, std::string font_name, int pixel_size):
	is_hover(false),text(nullptr)
{
}

void TGLEdit::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	EnableGeometry();

	//border
	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();

	//background
	glBegin(GL_QUADS);
	glColor4f(0.0f, 0.5f, 0.8f, 0.5f);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();

	if (text)
	{
		EnableTexture();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		text->DrawByClipCoord(w, h, x1,y1,x2,y2);
	}
}

void TGLEdit::OnChar(TCHAR tc, LPARAM lParam)
{
	if (tc >= 0x20)
		s += tc;
	else
		if (tc == TEXT('\b') && !s.empty())
			s.pop_back();

	if (s.empty())
		text = nullptr;
	else
	{
		text = make_unique<TBoxFreeType>(s, FONT_CJK);
	}
}

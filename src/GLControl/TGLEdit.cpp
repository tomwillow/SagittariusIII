#include "TGLEdit.h"

#include "gl_assist.h"

#include "ColorConfig.h"

using namespace std;

TGLEdit::TGLEdit(std::tstring s, std::string font_name, int pixel_size) :
	is_hover(false), text(nullptr)
{
}

void TGLEdit::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	static float t0 = (float)GetTickCount() / 1000.0f;
	float t = (float)GetTickCount() / 1000.0f;
	EnableGeometry();

	//background
	ApplyEditBackground();
	DrawRect(GL_QUADS, x1, y1, x2, y2);

	//border
	ApplyEditBorder();
	DrawRect(GL_LINE_LOOP, x1, y1, x2, y2);

	if (text)
	{
		EnableTexture();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		text->DrawVCenterByClipCoord(w, h, x1, y1, x2, y2);
	}

	//cursor

	bool cursor_show = (int)t % 2;
	if (cursor_show)
	{
		int cursor_width_pixel = 2;
		float cursor_width = (float)cursor_width_pixel / (float)w * 2.0f;
		float cursor_height = y2 - y1;

		float cursor_x = x1;
		if (text)
			cursor_x = text->GetXClipCoordInVCenter(cursor_pos, w, h, x1, y1, x2, y2);

		EnableGeometry();

		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		DrawRect(GL_QUADS, cursor_x, y1, cursor_x + cursor_width, y2);
	}
}

void TGLEdit::OnChar(TCHAR tc, LPARAM lParam)
{
	if (tc >= 0x20)
	{
		//s += tc;
		s.insert(s.begin() + cursor_pos, tc);
		cursor_pos++;
	}
	else
		if (!s.empty())
		{
			switch (tc)
			{
			case TEXT('\b'):
				if (cursor_pos > 0)
				{
					s.erase(cursor_pos - 1, 1);
					cursor_pos--;
				}
				break;
			}
		}

	if (s.empty())
		text = nullptr;
	else
	{
		text = make_unique<TBoxFreeType>(s, FONT_CJK);
	}
}

void TGLEdit::OnKeyDown(WPARAM vk_code, LPARAM lParam)
{
	switch (vk_code)
	{
	case VK_LEFT:
		if (!s.empty() && cursor_pos > 0)
			cursor_pos--;
		break;
	case VK_RIGHT:
		if (!s.empty() && cursor_pos < s.size())
			cursor_pos++;
		break;
	}
}

std::tstring TGLEdit::GetValue()
{
	return s;
}

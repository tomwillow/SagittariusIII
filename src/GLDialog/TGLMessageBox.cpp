#include "TGLMessageBox.h"

#include "gl_assist.h"

#include "TIniFile.h"

#include "StringConfig.h"

using namespace std;

TGLMessageBox::TGLMessageBox(TIniFile* lang, std::tstring text, std::tstring caption, unsigned int mb_code, float width, float height):
	TGLDialog(width,height),s_content(text),s_caption(caption),mb_code(mb_code),
	text(make_unique<TBoxFreeType>(s_content,FONT_CJK,text_pixel))
{
	if (mb_code & MB_OKCANCEL)
	{
		buttons.emplace_back(lang->GetValue(STR_OK), FONT_CJK);
		buttons.emplace_back(lang->GetValue(STR_CANCEL), FONT_CJK);
		return;
	}
	if (mb_code == MB_OK)
	{
		buttons.emplace_back(lang->GetValue(STR_OK), FONT_CJK);
		return;
	}

	assert(0);
}

TGLMessageBox::TGLMessageBox(TIniFile* lang, std::tstring text, std::tstring caption, unsigned int mb_code):
	TGLMessageBox(lang,text,caption,mb_code,0.8f,0.8f)
{
}

void TGLMessageBox::Draw(int w, int h)
{
	this->TGLDialog::Draw(w, h);

	if (t < fade_time)
		return;

	//content
	EnableTexture();
	//text->SetFontSizeScale(0.5f);
	text->DrawByClipCoord(w, h,x1+x_margin,y1+bottom_margin,x2-x_margin,y2-top_margin);

	//buttons
	int btn_count = buttons.size();
	float btn_interval = 0.2f;
	float btn_width = (width-x_margin*2.0f - (btn_count - 1) * btn_interval) / (float)btn_count;
	text_height = (float)text_pixel / (float)h * 2.0f;
	float btn_height = text_height * 1.1f;

	float x1 = -width / 2.0f + x_margin;
	float y1 = -height / 2.0f + bottom_margin;
	float y2 = y1 + btn_height;
	for (int i = 0; i < buttons.size(); ++i)
	{
		auto& btn = buttons[i];
		float x2 = x1 + btn_width;
		btn.DrawByClipCoord(w, h, x1, y1, x2, y2);
		x1 += btn_width + btn_interval;
	}
}

int TGLMessageBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		return OnKeyDown(wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	return 0;
}

int TGLMessageBox::OnKeyDown(WPARAM vk_code, LPARAM lParam)
{
	switch (mb_code)
	{
	case MB_OKCANCEL:
	case MB_OK:
		if (vk_code == VK_RETURN)
			return IDOK;
		break;
	}
	return 0;
}

void TGLMessageBox::OnMouseMove(WPARAM mk_code, int x, int y)
{
	for (auto& btn : buttons)
		btn.OnMouseMove(mk_code, x, y);
}

int TGLMessageBox::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	if (mb_code & MB_OKCANCEL)
	{
		if (buttons[0].OnLButtonDown(mk_code, x, y))
			return IDOK;
		if (buttons[1].OnLButtonDown(mk_code, x, y))
			return IDCANCEL;
	}
	if (mb_code == MB_OK)
	{
		if (buttons[0].OnLButtonDown(mk_code, x, y))
			return IDOK;
	}
	return 0;
}

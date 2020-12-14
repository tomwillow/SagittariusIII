#include "TGLMessageBox.h"

#include "gl_assist.h"

using namespace std;

TGLMessageBox::TGLMessageBox(std::tstring text, std::tstring caption, unsigned int mb_code, float width, float height):
	TGLDialog(width,height),s_content(text),s_caption(caption),mb_code(mb_code),
	text(make_unique<TBoxFreeType>(s_content,FONT_CJK,text_pixel))
{
	if (mb_code & MB_OKCANCEL)
	{
		buttons.emplace_back(TEXT("确定"), FONT_CJK);
		buttons.emplace_back(TEXT("取消"), FONT_CJK);
		return;
	}
	assert(0);
}

void TGLMessageBox::Draw(int w, int h)
{
	this->TGLDialog::Draw(w, h);

	if (t < fade_time)
		return;

	//content
	EnableTexture();
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
	return 0;
}

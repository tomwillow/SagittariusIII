#include "TGLEditDialog.h"

using namespace std;

TGLEditDialog::TGLEditDialog(std::tstring text, std::tstring caption, unsigned int mb_code, float width, float height):
	TGLMessageBox(text,caption,mb_code,width,height),edit(make_unique<TGLEdit>(TEXT(""),FONT_CJK))
{
}

void TGLEditDialog::Draw(int w, int h)
{
	this->TGLMessageBox::Draw(w, h);

	float edit_y2 = y2 - top_margin - text_height - 0.05f;
	edit->DrawByClipCoord(w, h, x1 + x_margin, edit_y2-text_height, x2 - x_margin, edit_y2);
}

void TGLEditDialog::OnMouseMove(WPARAM mk_code, int x, int y)
{
	this->TGLMessageBox::OnMouseMove(mk_code, x, y);
}

int TGLEditDialog::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	return this->TGLMessageBox::OnLButtonDown(mk_code, x, y);
}

void TGLEditDialog::OnChar(TCHAR tc, LPARAM lParam)
{
	edit->OnChar(tc, lParam);
}

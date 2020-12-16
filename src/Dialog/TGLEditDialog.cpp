#include "TGLEditDialog.h"

using namespace std;

TGLEditDialog::TGLEditDialog(std::tstring text, std::tstring caption, unsigned int mb_code):
	TGLMessageBox(text,caption,mb_code,1.0f,1.0f),edit(make_unique<TGLEdit>(TEXT(""),FONT_CJK))
{
}

int TGLEditDialog::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CHAR:
		edit->OnChar(wParam, lParam);
		return 0;
	default:
		return this->TGLMessageBox::WndProc(uMsg, wParam, lParam);
	}
	return 0;
}

void TGLEditDialog::Draw(int w, int h)
{
	this->TGLMessageBox::Draw(w, h);

	float edit_y2 = y2 - top_margin - text_height - 0.05f;
	edit->DrawByClipCoord(w, h, x1 + x_margin, edit_y2-text_height, x2 - x_margin, edit_y2);
}

void TGLEditDialog::OnChar(TCHAR tc, LPARAM lParam)
{
	edit->OnChar(tc, lParam);
}

std::tstring TGLEditDialog::GetValue()
{
	return edit->GetValue();
}

#include "GLNetworkDialog.h"

using namespace std;

GLNetworkDialog::GLNetworkDialog():
	TGLDialog(1.5f,1.5f),
	text(make_unique<TBoxFreeType>(TEXT("搜索局域网房间："), FONT_CJK, text_pixel))
{
	buttons.emplace_back(TEXT("IP直连"), FONT_CJK, text_pixel);
	buttons.emplace_back(TEXT("开设房间"), FONT_CJK, text_pixel);
	buttons.emplace_back(TEXT("取消"), FONT_CJK, text_pixel);
}

void GLNetworkDialog::Draw(int w, int h)
{
	this->TGLDialog::Draw(w, h);

	text_height = text->GetClipCoordHeight(h);
	text->DrawVCenterByClipCoord(w, h, x1+x_margin, y2-top_margin - text_height, x2-x_margin, y2-top_margin);

	float y_interval = 0.08f;

	float button_interval = 0.08f;
	float button_left = x1 + x_margin;
	float button_right = x2 - x_margin;
	float button_height = text_height * 1.1f;
	float button_bottom = y1 + bottom_margin;
	float button_top = button_bottom + button_height;

	float list_top = y2 - top_margin - text_height - y_interval;
	float list_bottom = button_top + y_interval;

	float button_width = (button_right - button_left-(button_interval*((float)buttons.size()-1.0f))) / buttons.size();
	float button_x = button_left;
	for (auto& btn : buttons)
	{
		btn.DrawByClipCoord(w, h, button_x, button_bottom, button_x + button_width, button_top);
		button_x += button_width+button_interval;
	}

	//
	if (ipDialog)
	{
		ipDialog->Draw(w, h);
	}

	if (ipMsgBox)
	{
		ipMsgBox->Draw(w, h);
	}
}

int GLNetworkDialog::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//
	if (ipMsgBox)
	{
		if (ipMsgBox->WndProc(uMsg, wParam, lParam) == IDOK)
		{
			ipMsgBox = nullptr;
		}
		return 0;
	}

	//
	if (ipDialog)
	{
		int key = ipDialog->WndProc(uMsg, wParam, lParam);
		if (key == IDOK)
		{
			tstring sip = ipDialog->GetValue();
			//

			ipMsgBox= make_unique<TGLEditDialog>(TEXT("IP格式不正确。"), TEXT(""), MB_OK);
		}
		if (key == IDCANCEL)
		{
			ipDialog = nullptr;
		}
		return 0;
	}

	//
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		for (auto& btn : buttons)
			btn.OnMouseMove(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int key = 0;
		key = buttons[0].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (key)
		{
			ipDialog = make_unique<TGLEditDialog>(TEXT("请输入服务器IP："), TEXT(""), MB_OKCANCEL);
		}

		key = buttons[2].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (key)
			return IDCANCEL;
		break;
	}
	}
	return 0;
}

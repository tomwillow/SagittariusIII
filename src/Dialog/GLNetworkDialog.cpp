#include "MySocket.h"

#include "SceneController.h"

#include "GLNetworkDialog.h"

#include "gl_assist.h"

#include "StringConfig.h"
#include "ColorConfig.h"

using namespace std;

GLNetworkDialog::GLNetworkDialog(SceneController* in_controller) :
	controller(in_controller),
	TGLDialog(1.5f,1.5f),
	text(make_unique<TBoxFreeType>(controller->lang.GetValue(STR_SEARCH_ROOM), FONT_CJK, text_pixel))
{
	buttons.emplace_back(controller->lang.GetValue(STR_CONNECT_IP), FONT_CJK, text_pixel);
	buttons.emplace_back(controller->lang.GetValue(STR_BUILD_ROOM), FONT_CJK, text_pixel);
	buttons.emplace_back(controller->lang.GetValue(STR_CANCEL), FONT_CJK, text_pixel);
}

void GLNetworkDialog::Draw(int w, int h)
{
	this->TGLDialog::Draw(w, h);

	if (t < fade_time)
		return;

	EnableTexture();

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

	//draw list
	EnableGeometry(); 
	ApplyEditBackground();
	DrawRect(GL_QUADS, button_left, list_bottom, button_right, list_top);

	ApplyEditBorder();
	DrawRect(GL_LINE_LOOP, button_left, list_bottom, button_right, list_top);

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
			controller->PlaySoundEffect();
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
			controller->PlaySoundEffect();
			tstring sip = ipDialog->GetValue();
			//
			if (CheckIP(wstring2string(sip)))//ip正确，进行连接
			{
				//
				controller->SetHost(false);
			}
			else
			{
				ipMsgBox = make_unique<TGLMessageBox>(&controller->lang,controller->lang.GetValue(STR_WRONG_IP), TEXT(""), MB_OK);
			}
		}
		if (key == IDCANCEL)
		{
			controller->PlaySoundEffect();
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
		if (key)//打开服务器ip对话框
		{
			controller->PlaySoundEffect();
			ipDialog = make_unique<TGLEditDialog>(&controller->lang,controller->lang.GetValue(STR_INPUT_SERVER_IP), TEXT(""), MB_OKCANCEL);
		}

		key = buttons[1].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (key)//开设房间
		{
			controller->SetHost(true);
			controller->PlaySoundEffect();
			return IDBUILDROOM;
		}

		key = buttons[2].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (key)//取消
		{
			controller->PlaySoundEffect();
			return IDCANCEL;
		}
		break;
	}
	}
	return 0;
}

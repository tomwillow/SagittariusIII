#include "GLUsernameDialog.h"

#include "SceneController.h"

#include "StringConfig.h"

using namespace std;

GLUsernameDialog::GLUsernameDialog(SceneController* in_controller) :
	controller(in_controller),
    TGLEditDialog(&in_controller->lang,in_controller->lang.GetValue(STR_INPUT_NAME), TEXT(""), MB_OKCANCEL)
{
}

int GLUsernameDialog::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (msgBox)
	{
		//确认姓名不正确
		int key = msgBox->WndProc(uMsg, wParam, lParam);
		if (key == IDOK)
		{
			controller->PlaySoundEffect();
			msgBox = nullptr;
		}
		return 0;
	}

	int key = this->TGLEditDialog::WndProc(uMsg, wParam, lParam);
	if (key == IDOK)
	{
		controller->PlaySoundEffect();
		tstring username = this->TGLEditDialog::GetValue();
		if (username.empty())
		{
			msgBox = make_unique<TGLMessageBox>(&controller->lang,controller->lang.GetValue(STR_WRONG_USERNAME), TEXT(""), MB_OK);
			return 0;
		}
		if (username.find(TEXT(' ')) != tstring::npos)
		{
			msgBox = make_unique<TGLMessageBox>(&controller->lang,controller->lang.GetValue(STR_DO_NOT_BLANK), TEXT(""), MB_OK);
			return 0;
		}

		//输入用户名成功

		return IDOK;
	}
	if (key == IDCANCEL)
	{
		controller->PlaySoundEffect();
		return IDCANCEL;
	}
    return 0;
}

void GLUsernameDialog::Draw(int w, int h)
{
	this->TGLEditDialog::Draw(w, h);

	if (t < fade_time)
		return;

	if (msgBox)
		msgBox->Draw(w, h);

}

std::tstring GLUsernameDialog::GetValue()
{
	return this->TGLEditDialog::GetValue();
}

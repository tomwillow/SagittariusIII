#include "GLUsernameDialog.h"

#include "SceneController.h"

using namespace std;

GLUsernameDialog::GLUsernameDialog(SceneController* in_controller) :
	controller(in_controller),
    TGLEditDialog(TEXT("�������û�����"), TEXT(""), MB_OKCANCEL)
{
}

int GLUsernameDialog::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (msgBox)
	{
		//ȷ����������ȷ
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
			msgBox = make_unique<TGLMessageBox>(TEXT("�û�������ȷ��"), TEXT(""), MB_OK);
			return 0;
		}
		if (username.find(TEXT(' ')) != tstring::npos)
		{
			msgBox = make_unique<TGLMessageBox>(TEXT("�����пո�"), TEXT(""), MB_OK);
			return 0;
		}

		//�����û����ɹ�

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

#include "SceneMatch.h"

#include "gl_assist.h"

#include "SceneController.h"

using namespace std;

SceneMatch::SceneMatch(SceneController* controller, int w, int h) :
	Scene(controller), text(nullptr), msgBox(nullptr),searchDialog(nullptr)
{
	starRender = make_unique<StarRender>(w, h);

	if (controller->ini.GetValue(TEXT("username")).empty())
		editDialog = make_unique<TGLEditDialog>(TEXT("�������û�����"), TEXT(""), MB_OKCANCEL);
	else
		searchDialog = make_unique<GLNetworkDialog>();
}

SceneMatch::SceneMatch(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender) :
	Scene(controller), text(nullptr), msgBox(nullptr), searchDialog(nullptr)
{
	starRender.reset(lastStarRender.release());

	if (controller->ini.GetValue(TEXT("username")).empty())
		editDialog = make_unique<TGLEditDialog>(TEXT("�������û�����"), TEXT(""), MB_OKCANCEL);
	else
		searchDialog = make_unique<GLNetworkDialog>();
}

int SceneMatch::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (msgBox)
	{
		int key = msgBox->WndProc(uMsg, wParam, lParam);
		if (key == IDOK)
		{
			controller->PlaySoundEffect();
			msgBox = nullptr;
		}
		return 0;
	}

	if (editDialog)
	{
		int key = editDialog->WndProc(uMsg, wParam, lParam);
		if (key == IDOK)
		{
			controller->PlaySoundEffect();
			tstring username = editDialog->GetValue();
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
			controller->ini.SetValue(TEXT("username"), username);
			controller->ini.SaveToFile();
			editDialog = nullptr;

			searchDialog = make_unique<GLNetworkDialog>();
			
			return 0;
		}
		if (key == IDCANCEL)
		{
			controller->PlaySoundEffect();
			controller->GoCover(W, H);
			return 0;
		}
		return 0;
	}

	if (searchDialog)
	{
		int key=searchDialog->WndProc(uMsg, wParam, lParam);
		if (key == IDCANCEL)
		{
			controller->PlaySoundEffect();
			controller->GoCover(W, H);
			return 0;
		}
	}

	return 0;
}

void SceneMatch::Render(int w, int h)
{
	W = w; H = h;
	float t = (float)GetTickCount() / 1000.0f;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//��������ʱ����ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���������������ɫ����������Ȼ�����

	starRender->Draw(w, h, t);

	if (editDialog)
		editDialog->Draw(w, h);

	if (msgBox)
		msgBox->Draw(w, h);

	if (searchDialog)
		searchDialog->Draw(w, h);
}

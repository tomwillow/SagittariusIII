#include "SceneController.h"

#include "SceneIntro.h"
#include "SceneCover.h"
#include "SceneMatch.h"

using namespace std;

SceneController::SceneController(int w, int h):
	scene(make_unique<SceneIntro>(this)),
	ini(TEXT("config.ini"))
{
	//GoCover(w,h);
	GoMatch(w, h);
}

SceneController::~SceneController()
{
	ini.SaveToFile();
}

void SceneController::GoCover(int w, int h)
{
	scene=make_unique<SceneCover>(this);
	scene->Start(w, h);
}

void SceneController::GoMatch(int w, int h)
{
	if (typeid(*scene) == typeid(SceneCover))
	{
		scene=make_unique<SceneMatch>(this, dynamic_cast<SceneCover*>(scene.get())->starRender);
		scene->Start(w, h);
	}
	else
	{
		scene = make_unique<SceneMatch>(this,w,h);
		scene->Start(w, h);
	}
}

void SceneController::OnKeyDown(WPARAM vk_code, LPARAM lParam)
{
	scene->OnKeyDown(vk_code, lParam);
}

void SceneController::OnChar(TCHAR tc, LPARAM lParam)
{
	scene->OnChar(tc, lParam);
}

void SceneController::OnMouseMove(WPARAM mk_code, int x, int y)
{
	scene->OnMouseMove(mk_code, x, y);
}

void SceneController::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	scene->OnLButtonDown(mk_code, x, y);
}

void SceneController::OnLButtonUp(WPARAM mk_code, int x, int y)
{
	scene->OnLButtonUp(mk_code, x, y);
}

void SceneController::Render(int w, int h)
{
	static auto start = GetTickCount();
	scene->Render(w,h);
}

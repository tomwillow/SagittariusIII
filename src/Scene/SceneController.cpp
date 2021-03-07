#include "SceneController.h"

#include "SceneIntro.h"
#include "SceneCover.h"
#include "SceneMatch.h"
#include "ScenePrepRoom.h"
#include "SceneYukiDispr.h"
#include "SceneGame.h"

using namespace std;

SceneController::SceneController(int w, int h):
	scene(make_unique<SceneIntro>(this)),
	ini(TEXT("config.ini")),
	isHost(false),
	receivedMsg(
		{WM_KEYDOWN,WM_KEYUP,
		WM_CHAR,WM_MOUSEMOVE,
		WM_LBUTTONDOWN,WM_LBUTTONUP,
		WM_RBUTTONDOWN,
		WM_SIZE,
		WM_KILLFOCUS}),
	lang(ini.GetValue(TEXT("language")))
{

	GoYukiDispr(w,h);//不符合条件会自动跳转至Intro
#ifdef _DEBUG

	//GoMatch(w, h);

	//SetHost(true);

	SetHost(true);
	GoPrepRoom(w, h);
#endif
}

SceneController::~SceneController()
{

	ini.SaveToFile();
}


void SceneController::PlayBGM()
{
	soundManager.PlayBGM();
}

void SceneController::PlaySoundEffect()
{
	soundManager.PlaySoundEffect();
}

void SceneController::SetHost(bool is_host)
{
	isHost = is_host;
}

bool SceneController::IsHost()
{
	return isHost;
}

void SceneController::GoIntro(int w, int h)
{
	scene = make_unique<SceneIntro>(this);
	scene->Start(w, h);
}

void SceneController::GoYukiDispr(int w, int h)
{
	scene = make_unique<SceneYukiDispr>(this);
	scene->Start(w, h);
}

void SceneController::GoCover(int w, int h)
{
	scene=make_unique<SceneCover>(this);
	scene->Start(w, h);
#ifdef _DEBUG
	//PlayBGM();
#else
	PlayBGM();
#endif
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

void SceneController::GoPrepRoom(int w, int h)
{
	scene = make_unique<ScenePrepRoom>(this, w, h);
	scene->Start(w, h);
}

void SceneController::GoGame(int w, int h)
{
	scene = make_unique<SceneGame>(this);
	scene->Start(w, h);
}

int SceneController::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return scene->WndProc(uMsg, wParam, lParam);
}

void SceneController::Render(int w, int h)
{
	static auto start = GetTickCount();
	scene->Render(w,h);
}

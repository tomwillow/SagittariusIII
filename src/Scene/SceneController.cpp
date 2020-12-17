#include "SceneController.h"

#include "SceneIntro.h"
#include "SceneCover.h"
#include "SceneMatch.h"
#include "ScenePrepRoom.h"

using namespace std;

void SceneController::PlayBGMThread()
{
	stopBGM = false;
	while (!stopBGM)
	{
		renderBGM->Render();
		waveoutBGM->PlayAudio(renderBGM->buf, renderBGM->buf_size);
	}
}

SceneController::SceneController(int w, int h):
	scene(make_unique<SceneIntro>(this)),
	ini(TEXT("config.ini")),
	stopBGM(true),
	receivedMsg({WM_KEYDOWN,WM_CHAR,WM_MOUSEMOVE,WM_LBUTTONDOWN,WM_RBUTTONDOWN})
{
	wavefile = make_unique<WaveFile>();

	//
	waveoutBGM = make_unique<WaveOut>(wavefile->GetHeader());
	waveoutBGM->Start();
	renderBGM = make_unique<NSFRender>(SND_TITLE, wavefile->GetHeader());

	//
	waveoutSelect = make_unique<WaveOut>(wavefile->GetHeader(), 200);
	waveoutSelect->Start();
	renderSelect = make_unique<NSFRender>(SND_SELECT, wavefile->GetHeader(), 200);

#ifdef _DEBUG
	//GoCover(w,h);
	//GoMatch(w, h);
	GoPrepRoom(w, h);
#endif
}

SceneController::~SceneController()
{
	stopBGM = true;
	if (t.joinable())
		t.join();

	ini.SaveToFile();
}


void SceneController::PlayBGM()
{
	if (!t.joinable())
		t = thread(&SceneController::PlayBGMThread, this);
}

void SceneController::PlaySoundEffect()
{
	renderSelect->Reset();
	renderSelect->Render();
	waveoutSelect->PlayAudio(renderSelect->buf, renderSelect->buf_size);
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

int SceneController::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return scene->WndProc(uMsg, wParam, lParam);
}

void SceneController::Render(int w, int h)
{
	static auto start = GetTickCount();
	scene->Render(w,h);
}

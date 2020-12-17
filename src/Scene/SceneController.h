#pragma once

#include "Scene.h"

#include "TIniFile.h"


#include <thread>

#include "NSFRender.h"
#ifdef _DEBUG
#pragma comment(lib,"Debug/NSFRender.lib")
#else
#pragma comment(lib,"Release/NSFRender.lib")
#endif

#include "WaveFile.h"
#include "WaveOut.h"

class Scene;
class SceneController
{
private:
	std::unique_ptr<Scene> scene;

	bool stopBGM;
	std::unique_ptr<NSFRender> renderBGM, renderSelect;
	std::unique_ptr<WaveOut> waveoutBGM, waveoutSelect;
	std::unique_ptr<WaveFile> wavefile;
	std::thread t;
	void PlayBGMThread();
public:
	TIniFile ini;

	SceneController(int w, int h);
	~SceneController();

	void GoCover(int w, int h);
	void GoMatch(int w, int h);
	void GoPrepRoom(int w, int h);

	std::set<UINT> receivedMsg;
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Render(int w, int h);

	void PlayBGM();
	void PlaySoundEffect();
};


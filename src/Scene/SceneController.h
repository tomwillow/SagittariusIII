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
	bool isHost;
	std::unique_ptr<Scene> scene;

	bool stopBGM;
	std::unique_ptr<NSFRender> renderBGM, renderSelect;
	std::unique_ptr<WaveOut> waveoutBGM, waveoutSelect;
	std::unique_ptr<WaveFile> wavefile;
	std::thread t;
	void PlayBGMThread();
public:
	TIniFile ini;
	TIniFile lang;

	SceneController(int w, int h);
	~SceneController();

	void GoIntro(int w, int h);
	void GoYukiDispr(int w, int h);
	void GoCover(int w, int h);
	void GoMatch(int w, int h);
	void GoPrepRoom(int w, int h);
	void GoGame(int w, int h);

	std::set<UINT> receivedMsg;//控制器总共可以接收的消息种类
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);//由管理者负责调用，控制器将消息分发至分场景

	void Render(int w, int h);

	void PlayBGM();
	void PlaySoundEffect();

	void SetHost(bool is_host);//设置本机为主机
	bool IsHost();//返回是否为主机
};


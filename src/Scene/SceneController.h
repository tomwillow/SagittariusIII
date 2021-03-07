#pragma once

#include <set>

#include "Scene.h"
#include "TIniFile.h"
#include "Player.h"
#include "SoundManager.h"

class Scene;
class SceneController
{
private:
	bool isHost;
	std::unique_ptr<Scene> scene;
	SoundManager soundManager;

public:
	TIniFile ini;
	TIniFile lang;
	std::vector<Player> players;
	float bat_width = 50.0f;
	float bat_height = 120.0f;

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


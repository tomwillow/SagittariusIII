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

	std::set<UINT> receivedMsg;//�������ܹ����Խ��յ���Ϣ����
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);//�ɹ����߸�����ã�����������Ϣ�ַ����ֳ���

	void Render(int w, int h);

	void PlayBGM();
	void PlaySoundEffect();

	void SetHost(bool is_host);//���ñ���Ϊ����
	bool IsHost();//�����Ƿ�Ϊ����
};


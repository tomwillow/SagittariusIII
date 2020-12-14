#pragma once

#include "Scene.h"

class Scene;
class SceneController
{
private:
	std::unique_ptr<Scene> scene;
public:
	SceneController(int w, int h);
	void GoCover(int w, int h);
	void GoMatch(int w, int h);
	void OnKeyDown(WPARAM vk_code, LPARAM lParam);
	void OnChar(TCHAR tc, LPARAM lParam);
	void OnMouseMove(WPARAM mk_code, int x, int y);
	void OnLButtonDown(WPARAM mk_code, int x, int y);
	void OnLButtonUp(WPARAM mk_code, int x, int y);
	void Render(int w, int h);
};


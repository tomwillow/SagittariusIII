#pragma once
#include <Windows.h>
#include <WindowsX.h>
#include <gl/GL.h>

#include <memory>

class SceneController;
class Scene
{
public:
	Scene(SceneController* controller) :controller(controller) {}
	virtual ~Scene() {}
	virtual void Start(int w,int h) {}

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) { return 0; }

	//virtual void OnKeyDown(WPARAM vk_code,LPARAM lParam) {}
	//virtual void OnChar(TCHAR tc, LPARAM lParam) {}
	//virtual void OnMouseMove(WPARAM mk_code, int x, int y) {}
	//virtual void OnLButtonDown(WPARAM mk_code, int x, int y) {}
	//virtual void OnLButtonUp(WPARAM mk_code, int x, int y) {}

	virtual void Render(int w,int h)=0;
protected:
	SceneController* controller;
};


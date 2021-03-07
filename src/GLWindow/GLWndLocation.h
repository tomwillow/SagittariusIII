#pragma once
#include "GLWindow.h"

class SceneController;
class GLWndLocation :
    public GLWindow
{
private:
	SceneController* controller;
	TTexture ship;
public:
	GLWndLocation(SceneController* controller);
	virtual void DrawByPiexl(int W, int H) override;
};


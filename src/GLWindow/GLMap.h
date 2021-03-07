#pragma once

#include "GLWindow.h"

class SceneController;
class GLMap:public GLWindow
{
private:
	SceneController* controller;
	float grid_pixel = 50;
	TTexture ship,mask;
public:
	GLMap(SceneController *controller);
	virtual void DrawByPiexl(int W, int H) override;
};


#pragma once

#include "GLWindow.h"

class GLMap:public GLWindow
{
public:
	GLMap();
	virtual void DrawByPiexl(int W, int H) override;
};


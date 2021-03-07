#pragma once
#include "GLWindow.h"
class GLWndStatus :
    public GLWindow
{
private:
	const float ival = 5;
public:
	GLWndStatus();
	virtual void DrawByPiexl(int W, int H) override;
};


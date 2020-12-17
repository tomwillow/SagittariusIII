#pragma once
#include <Windows.h>
#include <WindowsX.h>
#include <memory>

#include "ExternResource.h"

#include "TTexture.h"

class TGLDialog
{
private:
	std::unique_ptr<TTexture> bk;
protected:
	float x1, y1, x2, y2;
	const float fade_time;
	float t0,t;
	float width, height;
public:
	TGLDialog(float width, float height);
	virtual void Draw(int w, int h);
};


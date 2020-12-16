#pragma once

#include <memory>

#include "ExternResource.h"

#include "TTexture.h"

class TGLDialog
{
private:
	std::unique_ptr<TTexture> bk;
protected:
	float x1, y1, x2, y2;
	const float fade_time = 0.1f;
	float t;
	float width, height;
public:
	TGLDialog(float width, float height);
	void Draw(int w, int h);
};


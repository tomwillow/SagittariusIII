#pragma once

#include <memory>
#include "TTexture.h"

#include "TFreeType.h"

class GLWindow
{
private:
	static TTexture *tex_border;
	std::unique_ptr<TFreeType> caption;
protected:
	float x1, y1;
	float w, h;
	const float border;
	const float header;
	glm::u8vec4 bk_color;
	bool caption_align_center;
public:
	GLWindow();
	virtual void DrawByPiexl(int W, int H);

	void SetWindow(float x1, float y1, float w, float h);
	void SetSize(float w, float h);
	void SetPos(float x1, float y1);
};


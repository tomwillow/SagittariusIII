#pragma once

#include "tstring.h"
#include <memory>
#include "TTexture.h"

#include "TFreeType.h"

class GLWindow
{
private:
	bool is_dragging;
	POINT drag_start;
	POINT orig;
	static TTexture *tex_border;
	std::unique_ptr<TFreeType> caption;
	void Calc();
protected:
	float W, H;
	float x1, y1;
	float w, h;
	float ix1, iy1, ix2, iy2, iw, ih;
	const float border;
	const float header;
	glm::u8vec4 bk_color;
	bool caption_align_center;
	GLWindow(std::tstring title);
public:
	static void ReleaseResource();
	virtual void DrawByPiexl(int W, int H);

	void SetWindow(float x1, float y1, float w, float h);
	void SetSize(float w, float h);
	void SetPos(float x1, float y1);
	void SetTitle(std::tstring title);

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


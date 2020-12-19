#pragma once

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "tstring.h"

class TGLColorCombo
{
private:
	int W, H;
	float x1, y1, x2, y2; 
	float item_height;
	bool spread;
	int cur;
	int hover;//-2 : none; -1 : self, 0 -> size-1 

	float x_margin, y_margin;

	void DrawColor(int index,float x1, float y1, float x2, float y2);

	int GetHover(int x, int y);
public:
	TGLColorCombo();

	virtual void OnMouseMove(WPARAM mk_code, int x, int y);
	virtual int OnLButtonDown(WPARAM mk_code, int x, int y);
	virtual int OnRButtonDown(WPARAM mk_code, int x, int y);

	virtual void DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2);
	virtual void DrawSpreadByClipCoord(int w, int h);

	glm::vec4 GetColor();
	int GetCur();
	void SetCur(int i);
};


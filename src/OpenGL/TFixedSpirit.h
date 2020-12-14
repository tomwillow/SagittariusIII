#pragma once
#include "TTexture.h"
class TFixedSpirit :
    public TTexture
{
private:
	float x_per, y_per;
	float len_per;
	bool base_is_width;
protected:
	TFixedSpirit(float x_per, float y_per, float len_per, bool base_is_width);
	virtual void BindAndDraw_inner(float x1, float y1, float x2, float y2, int W, int H);
	void Calc(float &x1, float &y1, float &x2, float &y2, int W, int H);
public:
    TFixedSpirit(std::string filename, float x_per, float y_per, float len_per, bool base_is_width);

	//以[0,1]指定x,y范围，x_per,y_per指定中心点位置贴图
	void Draw(int W, int H);
	bool PosInTexture(int x,int y,int W, int H);
};


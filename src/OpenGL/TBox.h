#pragma once

#include "TBoxWithTex.h"

class TBox: public TBoxWithTex
{
private:
public:
	//���ݸ�ʽ��x,y,z,normal,texture
	TBox(float xpos = 0.0f, float ypos = 0.0f, float zpos = 0.0f, float size = 1.0f);
	virtual ~TBox();
};


#pragma once
#include "TVertexArray.h"
class TBoxWithTex :
	public TVertexArray
{
protected:
	static float vertices[];
public:
	//数据格式：x,y,z,normal,texture
	TBoxWithTex();
	virtual ~TBoxWithTex();
};

#pragma once
#include "TVertexArray.h"
class TBoxWithTex :
	public TVertexArray
{
protected:
	static float vertices[];
public:
	//���ݸ�ʽ��x,y,z,normal,texture
	TBoxWithTex();
	virtual ~TBoxWithTex();
};

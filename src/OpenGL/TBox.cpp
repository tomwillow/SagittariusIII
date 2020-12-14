#include "TBox.h"

#include <string.h>

TBox::TBox(float xpos, float ypos, float zpos, float size)
{
	this->TBoxWithTex::TBoxWithTex();
	data = new float[36*8];
	memcpy(data, vertices, 36 * 8 * sizeof(float));

	for (int pt = 0, index = 0; pt < 36; ++pt, index += 8)
	{
		data[index] = vertices[index] * size + xpos;
		data[index + 1] = vertices[index + 1] * size + ypos;
		data[index + 2] = vertices[index + 2] * size + zpos;
	}
	this->TVertexArray::TVertexArray(36*8*sizeof(float), data, { 3,3,2 });
}

TBox::~TBox()
{
	this->TBoxWithTex::~TBoxWithTex();
	delete[] data;
}

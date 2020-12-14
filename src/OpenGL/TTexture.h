#pragma once
#include <string>

class TTexture
{
private:
	unsigned int textureId;
protected:
	TTexture() :textureId(-1), width(0), height(0) {}
public:
	int width, height;

	//jpg: channels=3; png: channels=4;
	TTexture(std::string fileName);

	void Bind(int samplerIndex=0);
	unsigned int GetId();
	void DrawByClipCoord(float x1, float y1, float x2, float y2);
};

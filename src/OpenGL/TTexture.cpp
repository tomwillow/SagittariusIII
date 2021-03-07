//#include <glad/glad.h>
#include <Windows.h>
#include <gl/GL.h>
#include <stdexcept>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "TTexture.h"

using namespace std;

TTexture::TTexture(std::string fileName)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		int format = (nrChannels == 3) ? GL_RGB : GL_RGBA; 
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			throw std::runtime_error("Failed to load texture: "+fileName+" error:"+to_string(error));
		}
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw std::runtime_error("Failed to load texture: " + fileName);
	}
	stbi_image_free(data);
}

void TTexture::Bind(int samplerIndex)
{
	//glActiveTexture(GL_TEXTURE0+samplerIndex); // 在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, textureId);
}

unsigned int TTexture::GetId()
{
	return textureId;
}

void TTexture::DrawByClipCoord(float x1, float y1, float x2, float y2)
{
	Bind();

	/** 开始绘制四边形 */
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(x1, y1);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x2, y1);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x2, y2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x1, y2);
	glEnd();
}

void TTexture::DrawCenterByPixel(float cx, float cy,float scale)
{
	float x1 = cx - width / 2.0f*scale, x2 = cx + width / 2.0f * scale;
	float y1 = cy - height / 2.0f * scale, y2 = cy + width / 2.0f * scale;
	DrawByClipCoord(x1, y1, x2, y2);
}


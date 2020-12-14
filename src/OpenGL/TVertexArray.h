#pragma once
#include <initializer_list>

class TVertexArray
{
protected:
#ifdef _OPENGL3
	unsigned int VAO;
	unsigned int VBO;
#endif

	//protected构造函数，用于派生类的构造函数中，
	//先定义数据，再调用this->TVertexArray::TVertexArray(...)构造
	TVertexArray() {}
private:
	unsigned int bytes;
	int group_count;

	//例如分组为x,y,z,r,g,b,s,t
	//传入{3,3,2}对应location={0,1,2}
	void SetAttribPointer(std::initializer_list<int> init_list);
	virtual void Draw(unsigned int mode);
public:
	TVertexArray(unsigned int bytes, const float* data, std::initializer_list<int> init_list);
	virtual ~TVertexArray();

#ifdef _OPENGL3
	virtual void Bind();
#else
	float* data;
#endif

	//调用前需Bind，否则报错
	void DrawTriangles();
	void DrawLines();
};


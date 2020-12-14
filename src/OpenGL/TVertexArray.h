#pragma once
#include <initializer_list>

class TVertexArray
{
protected:
#ifdef _OPENGL3
	unsigned int VAO;
	unsigned int VBO;
#endif

	//protected���캯��������������Ĺ��캯���У�
	//�ȶ������ݣ��ٵ���this->TVertexArray::TVertexArray(...)����
	TVertexArray() {}
private:
	unsigned int bytes;
	int group_count;

	//�������Ϊx,y,z,r,g,b,s,t
	//����{3,3,2}��Ӧlocation={0,1,2}
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

	//����ǰ��Bind�����򱨴�
	void DrawTriangles();
	void DrawLines();
};


#pragma once

#include "Star.h"

#include "TTexture.h"
#include "ExternResource.h"

#include <vector>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class StarRender
{
private:
    const int point_count;
    const int tex_count;
    glm::mat4 projection;
    std::vector<TTexture> textures;//������ͼ
    std::vector<TTexture> tex_sos;//sos�ձ���ͼ
    std::vector<Star> point_stars;//�����
    std::vector<Star> tex_stars;//��ͼ����
    void AddPointStar(float t0);
    void AddTexStar(float t0);
public:
    StarRender(int w, int h);

    //��Ⱦ����ȫ��ʹ�òü��ռ����꣬����ǰ��ȷ��ͶӰ�����ģ�;����Ϊ��λ����
    void Draw(int w, int h,float t);
};


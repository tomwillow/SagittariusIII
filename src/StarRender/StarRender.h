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
    std::vector<TTexture> textures;//恒星贴图
    std::vector<TTexture> tex_sos;//sos徽标贴图
    std::vector<Star> point_stars;//点恒星
    std::vector<Star> tex_stars;//贴图恒星
    void AddPointStar(float t0);
    void AddTexStar(float t0);
public:
    StarRender(int w, int h);

    //渲染过程全部使用裁剪空间坐标，调用前需确保投影矩阵和模型矩阵均为单位矩阵
    void Draw(int w, int h,float t);
};


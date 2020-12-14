#pragma once
#include "Scene.h"

#include "TFixedSpirit.h"

#include "ExternResource.h"

class SceneIntro :
    public Scene
{
private:
    std::unique_ptr<TFixedSpirit> logo;
public:
    SceneIntro(SceneController* controller);
    void Render(int w, int h)override;
};


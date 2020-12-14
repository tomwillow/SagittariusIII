#pragma once
#include "Scene.h"

#include <thread>

#include "NSFRender.h"
#ifdef _DEBUG
#pragma comment(lib,"Debug/NSFRender.lib")
#else
#pragma comment(lib,"Release/NSFRender.lib")
#endif

#include "WaveFile.h"
#include "WaveOut.h"
#include "TFixedSpirit.h"
#include "TFixedFreeType.h"

#include "StarRender.h"

#include "ExternResource.h"

class SceneCover :
    public Scene
{
private:
    int W, H;
    bool stopBGM;
    std::unique_ptr<TFixedSpirit> tex1, tex2, tex3, round_rect, copyright;
    std::unique_ptr<NSFRender> renderBGM, renderSelect;
    std::unique_ptr<WaveOut> waveoutBGM, waveoutSelect;
    std::unique_ptr<WaveFile> wavefile;
    std::thread t;

    bool hoverStart, hoverLoad, hoverExit;
    std::unique_ptr<TFixedFreeType> textStart, textLoad, textExit;
    std::unique_ptr<TFreeType> text;

    std::unique_ptr<StarRender> starRender;
public:
    SceneCover(SceneController* controller);
    virtual ~SceneCover() override;
    void PlayBGM();
    void Start(int w, int h)override;
    void OnKeyDown(WPARAM vk_code, LPARAM lParam)override;
    void OnMouseMove(WPARAM mk_code, int x, int y) override;
    void OnLButtonDown(WPARAM mk_code, int x, int y)override;
    void OnLButtonUp(WPARAM mk_code, int x, int y)override {}
    void Render(int w, int h)override;

    friend class SceneController;
};


#pragma once
#include "TWindow.h"
class TSmallWindow :
    public TWindow
{
private:
#ifdef _OPENGL
    void Render()override;
#endif
};


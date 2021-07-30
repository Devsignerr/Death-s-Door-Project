#pragma once
#include "ComponentGUI.h"

class Light3DGUI :
    public ComponentGUI
{
private:
    int m_iLight3DIdx;
public:
    Light3DGUI();
    ~Light3DGUI();

public:
    virtual void finalupdate();
    virtual void render();

private:
    void BakingShadowMap();
    void LoadShadowMap();
};

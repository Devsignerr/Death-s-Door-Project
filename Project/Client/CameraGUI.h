#pragma once
#include "ComponentGUI.h"

class CameraGUI :
    public ComponentGUI
{

public:
    virtual void update();
    virtual void render();

public:
    CameraGUI();
    ~CameraGUI();

};


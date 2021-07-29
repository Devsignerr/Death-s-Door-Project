#pragma once

#include "ComponentGUI.h"

class TransformGUI :
    public ComponentGUI
{
private:
    float m_arrData[3][3];

public:
    virtual void init();
    virtual void lateupdate();
    virtual void render();


public:
    TransformGUI();
    ~TransformGUI();


};


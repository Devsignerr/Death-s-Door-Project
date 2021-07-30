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
private:
    void DrawGizmo(int _Mode);

public:
    TransformGUI();
    ~TransformGUI();


};


#pragma once
#include <Engine\CCamera.h>

class CCameraEx :
    public CCamera
{
public:
    virtual void finalupdate();
    void ObjectSelectToRay();
};


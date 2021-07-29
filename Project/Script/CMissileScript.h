#pragma once
#include <Engine\CScript.h>

class CMissileScript :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void update();

public:
    CLONE(CMissileScript);

public:
    CMissileScript();
    ~CMissileScript();
};


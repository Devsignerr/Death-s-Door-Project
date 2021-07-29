#pragma once
#include <Engine/CScript.h>

class CHsh :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void update() override;

public:
    CLONE(CHsh);

public:
    CHsh();
    ~CHsh();
};


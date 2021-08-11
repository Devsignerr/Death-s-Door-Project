#pragma once
#include "CState.h"
class CRunState :
    public CState
{
private:

private:
    virtual void update();

    virtual void Enter();
    virtual void Exit();

public:
    CRunState();
    virtual ~CRunState();
};


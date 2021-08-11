#pragma once
#include <Engine\CState.h>
class TPlayerRun :
    public CState
{
private:
    int a;
public:
    virtual void update();

public:
    virtual void Enter() {};
    virtual void Exit() {};
public:
    TPlayerRun();
    ~TPlayerRun();
};


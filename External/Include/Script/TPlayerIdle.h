#pragma once
#include <Engine\CState.h>
class TPlayerIdle :
    public CState
{
private:
    int a ;
public:
    virtual void update();
    virtual void lateupdate();
public:
    virtual void Enter(){};
    virtual void Exit(){};
public:
    TPlayerIdle();
    ~TPlayerIdle();
};


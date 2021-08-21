#pragma once
#include <Engine/CState.h>

class TPlayerRollSlash : public CState
{

private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRollSlash();
    ~TPlayerRollSlash();
};


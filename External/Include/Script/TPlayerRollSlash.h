#pragma once
#include <Engine/CState.h>

class TPlayerRollSlash : public CState
{
private:
    int m_ColOnOffCheck;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRollSlash();
    ~TPlayerRollSlash();
};


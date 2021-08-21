#pragma once
#include <Engine/CState.h>

class TIronChopAttack : public CState
{
private:
   void update() override;

   void Enter() override;
   void Exit() override;

public:
    TIronChopAttack();
    ~TIronChopAttack();
};


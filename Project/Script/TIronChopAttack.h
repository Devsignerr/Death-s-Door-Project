#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronChopAttack : public CState
{
private:
    CIronmaceScript* m_Script;
private:
   void update() override;

   void Enter() override;
   void Exit() override;

public:
    TIronChopAttack();
    ~TIronChopAttack();
};


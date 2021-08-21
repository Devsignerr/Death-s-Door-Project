#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronChopAttackEnd :
	public CState
{
private:
	bool m_SpinAttackCheck;
	CIronmaceScript* m_Script;
private:
	void update() override;
private:
	void Enter() override;
	void Exit() override;
public:
	TIronChopAttackEnd();
	~TIronChopAttackEnd();
};


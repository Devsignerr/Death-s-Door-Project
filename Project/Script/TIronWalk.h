#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronWalk :
	public CState
{
private:
	CIronmaceScript* m_Script;
private:
	void update() override;
private:
	void Enter() override;
	void Exit() override;
public:
	TIronWalk();
	~TIronWalk();
};


#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronWalk :
	public CState
{
private:
	CIronmaceScript* m_Script;
	bool			 m_bWalkSoundChange;
	float		     m_fSoundTimer;
private:
	void update() override;
private:
	void Enter() override;
	void Exit() override;
public:
	TIronWalk();
	~TIronWalk();
};


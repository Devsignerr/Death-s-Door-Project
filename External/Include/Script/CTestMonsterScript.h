#pragma once
#include "CMonsterScript.h"
class CTestMonsterScript : public CMonsterScript
{
public:
	void awake() override;
	void update() override;

private:
	void Idle() override;
	void Move() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void FinishAction() override;
	void Death() override;
public:
	CLONE(CTestMonsterScript);
public:
	CTestMonsterScript();
	~CTestMonsterScript();
};

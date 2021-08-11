#pragma once
#include "CMonsterScript.h"
class CBazookaScript :
	public CMonsterScript
{
private:
	float m_AimTime;
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
	void Jump() override;
	void Death() override;
private:
	void MeleeAttack() override;
	void LongDistanceAttack() override;
public:
	CLONE(CBazookaScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CBazookaScript();
	~CBazookaScript();
};


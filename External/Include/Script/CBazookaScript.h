#pragma once
#include "CMonsterScript.h"
class CBazookaScript :
	public CMonsterScript
{
private:
	float m_AimTime;
	float m_LongDistanceAttackRange;
	float m_MeleeAttackRange;
	float m_Speed;
	float m_ChaseRotSpeed;
	float m_AimRotSpeed;
	float m_AttackRotSpeed;
public:
	void awake() override;
	void update() override;
private:
	void Idle() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void Death() override;
private:
	void MeleeAttack() override;
	void LongDistanceAttack() override;
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:
	CLONE(CBazookaScript);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CBazookaScript();
	~CBazookaScript();
};


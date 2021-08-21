#pragma once
#include "CMonsterScript.h"
class CSkullScript :
	public CMonsterScript
{
private:
	float m_ChaseSpeed;
	float m_AttackMoveSpeed;
	float m_MeleeAttackRange;
	float m_MeleeAttack2Range;
	float m_LongDistanceAttackRange;
	float m_RotSpeed;
public:
	void awake() override;
	void update() override;
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

private:
	void LongDistanceAttack() override;
	void MeleeAttack() override;
private:
	void Idle() override;
	void Chase() override;
	void Attack() override;
	void Death() override;

private:
	bool	m_BulletLimit;
public:
	CLONE(CSkullScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CSkullScript();
	~CSkullScript();

};


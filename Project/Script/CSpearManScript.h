#pragma once
#include "CMonsterScript.h"
class CSpearManScript :
	public CMonsterScript
{
private:
	bool m_IsEvasion;
	float m_ChaseSpeed;
	float m_AttackMoveSpeed;
	float m_AttackRange;
	float m_Attack2Range;
	float m_EvasionRange;
	float m_AttackRotSpeed;
	float m_ChaseRotSpeed;
	float m_ReadyActionRotSpeed;
	float m_ReadyAction2RotSpeed;
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
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:
	CLONE(CSpearManScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CSpearManScript();
	~CSpearManScript();

};


#pragma once
#include "CMonsterScript.h"

class CGameObject;
class CTongueScript :
	public CMonsterScript
{
public:
	void awake() override;
	void update() override;
private:
	bool		 m_Left;
	int          m_SpinCount;
	Vec3		 m_AttackDir;
	float		 m_fTheta;
	bool		 m_BulletLimit;
	int			 m_ShieldPoint;
	float		 m_ChaseSpeed;
	float		 m_GuardRange;
	float        m_BackStepRange;
	float        m_ChaseRange;
	float		 m_SpinDownRange;
	float		 m_ChaseRotSpeed;
	float		 m_AttackRotSpeed;

private:
	void Idle() override;
	void Move() override;
	void Attack() override;
	void FinishAction() override;
	void SpecialAction() override;
	void Jump() override;
	void Death() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

private:
	void LongDistanceAttack() override;
	void MeleeAttack() override;
	void CalAttackDistance();
public:
	CLONE(CTongueScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CTongueScript();
	~CTongueScript();
};


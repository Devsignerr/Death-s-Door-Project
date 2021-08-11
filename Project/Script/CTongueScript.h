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
	CGameObject* m_RotObj;
	bool		 m_Left;
	int          m_Test;
	Vec3		 m_AttackDir;
	float		 m_fTheta;
	bool		 m_BulletLimit;
private:
	void Idle() override;
	void Move() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void FinishAction() override;
	void SpecialAction() override;
	void Jump() override;
	void Death() override;

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


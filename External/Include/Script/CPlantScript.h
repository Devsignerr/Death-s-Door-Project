#pragma once
#include "CMonsterScript.h"
class CPlantScript :
	public CMonsterScript
{
private:
	bool	m_BulletLimit;
	float	m_AttackDelayTime;
	float   m_RotSpeed;
	float   m_AttackRange;
public:
	void awake() override;
	void update() override;
private:
	void Idle() override;
	void Attack() override;
	void Death() override;

	void LongDistanceAttack() override;
	void CreateCol();

	void Test();
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:
	CLONE(CPlantScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CPlantScript();
	~CPlantScript();
};


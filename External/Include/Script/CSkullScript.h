#pragma once
#include "CMonsterScript.h"
class CSkullScript :
	public CMonsterScript
{
public:
	void awake() override;
	void update() override;

private:
	void LongDistanceAttack() override;
	void MeleeAttack() override;
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
	bool	m_BulletLimit;
public:
	CLONE(CSkullScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CSkullScript();
	~CSkullScript();

};


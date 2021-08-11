#pragma once
#include "CMonsterScript.h"
class CNailScript :
	public CMonsterScript
{
private:
	Vec3 m_AttackDir;
public:
	void awake() override;
	void update() override;
private:
	void Idle() override;
	void Move() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;  //
	void FinishAction() override;
	void Jump() override;
	void Death() override;
public:
	CLONE(CNailScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CNailScript();
	~CNailScript();
};


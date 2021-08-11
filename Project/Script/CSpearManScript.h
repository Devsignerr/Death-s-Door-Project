#pragma once
#include "CMonsterScript.h"
class CSpearManScript :
	public CMonsterScript
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
	void Jump() override;
	void Death() override;
public:
	CLONE(CSpearManScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CSpearManScript();
	~CSpearManScript();

};


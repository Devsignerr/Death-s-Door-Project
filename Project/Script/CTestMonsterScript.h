#pragma once
#include "CMonsterScript.h"
class CTestMonsterScript : public CMonsterScript
{
public:
	void awake() override;
	void update() override;
	float Test;
	Vec3  RevolutionPos;
	float fRadius;
	UINT  iSliceCount;
	float fTheta;
private:
	void Idle() override;
	void Move() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void FinishAction() override;
	void Death() override;
public:
	CLONE(CTestMonsterScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CTestMonsterScript();
	~CTestMonsterScript();
};

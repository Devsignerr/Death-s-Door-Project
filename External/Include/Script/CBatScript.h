#pragma once
#include "CMonsterScript.h"
class CBatScript :
	public CMonsterScript
{
private:
	float	m_fDegreeToRot;
	float   m_fRotRange;
	Vec3	m_AttackDir;
	float   m_fRadius;
	float   m_fTheta;
	float   m_fTestTheta; 
	Vec3	m_ChaseDir;
	float   m_Temp;

	//구불구불 이동용 임시 벡터 
	float   m_fTimetoCheckPlayerPos;
	float	m_fInternalRadianWithPlayer;
	bool	m_bPlayerMyLeft;

private:
	void CalAttackDistance();
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
	CLONE(CBatScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CBatScript();
	~CBatScript();
};


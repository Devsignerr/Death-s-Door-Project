#pragma once
#include "CMonsterScript.h"
class CBatScript :
	public CMonsterScript
{
private:
	float m_AttackRange;
	float m_Speed;
private:
	float	m_fDegreeToRot;
	float   m_fRotRange;
	Vec3	m_AttackDir;
	float   m_fTheta;

	//구불구불 이동용 임시 벡터 
	float   m_fTimetoCheckPlayerPos;
	float	m_fInternalRadianWithPlayer;
	bool	m_bPlayerMyLeft;

private:
	void CalAttackDistance();
	void CalChaseMove();
public:
	void awake() override;
	void update() override;
private:
	void Idle() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void Death() override;
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:
	CLONE(CBatScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CBatScript();
	~CBatScript();
};


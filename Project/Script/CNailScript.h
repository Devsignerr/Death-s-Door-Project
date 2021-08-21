#pragma once
#include "CMonsterScript.h"
class CNailScript :
	public CMonsterScript
{
private:
	Vec3  m_AttackDir;
	float m_NailAttackRange;
	float m_JumpAttackRange;
	float m_BackStepRange;
	float m_ChaseRotSpeed;
	float m_ReadyActionRotSpeed;
	float m_ChaseSpeed;
public:
	void awake() override;
	void update() override;
private:
	void Idle() override;
	void Chase() override;
	void ReadyAction() override;
	void Attack() override;
	void Jump() override;
	void Death() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

private:
	float m_fTheta;
	float m_fBackStepTheta;
	Vec3  m_BackStepDir;
	Vec3  m_ShiftAmount;
private:
	void CalAttackDistance();
	void CalBackStepDistance();

private:
	Vec3    m_Pos;
	float   m_Distance;
	float   m_Time;
	float   m_DestAttachTime;
	float	m_Gravity;
	float   m_VelocityX;
	float   m_VelocityY;
	float   m_VelocityZ;
	float   m_MaxHeight;
	float   m_MaxHeightTime;
private:
	void CalJumpAttackDistance();
	void JumpAttackStay();
	void ResetJumpAttackInfo();
	void ResetBackStepInfo();
public:
	CLONE(CNailScript);
public:
	CNailScript();
	~CNailScript();
};


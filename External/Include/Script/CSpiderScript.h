#pragma once
#include "CMonsterScript.h"
class CSpiderScript :
	public CMonsterScript
{
public:
	void awake() override;
	void update() override;
private:
	float	m_Speed;			// �ӵ�
	Vec3	m_ObjVector;		// ������Ʈ�� ���� ����
	Vec2	m_Power;			// �߻��ϴ� ��
	float	m_Rad;				// �߻��� ����
	float	m_AirResist;		// ���� ����
	float	m_Gravity;			// �߷�

	float   m_MoveTime;

private:
	float m_fTheta;
	Vec3	m_AttackDir;
	float m_BackMoveTime;
	bool  m_BackMoveCheck;
private:
	void CalAttackDistance();

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
	CLONE(CSpiderScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CSpiderScript();
	~CSpiderScript();
};


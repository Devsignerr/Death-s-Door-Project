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
	float	m_fTheta;
	Vec3	m_AttackDir;
	float	m_BackMoveTime;
	bool	m_BackMoveCheck;
	int		m_MoveCount;
private:
	void CalAttackDistance();

private:
	float m_MoveSpeed;
	float m_RotSpeed;
	float m_FrontMoveRange;
	float m_BackStepRange;
private:
	void Idle() override;
	void Move() override;
	void ReadyAction() override;
	void Attack() override;
	void Jump() override;
	void Death() override;
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

public:
	CLONE(CSpiderScript);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CSpiderScript();
	~CSpiderScript();
};


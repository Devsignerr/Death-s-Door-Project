#pragma once
#include "CMonsterScript.h"
class CSpiderScript :
	public CMonsterScript
{
public:
	void awake() override;
	void update() override;
private:
	float	m_Speed;			// 속도
	Vec3	m_ObjVector;		// 오브젝트의 진행 벡터
	Vec2	m_Power;			// 발사하는 힘
	float	m_Rad;				// 발사할 각도
	float	m_AirResist;		// 공기 저함
	float	m_Gravity;			// 중력

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


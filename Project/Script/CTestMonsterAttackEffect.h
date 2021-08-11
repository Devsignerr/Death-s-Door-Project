#pragma once
#include <Engine/CScript.h>
class CTestMonsterAttackEffect : public CScript
{
private:
	float	m_Speed;			// 속도
	Vec3	m_ObjVector;		// 오브젝트의 진행 벡터
	Vec2	m_Power;			// 발사하는 힘
	float	m_Rad;				// 발사할 각도
	float	m_AirResist;		// 공기 저함
	float	m_Gravity;			// 중력
	float   m_XRad;
	float   m_ZRad;
public:
	void SetRad(float _Degree) { m_Rad = _Degree * XM_PI / 180.0f; }
	void SetPower(Vec2 _Power) { m_Power = _Power; }
	void SetAirResist(float _AirResist) { m_AirResist = _AirResist; }
	void SetGravity(float _Gravity) { m_Gravity = _Gravity; }
	void SetSpeed(float _Speed) { m_Speed = _Speed; }
private:
	void SetVector();
	void Action();
public:
	void awake() override;
	void update() override;
public:

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CTestMonsterAttackEffect);
public:
	CTestMonsterAttackEffect();
	~CTestMonsterAttackEffect();
};


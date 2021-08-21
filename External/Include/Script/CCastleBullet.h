#pragma once
#include "CProjectile.h"
class CCastleBullet : public CProjectile
{
public:
	void awake() override;
	void start() override;
	void update() override;

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
	Vec3	m_TargetPos;

public:
	void SetMaxHeight(float _MaxHeight) { m_MaxHeight = _MaxHeight; }
	void SetMaxHeightTime(float _MaxHeightTime) { m_MaxHeightTime = _MaxHeightTime; }
	void SetTargetPos(Vec3 _Pos) { m_TargetPos = _Pos; }
public:

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CLONE(CCastleBullet);
public:
	CCastleBullet();
	~CCastleBullet();
};


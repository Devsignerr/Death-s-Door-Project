#pragma once
#include "CProjectile.h"
class CCrowBullet :
	public CProjectile
{
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

	Vec3    m_AwakeMoveDir;
private:
	bool AwakeMove();

private:
	float   m_fTimetoCheckPlayerPos;
	float	m_fInternalRadianWithPlayer;
	bool	m_bPlayerMyLeft;
public:
	void SetAwakeMoveDir(Vec3 _Dir) { m_AwakeMoveDir = _Dir; }
private:
	void GuidedMove();

public:
	void awake() override;
	void update() override;

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CCrowBullet);

public:
	CCrowBullet();
	~CCrowBullet();
};


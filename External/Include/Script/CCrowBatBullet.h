#pragma once
#include "CProjectile.h"
class CCrowBatBullet : public CProjectile
{
private:
	float m_AwakeMoveTime;
private:
	bool AwakeMove();
private:
	float   m_fTimetoCheckPlayerPos;
	float	m_fInternalRadianWithPlayer;
	bool	m_bPlayerMyLeft;
	bool    m_ChangeMove;
	float   m_fTheta;
	float	m_fLimitTheta;
	Vec3    m_LastPlayerpos;
private:
	bool GuidedMove();
	bool CircleMove();
public:
	void awake() override;
	void update() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CCrowBatBullet);

public:
	CCrowBatBullet();
	~CCrowBatBullet();
};


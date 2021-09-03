#pragma once
#include "CProjectile.h"

class CPlayerArrow :
    public CProjectile
{
private:
	Vec3	m_BulletDir;
	float	m_BulletSpeed;
public:
	void SetBulletDir(Vec3 _Dir) { m_BulletDir = _Dir; }
	void SetBulletSpeed(float _Speed) { m_BulletSpeed = _Speed; }
public:
	void update() override;
	void awake() override; 
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerArrow);

public:
	CPlayerArrow();
	~CPlayerArrow();
};


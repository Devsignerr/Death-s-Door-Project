#pragma once
#include "CProjectile.h"
class CPlantBullet : public CProjectile
{
private:
	Vec3 m_BulletDir;
public:
	void SetBulletDir(Vec3 _Dir) { m_BulletDir = _Dir; }
public:
	void update() override;


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CPlantBullet);

public:
	CPlantBullet();
	~CPlantBullet();
};


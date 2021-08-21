#pragma once
#include "CProjectile.h"
class CCrowEggBullet :
    public CProjectile
{
public:
	void update() override;


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CCrowEggBullet);

public:
	CCrowEggBullet();
	~CCrowEggBullet();
};


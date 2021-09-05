#pragma once
#include "CProjectile.h"
class CCastleLaser :
    public CProjectile
{

public:
	void awake() override;
	void update() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CCastleLaser();
	~CCastleLaser();


public:
	CLONE(CCastleLaser);
};


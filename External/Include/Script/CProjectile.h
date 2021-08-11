#pragma once
#include "CActorScript.h"
class CProjectile : public CActorScript
{
public:
	void update() override;

	CLONE(CProjectile);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CProjectile();
	~CProjectile();
};


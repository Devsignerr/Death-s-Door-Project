#pragma once
#include "CActorScript.h"

class CProjectile : public CActorScript
{
protected: 
	float m_fLifeTime;
	float m_fCurTime;
	bool  m_bActive;
	bool  m_bDestroyed;
	bool  m_bMemoryObj;

public:
	
	virtual void ActivateExplosionParticle() {}
	virtual void ReturnToMemoryPool();

	virtual void SetActive(bool _b) { m_bActive = _b; }

public:
	virtual void update() override;

	CLONE(CProjectile);

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CProjectile();
	virtual ~CProjectile();
};


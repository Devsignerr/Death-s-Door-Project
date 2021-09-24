#pragma once
#include "CProjectile.h"
class CTongueBullet : public CProjectile
{
private:
	float   m_fTimetoCheckPlayerPos;
	float	m_fInternalRadianWithPlayer;
	bool	m_bPlayerMyLeft;
public:
	void update() override;
	void awake() override;

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CTongueBullet);


	void OnCollisionEnter(CGameObject* _pOther) override;

public:
	CTongueBullet();
	~CTongueBullet();
};


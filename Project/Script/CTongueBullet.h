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


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CTongueBullet);

public:
	CTongueBullet();
	~CTongueBullet();
};


#pragma once
#include "CEffectScript.h"

class CExplosionParticle:
	public  CEffectScript 
{
private:
    int   m_iMaxCount;

public:
    virtual void awake();
    virtual void update();

public:
    virtual void SetActive(bool _b) override;
    void Reset();

public:
    CExplosionParticle();
    virtual ~CExplosionParticle();

public:
    CLONE(CExplosionParticle);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

};


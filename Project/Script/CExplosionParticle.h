#pragma once
#include "CEffectScript.h"

class CExplosionParticle:
	public  CEffectScript 
{
private:
    int                m_iMaxCount;
    EXPLOSION_PTC_TYPE m_eType;
    bool               m_bMemoryObj;

public:
    bool  IsMemoryObj() { return m_bMemoryObj; }
    void SetMemoryObj(bool _b) { m_bMemoryObj = _b; }

    EXPLOSION_PTC_TYPE GetExploPTCType() { return m_eType; }
    void SetExploPTCType(EXPLOSION_PTC_TYPE _Type) { m_eType = _Type; }

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


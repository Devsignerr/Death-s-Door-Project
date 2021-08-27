#pragma once
#include "CEffectScript.h"

enum class SLASH_TYPE
{
    ROLL,
    HORIZON,
};

class CSlashEffect :
    public CEffectScript
{
private:
    float   m_fBurnRatio;
    Vec3    m_vBurnColor;
    SLASH_TYPE m_eType;

public:
    virtual void update();


public:   
    virtual void SetActive(bool _b);

public:
    CSlashEffect();
    virtual ~CSlashEffect();

public:
    CLONE(CSlashEffect);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
};


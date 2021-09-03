#pragma once
#include "CEffectScript.h"
class CFireDamageParticle :
    public CEffectScript
{
private:
    CGameObject* m_pTarget;

public:
    virtual void awake();
    virtual void update();

public:
    // 타겟으로 설정된 몬스터를 따라다니며 불 이펙트 활성화 
    void SetTarget(CGameObject* _Target) { m_pTarget = _Target; }
    virtual void SetActive(bool _b) override;

public:
    CFireDamageParticle();
    virtual ~CFireDamageParticle();

public:
    CLONE(CFireDamageParticle);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
};


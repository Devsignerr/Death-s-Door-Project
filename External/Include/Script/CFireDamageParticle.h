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
    // Ÿ������ ������ ���͸� ����ٴϸ� �� ����Ʈ Ȱ��ȭ 
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


#pragma once
#include <Engine\CScript.h>

class CEffectScript :
    public CScript
{
protected:
    bool               m_bRepeat;

public:
    void SetRepeat(bool _b) { m_bRepeat = _b; }

protected:
    bool    m_bActive;
    float   m_fCurTime;
    float   m_fLifeTime;

public:
    virtual void SetActive(bool _b) { m_bActive = _b; }

public:
    virtual void update() {}

public:
    virtual void OnCollisionEnter(CGameObject* _pOther) {};
    virtual void OnCollision(CGameObject* _pOther) {};
    virtual void OnCollisionExit(CGameObject* _pOther) {};

public:
    CEffectScript();
    virtual ~CEffectScript();

public:
    CLONE(CEffectScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
};


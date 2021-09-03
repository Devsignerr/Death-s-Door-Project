#pragma once
#include "CEffectScript.h"
class CAttackImpactScript :
    public CEffectScript
{
private:
    Vec3 m_vDir;
    float m_fMoveSpeed;
    float m_fScale;
public:
    void SetDir(Vec3 _Dir) { m_vDir = _Dir; }
    void SetScale(int _zlength) { m_fScale = _zlength; }
    void SetSpeed(int _Speed) { m_fMoveSpeed = _Speed; }
public:
    virtual void SetActive(bool _b) override;

public:
    virtual void update();

public:
    CAttackImpactScript();
    virtual ~CAttackImpactScript();

public:
    CLONE(CAttackImpactScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
};


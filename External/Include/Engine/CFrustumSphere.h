#pragma once
#include "CComponent.h"
class CFrustumSphere :
    public CComponent
{
private:
    Vec3        m_vOffsetPos;
    float       m_fRadius;
public:
    virtual void awake();

    virtual void update();
    virtual void finalupdate();

public:
    Vec3 GetOffSetPos() { return m_vOffsetPos; }
    float GetRadius() { return m_fRadius; }

    void SetOffsetPos(Vec3 _Pos) { m_vOffsetPos = _Pos; }
    void SetRadius(float _Radius) { m_fRadius = _Radius; }

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    CLONE(CFrustumSphere);

public:
    CFrustumSphere();
    CFrustumSphere(CFrustumSphere& _Origin );
    ~CFrustumSphere();

};


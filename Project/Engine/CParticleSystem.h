#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;


class CParticleSystem :
    public CComponent
{
private:
    CStructuredBuffer* m_pParticleBuffer;

    Ptr<CMesh>                  m_pMesh;
    Ptr<CMaterial>              m_pMtrl;
    Ptr<CParticleUpdateShader>  m_pUpdateShader;
    Ptr<CTexture>               m_pTex;
    Ptr<CTexture>               m_pGateTex;

    int                         m_iMaxCount;    // 파티클 최대 수용량
    int                         m_iAliveCount;  // 이번에 활성화 시킬 파티클 개수
    float                       m_fMinLifeTime;
    float                       m_fMaxLifeTime;
    float                       m_fMinSpeed;
    float                       m_fMaxSpeed;
    float                       m_fFrequency;   // 파티클 생성 주기
    float                       m_fAccTime;     // 누적시간

    Vec4                        m_vCreateRange; // 파티클 생성 범위

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;
    Vec3                        m_vStartScale;
    Vec3                        m_vEndScale;

public:
    virtual void awake();
    virtual void update() {}
    virtual void finalupdate();
    void render();

public:
    void SetTexture(Ptr<CTexture> _Tex) { m_pTex = _Tex; }
    void SetGateTexture(Ptr<CTexture> _Tex) { m_pGateTex = _Tex; }
    void SetMaxParticleCount(int _iMaxCount);
    void SetMaxCount(int _count) { m_iMaxCount = _count; }
    void SetAliveCount(int _count) { m_iAliveCount = _count; }
    void SetMinLifeTime(float _time) { m_fMinLifeTime = _time; }
    void SetMaxLifeTime(float _time) { m_fMaxLifeTime = _time; }
    void SetMinSpeed(float _speed) { m_fMinSpeed = _speed; }
    void SetMaxSpeed(float _speed) { m_fMaxSpeed = _speed; }
    void SetFrequency(float _frequency) { m_fFrequency = _frequency; }
    void SetAccTime(float _time) { m_fAccTime = _time; }
    void SetCreateRange(Vec4 _range) { m_vCreateRange = _range; }
    void SetStartColor(Vec4 _Color) { m_vStartColor = _Color; }
    void SetEndColor(Vec4 _Color) { m_vEndColor = _Color; }
    void SetStartScale(Vec3 _Scale) { m_vStartScale = _Scale; }
    void SetEndScale(Vec3 _Scale) { m_vEndScale = _Scale; }

    int     GetMaxCount() { return m_iMaxCount; }
    int     GetAliveCount() { return m_iAliveCount; }
    float   GetMinLifeTime() { return m_fMinLifeTime; }
    float   GetMaxLifeTime() { return m_fMaxLifeTime; }
    float   GetMinSpeed() { return m_fMinSpeed; }
    float   GetMaxSpeed() { return m_fMaxSpeed; }
    float   GetFrequency() { return m_fFrequency; }
    float   GetAccTime() { return m_fAccTime; }
    Vec4    GetCreateRange() { return m_vCreateRange; }
    Vec4    GetStartColor() { return m_vStartColor; }
    Vec4    GetEndColor() { return m_vEndColor; }
    Vec3    GetStartScale() { return m_vStartScale; }
    Vec3    GetEndScale() { return m_vEndScale; }

    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    CLONE(CParticleSystem);

public:
    CParticleSystem();
    ~CParticleSystem();
};


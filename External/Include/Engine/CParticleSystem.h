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
    CStructuredBuffer*          m_pParticleBuffer;

    Ptr<CMesh>                  m_pMesh;
    Ptr<CMaterial>              m_pMtrl;
    Ptr<CParticleUpdateShader>  m_pUpdateShader;
    Ptr<CTexture>               m_pTex;

    int                         m_iMaxCount;    // ��ƼŬ �ִ� ���뷮
    int                         m_iAliveCount;  // �̹��� Ȱ��ȭ ��ų ��ƼŬ ����
    float                       m_fMinLifeTime;
    float                       m_fMaxLifeTime;
    float                       m_fMinSpeed;
    float                       m_fMaxSpeed;
    float                       m_fFrequency;   // ��ƼŬ ���� �ֱ�
    float                       m_fAccTime;     // �����ð�

    Vec4                        m_vCreateRange; // ��ƼŬ ���� ����

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;
    Vec3                        m_vStartScale;
    Vec3                        m_vEndScale;

    SHADER_POV                  m_ePOV;
    PARTICLE_TYPE               m_eType;

    bool                        m_bRepeat; //��ƼŬ maxī��Ʈ �޼��� �ݺ���ų�� ����
    int                         m_iSlow; //������ ������ 

    int                         m_iAccLiveCount;
    int                         m_iMaxLiveCount; //�ѹ� ����� ����� �����ų���� 

    bool                        m_bDestroy;
public:
    void Activate(bool _b);
    void Reset() { m_iAccLiveCount = 0; m_fAccTime = 0.f; }
    void LateDestroy();

public:
    virtual void awake();
    virtual void update() {}
    virtual void finalupdate();
    void render();

public:
    void Destroy() { m_bDestroy = true; m_fAccTime = 0.f; }
    void  SetSlow(int _i) { m_iSlow = _i; }
    void SetRepeat(bool _b) { m_bRepeat = _b; }
    bool IsRepeat() { return m_bRepeat; }

    void SetAccLiveCount(int _count) { m_iAccLiveCount = _count; }
    void SetAliveCount(int _count) { m_iAliveCount = _count; }
    void SetTexture(Ptr<CTexture> _Tex) { m_pTex = _Tex; }
    void SetMaxParticleCount(int _iMaxCount);
    void SetMaxCount(int _count) { m_iMaxCount = _count; }
    void SetMaxLiveCount(int _count) { m_iMaxLiveCount = _count; }

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

    void SetType(PARTICLE_TYPE _Type) { m_eType = _Type; }

    //�ѹ��� ����� ������ų��
    int     GetSlow() { return m_iSlow; }
    int     GetAccLiveCount() { return m_iAccLiveCount; }
    int     GetMaxLiveCount() { return m_iMaxLiveCount; }
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
    PARTICLE_TYPE GetType() { return m_eType; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
    Ptr<CTexture> GetTexture() { return m_pTex; }
public:
    SHADER_POV GetPOV() { return m_ePOV; }
    void SetPOV(SHADER_POV _Pov) { m_ePOV = _Pov; }

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    CLONE(CParticleSystem);

public:
    CParticleSystem();
    CParticleSystem(CParticleSystem& _origin);
    ~CParticleSystem();
};


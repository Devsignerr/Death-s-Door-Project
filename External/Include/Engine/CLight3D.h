#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CMaterial.h"

class CGameObject;

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_info;

    Ptr<CMesh>      m_pMesh; // Lighting 볼륨 메쉬
    Ptr<CMaterial>  m_pMtrl; // Lighting 메테리얼

    int             m_iLightIdx; // RenderMgr 에 등록 된 인덱스

    CGameObject*     m_pCamObj;

// =============================================================================================
    Ptr<CTexture>   m_StaticShadowTex; 
public:
    void SetStaticShadowTex(Ptr<CTexture> _ShadowTex) { m_StaticShadowTex = _ShadowTex; }
    Ptr<CTexture> GetStaticShadowTex() { return m_StaticShadowTex; }
// =============================================================================================

public:
    virtual void finalupdate();
    void render();
    void render_shadowmap();

public:
    void SetLightType(LIGHT_TYPE _eType);
    void SetDiffusePow(Vec3 _vDiffusePow) { m_info.color.vDiff = _vDiffusePow; }
    void SetSpecPow(Vec3 _vSpecPow) { m_info.color.vSpec = _vSpecPow; }
    void SetAmbPow(Vec3 _vAmbPow) { m_info.color.vAmb = _vAmbPow; }
    void SetRange(float _fRange);
    void SetAngle(float _fAngle) { m_info.fAngle = _fAngle; }

    const tLightInfo& GetInfo(){return m_info;}
    void SetLightInfo(tLightInfo Info);

    CGameObject* GetLightCam() { return m_pCamObj; }
public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    CLONE(CLight3D);

public:
    CLight3D();
    CLight3D(const CLight3D&);
    ~CLight3D();
};


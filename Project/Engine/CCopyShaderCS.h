#pragma once
#include "CComputeShader.h"
class CCopyShaderCS :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_pDestTex;
    Ptr<CTexture>   m_pSrcTex;
    Ptr<CTexture>   m_StaticShadowTex;

public:
    void SetDestTex(Ptr<CTexture> _pDestTex) { m_pDestTex = _pDestTex; }
    void SetSrcTex(Ptr<CTexture> _pSrcTex) { m_pSrcTex = _pSrcTex; }
    void SetStaticShadowTex(Ptr<CTexture> _StaticShadowTex) { m_StaticShadowTex = _StaticShadowTex; }

public:
    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();


};


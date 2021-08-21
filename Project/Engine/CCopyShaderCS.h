#pragma once
#include "CComputeShader.h"
class CCopyShaderCS :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_pDestTex;
    Ptr<CTexture>   m_pSrcTex;
    //Ptr<CTexture>   m_StaticShadowTex;
    COPY_TYPE       m_eCopyType;

public:
    void SetDestTex(Ptr<CTexture> _pDestTex) { m_pDestTex = _pDestTex; }
    void SetSrcTex(Ptr<CTexture> _pSrcTex) { m_pSrcTex = _pSrcTex; }
    //void SetStaticShadowTex(Ptr<CTexture> _StaticShadowTex) { m_StaticShadowTex = _StaticShadowTex; }

    void SetCopyType(COPY_TYPE _eType) 
    {
        m_eCopyType = _eType;
    }

public:
    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();


};


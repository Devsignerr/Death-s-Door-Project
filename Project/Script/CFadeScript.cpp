#include "pch.h"
#include "CFadeScript.h"

#include <Engine/CKeyMgr.h>

void CFadeScript::Fade_In()
{
    m_FadeType = FADETYPE::FADE_IN;
    m_Time = 0.0f;
    m_FadeInOut = false;
}

void CFadeScript::Fade_Out()
{
    m_FadeType = FADETYPE::FADE_OUT;
    m_Time = 0.0f;
    m_FadeInOut = false;
}


void CFadeScript::awake()
{
    m_FadeType = FADETYPE::FADE_IN;
    m_Time = 0.0f;
    m_FadeInOut = false;
    m_FadeTime = XM_PI / 1.0;

    m_SceneChangeCheck = false;
}

void CFadeScript::update()
{
    
    m_Time += fDT * m_FadeTime;

    if (false == m_FadeInOut)
    {
        if (false == m_SceneChangeCheck)
        {
            m_SceneChangeCheck = true;
            m_FadeType = FADETYPE::FADE_IN;
            m_Time = 0.0f;
            m_FadeInOut = false;
            m_FadeTime = XM_PI / 2.0;
        }
        else
        {
            if (m_Time > XM_PI / 2.0f)
            {
                m_FadeInOut = true;
                m_Time = XM_PI / 2.0f;
            }
        }

        if (FADETYPE::FADE_OUT == m_FadeType)
        {
            Fade_out(m_Time);
        }
        else if (FADETYPE::FADE_IN == m_FadeType)
        {
            Fade_in(m_Time);
        }
    }

}

void CFadeScript::Fade_in(float _FadeTime)
{
    int FadeType = 1;
    float Time = _FadeTime;

    MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &FadeType);
    MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &Time);

}

void CFadeScript::Fade_out(float _FadeTime)
{
    int FadeType = 0;
    float Time = _FadeTime;

    MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &FadeType);
    MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &Time);
}

CFadeScript::CFadeScript()
    : CScript((UINT)SCRIPT_TYPE::FADESCRIPT)
    , m_FadeType(CFadeScript::FADETYPE::END)
    , m_FadeInOut(false)
    , m_Time(0.0f)
    , m_FadeTime(XM_PI / 2.0)
    , m_SceneChangeCheck(false)
{
    //SetFadeTime(3.0f);
}

CFadeScript::~CFadeScript()
{

}

void CFadeScript::SaveToScene(FILE* _pFile)
{
    CScript::SaveToScene(_pFile);
}

void CFadeScript::LoadFromScene(FILE* _pFile)
{
    CScript::LoadFromScene(_pFile);
}
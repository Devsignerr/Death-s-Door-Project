#include "pch.h"
#include "CFadeScript.h"

#include <Engine/CKeyMgr.h>

CFadeScript::FADETYPE CFadeScript::m_FadeType = CFadeScript::FADETYPE::END;
bool CFadeScript::m_FadeInOut = false;
float CFadeScript::m_Time = 0.0f;
float CFadeScript::m_FadeTime = XM_PI / 2.0;

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
}

void CFadeScript::update()
{

	/*if (KEY_HOLD(KEY_TYPE::NUM_3))
	{
		m_FadeType = FADETYPE::FADE_IN;
		m_Time = 0.0f;
		m_FadeInOut = false;
	}
	if (KEY_TAP(KEY_TYPE::NUM_4))
	{
		m_FadeType = FADETYPE::FADE_OUT;
		m_Time = 0.0f;
		m_FadeInOut = false;
	}*/
	m_Time += fDT * m_FadeTime;

	if (m_Time > XM_PI / 2.0f)
		m_FadeInOut = true;

	if (false == m_FadeInOut)
	{
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
{
	SetFadeTime(3.0f);
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

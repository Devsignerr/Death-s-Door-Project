#include "pch.h"
#include "CEffectScript.h"

CEffectScript::CEffectScript():
	CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT),
	m_bActive(false)
{
}

CEffectScript::~CEffectScript()
{
}


void CEffectScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CEffectScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

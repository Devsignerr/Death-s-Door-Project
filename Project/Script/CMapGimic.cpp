#include "pch.h"
#include "CMapGimic.h"

void CMapGimic::awake()
{
}

void CMapGimic::update()
{
}

CMapGimic::CMapGimic()
	: CScript((UINT)SCRIPT_TYPE::MAPGIMIC)
	, m_GimicType(GIMICTYPE::END)
{
}

CMapGimic::~CMapGimic()
{
}

void CMapGimic::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CMapGimic::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

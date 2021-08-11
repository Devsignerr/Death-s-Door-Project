#include "pch.h"
#include "CProjectile.h"

void CProjectile::update()
{
}

CProjectile::CProjectile()
{
	m_iScriptType = (int)SCRIPT_TYPE::PROJECTILE;
}

CProjectile::~CProjectile()
{
}

void CProjectile::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CProjectile::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}

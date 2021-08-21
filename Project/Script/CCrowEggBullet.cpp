#include "pch.h"
#include "CCrowEggBullet.h"
void CCrowEggBullet::update()
{
}

void CCrowEggBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCrowEggBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

CCrowEggBullet::CCrowEggBullet()
{
	m_iScriptType = (int)SCRIPT_TYPE::CROWEGGBULLET;
}

CCrowEggBullet::~CCrowEggBullet()
{
}
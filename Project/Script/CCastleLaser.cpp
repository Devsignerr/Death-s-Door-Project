#include "pch.h"
#include "CCastleLaser.h"


CCastleLaser::CCastleLaser()
{
	m_iScriptType = (UINT)SCRIPT_TYPE::CASTLELASER;
}

CCastleLaser::~CCastleLaser()
{
}


void CCastleLaser::awake()
{
}

void CCastleLaser::update()
{
}


void CCastleLaser::OnCollisionEnter(CGameObject* _pOther)
{
}

void CCastleLaser::OnCollision(CGameObject* _pOther)
{
}

void CCastleLaser::OnCollisionExit(CGameObject* _pOther)
{
}



void CCastleLaser::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCastleLaser::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

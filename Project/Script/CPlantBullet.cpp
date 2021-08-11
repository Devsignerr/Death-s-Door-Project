#include "pch.h"
#include "CPlantBullet.h"

void CPlantBullet::update()
{
	Vec3 Pos = Transform()->GetLocalPos();
	
	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.x;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.z;
	
	Transform()->SetLocalPos(Pos);
}

CPlantBullet::CPlantBullet()
	: m_BulletDir{}
{
	m_iScriptType = (int)SCRIPT_TYPE::PLANTBULLET;
}

CPlantBullet::~CPlantBullet()
{
}

void CPlantBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlantBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

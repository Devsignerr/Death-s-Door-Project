#include "pch.h"
#include "CPlayerArrow.h"

void CPlayerArrow::update()
{
	CProjectile::update();

	Vec3 Pos = Transform()->GetLocalPos();
	Pos += m_BulletDir * m_BulletSpeed * fDT;

	Transform()->SetLocalPos(Pos);
}

void CPlayerArrow::OnCollisionEnter(CGameObject* _pOther)
{
}

void CPlayerArrow::OnCollision(CGameObject* _pOther)
{
}

void CPlayerArrow::OnCollisionExit(CGameObject* _pOther)
{
}

CPlayerArrow::CPlayerArrow()
	: m_BulletDir{}
	, m_BulletSpeed(5000.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::PLAYERARROW;
	m_fLifeTime = 2.f;
	m_bMemoryObj = false;
}

CPlayerArrow::~CPlayerArrow()
{
}

void CPlayerArrow::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlayerArrow::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}
#include "pch.h"
#include "CPlayerBomb.h"

void CPlayerBomb::update()
{
	CProjectile::update();

	Vec3 Pos = Transform()->GetLocalPos();
	Pos += m_BulletDir * m_BulletSpeed * fDT;

	Transform()->SetLocalPos(Pos);
}

void CPlayerBomb::ActivateExplosionParticle()
{



}

void CPlayerBomb::OnCollisionEnter(CGameObject* _pOther)
{
	m_bDestroyed = true;
}

void CPlayerBomb::OnCollision(CGameObject* _pOther)
{
}

void CPlayerBomb::OnCollisionExit(CGameObject* _pOther)
{
}

CPlayerBomb::CPlayerBomb()
	: m_BulletDir{}
	, m_BulletSpeed(5000.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::PLAYERBOMB;
}

CPlayerBomb::~CPlayerBomb()
{
}

void CPlayerBomb::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlayerBomb::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}
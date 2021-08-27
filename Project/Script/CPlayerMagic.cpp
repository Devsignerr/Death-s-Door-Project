#include "pch.h"
#include "CPlayerMagic.h"

void CPlayerMagic::update()
{
	CProjectile::update();

	Vec3 Pos = Transform()->GetLocalPos();
	Pos += m_BulletDir * m_BulletSpeed * fDT;

	Transform()->SetLocalPos(Pos);
}

void CPlayerMagic::OnCollisionEnter(CGameObject* _pOther)
{
}

void CPlayerMagic::OnCollision(CGameObject* _pOther)
{
}

void CPlayerMagic::OnCollisionExit(CGameObject* _pOther)
{
}

CPlayerMagic::CPlayerMagic()
	: m_BulletDir{}
	, m_BulletSpeed(5000.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::PLAYERMAGIC;
	m_bMemoryObj = false;
	m_fLifeTime = 2.0f;
}

CPlayerMagic::~CPlayerMagic()
{
}

void CPlayerMagic::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlayerMagic::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

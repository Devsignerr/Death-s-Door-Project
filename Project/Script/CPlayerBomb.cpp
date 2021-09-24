#include "pch.h"
#include "CPlayerBomb.h"
#include "CMemoryPoolScript.h"
#include "CExplosionParticle.h"
#include "CCameraScript.h"

#include <Engine/CParticleSystem.h>

void CPlayerBomb::awake()
{
	CProjectile::awake();
	CreateCollider((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, Vec3(100.f, 100.f, 100.f), Vec3(0.f, 0.f, 0.f));
}

void CPlayerBomb::update()
{
	if (m_bActive)
	{
		m_fCurTime += fDT;

		if (m_fCurTime > m_fLifeTime)
		{
			m_fCurTime = 0.f;

			GetObj()->ParticleSystem()->Activate(false);
			GetObj()->SetAllColliderActive(false);
			Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			SetActive(false);
			m_bDestroyed = false;
			GetObj()->ParticleSystem()->Destroy();
		}

		else if (m_bDestroyed)
		{
			m_fCurTime = 0.f;

			ActivateExplosionParticle();

			GetObj()->ParticleSystem()->Activate(false);
			GetObj()->SetAllColliderActive(false);
			Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			SetActive(false);
			m_bDestroyed = false;
			GetObj()->ParticleSystem()->Destroy();
		}

		Vec3 Pos = Transform()->GetLocalPos();
		Pos += m_BulletDir * m_BulletSpeed * fDT;

		Transform()->SetLocalPos(Pos);

	}
}


void CPlayerBomb::OnCollisionEnter(CGameObject* _pOther)
{
	Play_Sound(L"BombFullyCharged", 1, true, 0.5f);
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
	m_fLifeTime = 2.f;
	m_bMemoryObj = false;
	m_eType = EXPLOSION_PTC_TYPE::PLAYER_BOMB;
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
#include "pch.h"
#include "CPlayerBomb.h"
#include "CMemoryPoolScript.h"
#include "CExplosionParticle.h"
#include "CCameraScript.h"

#include <Engine/CParticleSystem.h>

void CPlayerBomb::awake()
{
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

void CPlayerBomb::ActivateExplosionParticle()
{
	CGameObject* pExploParticle = CMemoryPoolScript::GetExplosionPTC();

	if (nullptr == pExploParticle)
		return;

	((CExplosionParticle*)pExploParticle->GetScript())->Reset();

	//폭발 파티클의 소환 위치는 호출당시의 투사체파티클의 위치
	pExploParticle->Transform()->SetLocalPos(Transform()->GetLocalPos());
}

void CPlayerBomb::OnCollisionEnter(CGameObject* _pOther)
{
	CCameraScript::SetCameraShake(0.3f, 100.f, 5.f);

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
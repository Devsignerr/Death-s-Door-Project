#include "pch.h"
#include "CPlayerMagic.h"
#include "CMemoryPoolScript.h"
#include "CFireDamageParticle.h"
#include "CCameraScript.h"
#include <Engine/CParticleSystem.h>

void CPlayerMagic::awake()
{
	CreateCollider((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, Vec3(100.f, 100.f, 100.f), Vec3(0.f, 0.f, 0.f));
}

void CPlayerMagic::update()
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

		Vec3 Pos = Transform()->GetLocalPos();
		Pos += m_BulletDir * m_BulletSpeed * fDT;

		Transform()->SetLocalPos(Pos);
	}
}

void CPlayerMagic::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* pFireDamagePTC = CMemoryPoolScript::GetFireDamagePTC();

	if (nullptr == pFireDamagePTC)
		return;

	((CFireDamageParticle*)pFireDamagePTC->GetScript())->SetTarget(_pOther);

	CCameraScript::SetCameraShake(0.2f, 100.f, 3.f);

	ActivateImpactParticle(Transform()->GetLocalPos(),m_BulletDir, 15 ,9);
	
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

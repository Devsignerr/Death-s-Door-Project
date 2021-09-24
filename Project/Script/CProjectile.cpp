#include "pch.h"
#include "CProjectile.h"
#include "CMemoryPoolScript.h"
#include "CExplosionParticle.h"
#include "CCameraScript.h"

#include <Engine/CParticleSystem.h>
#include <Engine/CCollider3D.h>
#include <Engine/CEventMgr.h>


CProjectile::CProjectile():
	m_bMemoryObj(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::PROJECTILE;
}

CProjectile::~CProjectile()
{
}


void CProjectile::ActivateExplosionParticle()
{
	CGameObject* pExploParticle = nullptr;

	if (EXPLOSION_PTC_TYPE::PLAYER_BOMB == m_eType)
	{
		pExploParticle = CMemoryPoolScript::GetExplosionPTC();
		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::PLAYER_BOMB);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(true);

		CCameraScript::SetCameraShake(0.3f, 100.f, 5.f);
	}
	  

	else if (EXPLOSION_PTC_TYPE::BAZOOKA_EXPLO_PTC == m_eType)
	{
		pExploParticle = IntanciatePrefab(L"BAZOOKA_EXPLO_PTC", (UINT)LAYER_TYPE::MONSTER_EFFECT);
		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::BAZOOKA_EXPLO_PTC);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);

		CCameraScript::SetCameraShake(0.1f, 100.f, 3.f);
	}

	else if (EXPLOSION_PTC_TYPE::CASTLE_EXPLO_PTC == m_eType)
	{
		pExploParticle = IntanciatePrefab(L"CASTLE_EXPLO_PTC", (UINT)LAYER_TYPE::BOSS_EFFECT);

		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::CASTLE_EXPLO_PTC);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);

		pExploParticle->ParticleSystem()->SetPaperburnPTC(true);

		CCameraScript::SetCameraShake(0.1f, 100.f, 3.f);
	}

	else if (EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC == m_eType)
	{
		pExploParticle = IntanciatePrefab(L"SKULL_EXPLO_PTC", (UINT)LAYER_TYPE::MONSTER_EFFECT);
		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);

		CCameraScript::SetCameraShake(0.1f, 100.f, 3.f);
	}

	else if (EXPLOSION_PTC_TYPE::TONGUE_EXPLO_PTC == m_eType)
	{
		pExploParticle = IntanciatePrefab(L"BAZOOKA_EXPLO_PTC", (UINT)LAYER_TYPE::MONSTER_EFFECT);

		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);

		pExploParticle->ParticleSystem()->SetStartScale(Vec3(200.f, 200.f, 200.f));
		pExploParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 1.f,1.f));

		CCameraScript::SetCameraShake(0.1f, 100.f, 3.f);
	}
		

	if (nullptr == pExploParticle)
		return;

	((CExplosionParticle*)pExploParticle->GetScript())->Reset();

	//폭발 파티클의 소환 위치는 호출당시의 투사체파티클의 위치
	pExploParticle->Transform()->SetLocalPos(Transform()->GetLocalPos());
}


void CProjectile::ReturnToMemoryPool()
{
	CMemoryPoolScript::ReturnObj(GetObj());
}

void CProjectile::awake()
{
	GetObj()->SetAlldynamicShadow(false);
}

void CProjectile::update()
{
	if (m_bActive)
	{
		m_fCurTime += fDT;

		if (m_fCurTime > m_fLifeTime)
		{
			m_fCurTime = 0.f;

			if(m_bMemoryObj)
				ReturnToMemoryPool();
			else
			{
				DeleteObject(GetObj());
			}
		}

		if (m_bDestroyed)
		{
			m_fCurTime = 0.f;

			ActivateExplosionParticle();

			if(m_bMemoryObj)
				ReturnToMemoryPool();

			else 
			{
				DeleteObject(GetObj());
			}
		}
	}
}


void CProjectile::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CProjectile::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}

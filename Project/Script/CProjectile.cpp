#include "pch.h"
#include "CProjectile.h"
#include "CMemoryPoolScript.h"
#include "CExplosionParticle.h"

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
	}
	  

	else if (EXPLOSION_PTC_TYPE::BAZOOKA_EXPLO_PTC == m_eType)
	{
		pExploParticle = IstanciatePrefab(L"BAZOOKA_EXPLO_PTC", (UINT)LAYER_TYPE::MONSTER_EFFECT);
		((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::BAZOOKA_EXPLO_PTC);
		((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);
	}

	else if (EXPLOSION_PTC_TYPE::CASTLE_EXPLO_PTC == m_eType)
	{
		//pExploParticle = IstanciatePrefab(L"CASTLE_EXPLO_PTC", (UINT)LAYER_TYPE::BOSS_EFFECT);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::CASTLE_EXPLO_PTC);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);
	}

	else if (EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC == m_eType)
	{
		//pExploParticle = IstanciatePrefab(L"SKULL_EXPLO_PTC", (UINT)LAYER_TYPE::MONSTER_EFFECT);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetExploPTCType(EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetMemoryObj(false);
		//((CExplosionParticle*)pExploParticle->GetScript())->SetActive(true);
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

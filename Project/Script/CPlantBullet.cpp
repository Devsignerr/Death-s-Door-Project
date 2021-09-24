#include "pch.h"
#include "CPlantBullet.h"
#include "CCameraScript.h"

#include <Engine/CLight3D.h>
#include <Engine/CParticleSystem.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void CPlantBullet::update()
{
	Vec3 Pos = Transform()->GetLocalPos();
	
	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.x * 1000.0f;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.z * 1000.0f;
	
	Transform()->SetLocalPos(Pos);
}

void CPlantBullet::awake()
{
	CProjectile::awake();
}

void CPlantBullet::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIndex() == (UINT)LAYER_TYPE::PLAYER_COL)
	{
		CCameraScript::SetCameraShake(0.2f, 100.f, 4.f);

		ActivateExplosionParticle();

		GetObj()->SetAllColliderActive(false);
		SetActive(false);
		m_bDestroyed = false;
		GetObj()->ParticleSystem()->Destroy();
	}
}

CPlantBullet::CPlantBullet()
	: m_BulletDir{}

{
	m_iScriptType = (int)SCRIPT_TYPE::PLANTBULLET;
	m_eType = EXPLOSION_PTC_TYPE::SKULL_EXPLO_PTC;
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

#include "pch.h"
#include "CPlayerArrow.h"
#include "CCameraScript.h"

void CPlayerArrow::update()
{
	if (m_bActive)
	{
		m_fCurTime += fDT;

		if (m_fCurTime > m_fLifeTime)
		{
			m_fCurTime = 0.f;

			GetObj()->SetAllMeshrenderActive(false);
			GetObj()->SetAllColliderActive(false);
			Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			SetActive(false);
			m_bDestroyed = false;
			DeleteObject(GetObj());
		}

		else if (m_bDestroyed)
		{
			m_fCurTime = 0.f;

			GetObj()->SetAllMeshrenderActive(false);
			GetObj()->SetAllColliderActive(false);
			Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			SetActive(false);
			m_bDestroyed = false;
			DeleteObject(GetObj());
		}

		Vec3 Pos = Transform()->GetLocalPos();
		Pos += m_BulletDir * m_BulletSpeed * fDT;

		Transform()->SetLocalPos(Pos);

	}
}

void CPlayerArrow::awake()
{
	CProjectile::awake();
	CreateCollider((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, Vec3(80.f, 80.f, 80.f), Vec3(0.f, 0.f, -50.f));
}

void CPlayerArrow::OnCollisionEnter(CGameObject* _pOther)
{
	m_bDestroyed = true;

	Vec3 Pos = Transform()->GetWorldPos();

	Vec3 Front = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	CCameraScript::SetCameraShake(0.3f, 100.f, 5.f);

	ActivateImpactParticle(Vec4(0.4f, 0.1f, 0.1f, 0.f),Pos, Front, 10, 9);

	Play_Sound(L"ArrowHitEnemy1", 1, true, 0.2f);
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
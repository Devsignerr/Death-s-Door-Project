#include "pch.h"
#include "CTongueBullet.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"

#include <Engine/CParticleSystem.h>

void CTongueBullet::update()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();
	Vec3 vRelative = PlayerPos - Pos;
	vRelative.Normalize();

	m_fTimetoCheckPlayerPos += fDT;


	//사이각 
	m_fInternalRadianWithPlayer = vFront.Dot(vRelative);

	Vec3 vCross = vFront.Cross(vRelative);

	float fDot = vCross.Dot(vUp);

	//플레이어는 내 왼쪽에 있다 
	if (fDot > 0.0f)
		m_bPlayerMyLeft = true;

	//플레이어는 내 오른쪽에 있다 
	else if (fDot < 0.0f)
		m_bPlayerMyLeft = false;

	if (m_bPlayerMyLeft)
		Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
	else
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);

	if (m_fTimetoCheckPlayerPos < 1.0f)
	{

		Pos.x += fDT * vRelative.x * 1000.0f;
		Pos.z += fDT * vRelative.z * 1000.0f;

	}
	else
	{
		Vec3 vDiff = PlayerPos - Pos;
		vDiff.Normalize();
		vDiff.x = 0.0f;
		vDiff.z = 0.0f;

		Pos += (vFront / 0.95f) * fDT * 1000.f;
		Pos += vDiff * fDT * 1000.f;
	}


	Transform()->SetLocalPos(Pos);
	Transform()->SetLocalRot(Rot);

}

void CTongueBullet::awake()
{
	CProjectile::awake();
}

void CTongueBullet::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIndex() == (UINT)LAYER_TYPE::PLAYER_ATTACK_COL || _pOther->GetLayerIndex() == (UINT)LAYER_TYPE::PLAYER_COL)
	{
		CCameraScript::SetCameraShake(0.2f, 100.f, 4.f);

		ActivateExplosionParticle();

		GetObj()->SetAllColliderActive(false);
		SetActive(false);
		m_bDestroyed = false;
		GetObj()->ParticleSystem()->Destroy();
	}
}

CTongueBullet::CTongueBullet()
{
	m_iScriptType = (int)SCRIPT_TYPE::TONGUEBULLET;
	m_eType = EXPLOSION_PTC_TYPE::TONGUE_EXPLO_PTC;
}

CTongueBullet::~CTongueBullet()
{
}

void CTongueBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CTongueBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}
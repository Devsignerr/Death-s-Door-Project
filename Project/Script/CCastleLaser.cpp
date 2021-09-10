#include "pch.h"
#include "CCastleLaser.h"
#include "CCameraScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>

CCastleLaser::CCastleLaser()
	:m_fSphereTime(1.5f),
	 m_fCurTime(0.f),
     m_fShakeTime(0.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::CASTLELASER;
}

CCastleLaser::~CCastleLaser()
{
}


void CCastleLaser::awake()
{
	
}

void CCastleLaser::update()
{
	if (!m_bActive)
		return;

	m_fShakeTime += fDT;

	Vec3 LaserDirection = m_vLaserPointPos - m_vLaserOriginPos;
	float Length = LaserDirection.Length();

	m_fCurTime += fDT;

	if (m_fSphereTime >= m_fCurTime)
	{
		Vec3 Scale = Vec3(25.f, 25.f, 25.f);

		float Ratio = cos(m_fCurTime*70.f)+1;

		Scale *= Ratio;

		Transform()->SetLocalScale(Scale);

		Length = 0.f;
	}
	else
	{
		if (m_fShakeTime > 0.1)
		{
			Vec3 ImpactPos = m_vLaserPointPos;
			ImpactPos.y -= 1.f;

			ActivateImpactParticle(Vec4(0.4f, 0.1f, 0.1f, 1.f), ImpactPos, Vec3(0.5f, 0.5f, 0.5f), 5, 34, Vec2(150.f, 150.f), Vec2(6000.f, 6000.f));

			CCameraScript::SetCameraShake(0.1f, 100.f, 6.f);
			m_fShakeTime = 0.f;
		}

		Vec3 Scale = Transform()->GetLocalScale();

		Scale.x = 100.f;
		Scale.y = 100.f;

		float Ratio = (cos(m_fCurTime * 100.f) + 1.f)/6.f;

		Scale.x *= Ratio;
		Scale.y *= Ratio;

		GetObj()->Transform()->SetLocalScale(Vec3(Scale.x, Scale.y, Length / 1.95f));
	}

	GetObj()->Transform()->SetLocalPos(Vec3(0.f, 0.f, Length / 1.95f));
}

void CCastleLaser::SetActive(bool _b)
{
	if (_b == false && m_bActive==true)
	{
		m_fCurTime = 0.f;
		m_fShakeTime = 0.f;
		Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		GetObj()->SetAllColliderActive(false);
		GetObj()->SetAllColliderActive(false);	
	}
	else if (_b == true && m_bActive == false)
	{
		m_fCurTime = 0.f;
		m_fShakeTime = 0.f;

		Transform()->SetLocalScale(Vec3(20.f, 20.f ,20.f));
		GetObj()->SetAllColliderActive(true);
		GetObj()->SetAllColliderActive(true);
	}

	m_bActive = _b;

}


void CCastleLaser::OnCollisionEnter(CGameObject* _pOther)
{
}

void CCastleLaser::OnCollision(CGameObject* _pOther)
{
}

void CCastleLaser::OnCollisionExit(CGameObject* _pOther)
{
}



void CCastleLaser::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCastleLaser::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

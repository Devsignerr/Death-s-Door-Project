#include "pch.h"
#include "CCastleBullet.h"
#include "CRandomMgrScript.h"

#include <Engine/CParticleSystem.h>

void CCastleBullet::awake()
{
	CProjectile::awake();

	m_Pos = Transform()->GetLocalPos();

	// 최고점 도달 시간 // 0.9 ~ 1.5
	m_MaxHeightTime = 0.9f;
	// 도착점 높이
	float DestHeight = m_TargetPos.y - m_Pos.y;

	// 최고점 - 시작점(높이)
	float Height = m_MaxHeight - m_Pos.y;
	// y축 방향의 중력가속도 
	m_Gravity = 2 * Height / (m_MaxHeightTime * m_MaxHeightTime);

	// y축 방향의 속도 
	m_VelocityY = sqrtf(2 * m_Gravity * Height);

	float a = m_Gravity;
	float b = -2 * m_VelocityY;
	float c = 2 * DestHeight;

	m_DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	m_VelocityX = -(m_Pos.x - m_TargetPos.x) / m_DestAttachTime;
	m_VelocityZ = -(m_Pos.z - m_TargetPos.z) / m_DestAttachTime;
}

void CCastleBullet::start()
{
}

void CCastleBullet::update()
{
	if (false == m_bActive)
		return;

	m_Time += fDT;

	if (m_Time <= m_DestAttachTime)
	{
		float X = m_Pos.x + m_VelocityX * m_Time;
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;
		float Z = m_Pos.z + m_VelocityZ * m_Time;

		Vec3 Pos = Transform()->GetLocalPos();
		Pos.x = X;
		Pos.y = Y;
		Pos.z = Z;

		Transform()->SetLocalPos(Pos);
	}
	else 
	{
		int i = CRandomMgrScript::GetRandomintNumber(0,1);

		if(i==0)
			Play_Sound(L"RedeemerMissileExplosion2", 1, true, 0.5f);

		ActivateExplosionParticle();

		GetObj()->SetAllColliderActive(false);
		SetActive(false);
		m_bDestroyed = false;

		GetObj()->ParticleSystem()->Destroy();
	}
}

void CCastleBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCastleBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

CCastleBullet::CCastleBullet()
	: m_Pos{}
	, m_Distance(0.0f)
	, m_Time(0.0f)
	, m_DestAttachTime(0.0f)
	, m_Gravity(0.0f)
	, m_VelocityX(0.0f)
	, m_VelocityY(0.0f)
	, m_VelocityZ(0.0f)
	, m_MaxHeightTime(0.0f)
	, m_MaxHeight(2500.0f)
	, m_TargetPos{}
{
	m_iScriptType = (int)SCRIPT_TYPE::CASTLEBULLET;
	m_eType = EXPLOSION_PTC_TYPE::CASTLE_EXPLO_PTC;
}

CCastleBullet::~CCastleBullet()
{
}

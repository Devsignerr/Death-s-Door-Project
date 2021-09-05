#include "pch.h"
#include "CBazookaBullet.h"

#include "CPlayerScript.h"

#include <Engine/CLight3D.h>
#include <Engine/CParticleSystem.h>

void CBazookaBullet::awake()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	m_Pos = Transform()->GetLocalPos();

	// 최고점 도달 시간
	m_MaxHeightTime = 0.3f;
	// 도착점 높이
	float DestHeight = PlayerPos.y - m_Pos.y;

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
	m_VelocityX = -(m_Pos.x - PlayerPos.x) / m_DestAttachTime;
	m_VelocityZ = -(m_Pos.z - PlayerPos.z) / m_DestAttachTime;
}

void CBazookaBullet::start()
{
}

void CBazookaBullet::update()
{
	if (false == m_bActive)
		return;

	m_Time += fDT;


	if (m_Time <= m_DestAttachTime)
	{
		float X = m_Pos.x + m_VelocityX * m_Time;
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time* m_Time;
		float Z = m_Pos.z + m_VelocityZ * m_Time;

		Vec3 Pos = Transform()->GetLocalPos();
		Pos.x = X;
		Pos.y = Y;
		Pos.z = Z;

		Transform()->SetLocalPos(Pos);
	}
	//목표했던 위치로 옴
	else
	{
		ActivateExplosionParticle();

		GetObj()->SetAllColliderActive(false);
		SetActive(false);
		m_bDestroyed = false;
		GetObj()->ParticleSystem()->Destroy();	
	}
}

CBazookaBullet::CBazookaBullet()
	: m_Pos{}
	, m_Distance(0.0f)
	, m_Time(0.0f)
	, m_DestAttachTime(0.0f)
	, m_Gravity(0.0f)
	, m_VelocityX(0.0f)
	, m_VelocityY(0.0f)
	, m_VelocityZ(0.0f)
	, m_MaxHeightTime(0.0f)
	, m_MaxHeight(500.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BAZOOKABULLET;
	m_eType = EXPLOSION_PTC_TYPE::BAZOOKA_EXPLO_PTC;
}

CBazookaBullet::~CBazookaBullet()
{
}



void CBazookaBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CBazookaBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

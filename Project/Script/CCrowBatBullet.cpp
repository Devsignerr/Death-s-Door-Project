#include "pch.h"
#include "CCrowBatBullet.h"
#include "CPlayerScript.h"

bool CCrowBatBullet::AwakeMove()
{
	

	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	m_AwakeMoveTime += fDT;

	if (1.0f > m_AwakeMoveTime)
	{
		Pos.x += Front.x * 1000.0f * fDT;
		Pos.z += Front.z * 1000.0f * fDT;
		GetObj()->Transform()->SetLocalPos(Pos);
	}
	else
	{
		return false;
	}

	return true;
}

bool CCrowBatBullet::GuidedMove()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vRelative = PlayerPos - Pos;
	vRelative.y = 0.f;

	float Distance = Vec3::Distance(PlayerPos, Pos);

	if (600.0f < Distance)
	{
		m_fTheta += fDT * 0.2f;
		m_fLimitTheta = 45.f;
	}
	else
	{
		m_fTheta -= fDT * 0.2f;
		m_fLimitTheta = 90.f;
	}


		//m_ChangeMove = true; // 서클무브로 감 

		Vec3 PlayerRot = CPlayerScript::GetPlayerRot();
		//m_LastPlayerpos = PlayerPos;

		//m_fTheta = vUp.Dot(vRelative);
		//m_fTheta = acos(m_fTheta);
		//float Dot = PlayerCross.Dot(PlayerUp);

		//내가 플레이어를 바라보는 벡터와 내 front 벡터를 외적 
		Vec3 vCross = vRelative.Cross(vFront);

		//위 결과에서 나온 벡터와 몬스터의 up벡터(위에서 쓴 front 아님)를 내적했을때 , 
		// 위 값이 양수면 플레이어는 내 왼쪽 , 음수면 오른쪽에 있는 것 

		//프론트를 얻어왔지만 회전시켜서 세웠으므로 업벡터로 쓰임 
		float dot = vCross.Dot(vUp);

		float dist = (-vFront + vRelative).Length();

		//if (dist > 1.9f)
		//{
		//	Rot.y -= CTimeMgr::GetInst()->GetfDT() * 0.5f;
		//}

		vRelative.Normalize();

		float InnerRadian = vRelative.Dot(vFront);
		InnerRadian = acos(InnerRadian);

	
		if (InnerRadian > m_fLimitTheta *XM_PI/180.f)
		{
			//if (dist > 1.9f)
			//{
			//	Rot.y += CTimeMgr::GetInst()->GetfDT() * m_fTheta;
			//}

			//플레이어는 내 왼쪽에 있다 
			if (dot > 0.0) {
				Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_fTheta;
			}

			//플레이어는 내 오른쪽에 있다 
			else if (dot < 0.0)
			{
				Rot.y += CTimeMgr::GetInst()->GetfDT() * m_fTheta;
			}
		   
			///플레이어를 똑바로 바라보고 있다 
			//else if (dot > -20.0 && dot < 20.0 && dist < 1.f)
			//{
			//	Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_fTheta;
			//}
		}
		
		Pos += vFront * fDT * 1000.f;



		//m_fTimetoCheckPlayerPos += fDT;
		//
		//m_fInternalRadianWithPlayer = vFront.Dot(vRelative);
		//
		////플레이어는 내 왼쪽에 있다 
		//if (fDot > 0.0f)
		//	m_bPlayerMyLeft = true;
		//
		////플레이어는 내 오른쪽에 있다 
		//else if (fDot < 0.0f)
		//	m_bPlayerMyLeft = false;
		//
		//
		//if (m_bPlayerMyLeft)
		//	Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
		//else
		//	Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
		//
		//if (m_fTimetoCheckPlayerPos < 1.0f)
		//{
		//	Pos.x += fDT * vRelative.x * 800.0f;
		//	Pos.z += fDT * vRelative.z * 800.0f;
		//}
		//else
		//{
		//	Pos += (vFront / 0.95f) * fDT * 800.f;
		//}
		//

		Transform()->SetLocalPos(Pos);
		Transform()->SetLocalRot(Rot);

		return true;	
}

bool CCrowBatBullet::CircleMove()
{
	float fRadius = 300.f;

	// 중심 점 -> 플레이어의 위치
	Vec3 PlayerPos = m_LastPlayerpos;
	
	// 테두리 점 -> 슬라이딩 시작 위치
	Vec3 SlidingPoint = Vec3(fRadius * cosf(m_fTheta), 0.0f, fRadius * sinf(m_fTheta));

	// 정해진 위치로 이동
	PlayerPos += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(PlayerPos);

	m_fTheta += fDT;

	if (XM_2PI < m_fTheta)
		m_fTheta = 0.0f;

	return false;
}

void CCrowBatBullet::awake()
{
	m_fTheta = 1.5f;
}

void CCrowBatBullet::update()
{
	CProjectile::update();

	if (false == AwakeMove())
	{
		if (m_ChangeMove)
		{
			CircleMove();
		}
		else
		{
			GuidedMove();
		}
	}

}

void CCrowBatBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCrowBatBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

CCrowBatBullet::CCrowBatBullet()
	: m_AwakeMoveTime(0.0f)
	, m_fTimetoCheckPlayerPos(0.0f)
	, m_fInternalRadianWithPlayer(0.0f)
	, m_bPlayerMyLeft(false)
	, m_ChangeMove(false)
	, m_fTheta(0.0f)
	, m_LastPlayerpos{}
{
	m_iScriptType = (int)SCRIPT_TYPE::CROWBATBULLET;
}

CCrowBatBullet::~CCrowBatBullet()
{
}
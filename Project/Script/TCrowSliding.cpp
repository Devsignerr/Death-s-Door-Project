#include "pch.h"
#include "TCrowSliding.h"
#include "TCrowSlidingReady.h"
#include "CCrowScript.h"
#include "CRandomMgrScript.h"
#include "CPlayerScript.h"


#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowSliding::SlidingPosSet(int _Point)
{
	if (m_PrevPoint - 3 <= _Point && m_PrevPoint + 3 >= _Point)
		_Point += 5;
	
	float fRadius = 2000.f;
	UINT  iSliceCount = 20;
	float Interval = XM_2PI / iSliceCount;
	float fTheta = Interval * _Point;

	// 중심 점 -> 플레이어의 위치
	Vec3 Point = CPlayerScript::GetPlayerPos();

	// 테두리 점 -> 슬라이딩 시작 위치
	Vec3 SlidingPoint = Vec3(fRadius * cosf(fTheta), 0.0f, fRadius * sinf(fTheta));

	// 정해진 위치로 이동
	Point += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(Point);

	// 회전
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	relativePos.Normalize();

	float RotAngle = vFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//플레이어는 내 왼쪽에 있다 
	if (dot > 0.0)
		Rot.y -= RotAngle;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < 0.0)
		Rot.y += RotAngle;
		
	GetObj()->Transform()->SetLocalRot(Rot);

	// 진행 방향
	m_SlidingDir = relativePos;
	m_PrevPoint = _Point;
}

void TCrowSliding::DiagonalPattern(int _StartPoint)
{
	float fRadius = 2000.f;
	UINT  iSliceCount = 20;
	float Interval = XM_2PI / iSliceCount;
	float fTheta = Interval * _StartPoint;

	// 중심 점 -> 플레이어의 위치
	Vec3 Point = CPlayerScript::GetPlayerPos();
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	if (3 == _StartPoint)
	{
		if (0 == m_SlidingCount)
		{
			Point = CPlayerScript::GetPlayerPos();
		}
		else if (1 == m_SlidingCount)
		{
			Point = CPlayerScript::GetPlayerPos();
			Point.x -= 400.0f;
			fTheta = Interval * (_StartPoint + 10);
		}
		else if (2 == m_SlidingCount)
		{
			Point = CPlayerScript::GetPlayerPos();
			Point.x += 400.0f;
		
		}
		else if (3 == m_SlidingCount)
		{
			Point = CPlayerScript::GetPlayerPos();
			Point.x -= 800.0f;
			fTheta = Interval * (_StartPoint + 10);
		}
		else if (4 == m_SlidingCount)
		{
			Point = CPlayerScript::GetPlayerPos();
			Point.x += 800.0f;
		}
	}
	else
	{
			if (0 == m_SlidingCount)
			{
				Point = CPlayerScript::GetPlayerPos();
			}
			else if (1 == m_SlidingCount)
			{
				Point = CPlayerScript::GetPlayerPos();
				Point.x += 400.0f;
				fTheta = Interval * (_StartPoint - 10);

			}
			else if (2 == m_SlidingCount)
			{
				Point = CPlayerScript::GetPlayerPos();
				Point.x -= 400.0f;

			}
			else if (3 == m_SlidingCount)
			{
				Point = CPlayerScript::GetPlayerPos();
				Point.x += 800.0f;
				fTheta = Interval * (_StartPoint - 10);
			}
			else if (4 == m_SlidingCount)
			{
				Point = CPlayerScript::GetPlayerPos();
				Point.x -= 800.0f;
			}		
	}

	PlayerPos = Point;

	// 테두리 점 -> 슬라이딩 시작 위치
	Vec3 SlidingPoint = Vec3(fRadius * cosf(fTheta), 0.0f, fRadius * sinf(fTheta));

	// 정해진 위치로 이동
	Point += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(Point);

	// 회전
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	relativePos.Normalize();

	float RotAngle = vFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//플레이어는 내 왼쪽에 있다 
	if (dot > 0.0)
		Rot.y -= RotAngle;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < 0.0)
		Rot.y += RotAngle;

	GetObj()->Transform()->SetLocalRot(Rot);

	// 진행 방향
	m_SlidingDir = relativePos;
}

void TCrowSliding::CrossPattern(int _StartPoint)
{
	float fRadius = 2000.f;
	UINT  iSliceCount = 20;
	float Interval = XM_2PI / iSliceCount;
	float fTheta = Interval * _StartPoint;

	// 중심 점 -> 플레이어의 위치
	Vec3 Point = CPlayerScript::GetPlayerPos();
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	if (0 == _StartPoint)
	{
		if (1 == m_SlidingCount)
		{
			int UpDown = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (0 == UpDown)
			{
				fTheta = Interval * (_StartPoint + 5);
			}
			else
			{
				fTheta = Interval * (_StartPoint + 15);
			}

		}
	}
	else
	{
		if (1 == m_SlidingCount)
		{
			int UpDown = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (0 == UpDown)
			{
				fTheta = Interval * (_StartPoint - 5);
			}
			else
			{
				fTheta = Interval * (_StartPoint + 5);
			}

		}
	}

	PlayerPos = Point;

	// 테두리 점 -> 슬라이딩 시작 위치
	Vec3 SlidingPoint = Vec3(fRadius * cosf(fTheta), 0.0f, fRadius * sinf(fTheta));

	// 정해진 위치로 이동
	Point += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(Point);

	// 회전
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	relativePos.Normalize();

	float RotAngle = vFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//플레이어는 내 왼쪽에 있다 
	if (dot > 0.0)
		Rot.y -= RotAngle;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < 0.0)
		Rot.y += RotAngle;

	GetObj()->Transform()->SetLocalRot(Rot);

	// 진행 방향
	m_SlidingDir = relativePos;
}

void TCrowSliding::GoboardPattern(int _StartPoint)
{
	float fRadius = 2000.f;
	UINT  iSliceCount = 20;
	float Interval = XM_2PI / iSliceCount;
	float fTheta = Interval * _StartPoint;

	// 중심 점 -> 플레이어의 위치
	Vec3 Point = CPlayerScript::GetPlayerPos();
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	if (3 == _StartPoint)
	{
		Point = CPlayerScript::GetPlayerPos();
		Point.x += 400.0f;
	}
	else if(18 == _StartPoint)
	{
		Point = CPlayerScript::GetPlayerPos();
		Point.x += 400.0f;
	}
	else if (8 == _StartPoint)
	{
		Point = CPlayerScript::GetPlayerPos();
		Point.x -= 400.0f;
	}
	else if (13 == _StartPoint)
	{
		Point = CPlayerScript::GetPlayerPos();
		Point.x -= 400.0f;
	}

	PlayerPos = Point;

	// 테두리 점 -> 슬라이딩 시작 위치
	Vec3 SlidingPoint = Vec3(fRadius * cosf(fTheta), 0.0f, fRadius * sinf(fTheta));

	// 정해진 위치로 이동
	Point += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(Point);

	// 회전
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	relativePos.Normalize();

	float RotAngle = vFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//플레이어는 내 왼쪽에 있다 
	if (dot > 0.0)
		Rot.y -= RotAngle;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < 0.0)
		Rot.y += RotAngle;

	GetObj()->Transform()->SetLocalRot(Rot);

	// 진행 방향
	m_SlidingDir = relativePos;
}

void TCrowSliding::EndMotion()
{
	float fRadius = 2000.f;
	UINT  iSliceCount = 20;
	float Interval = XM_2PI / iSliceCount;

	// 회전
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	relativePos.Normalize();

	float RotAngle = vFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//플레이어는 내 왼쪽에 있다 
	if (dot > 0.0)
		Rot.y -= RotAngle;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < 0.0)
		Rot.y += RotAngle;

	GetObj()->Transform()->SetLocalRot(Rot);
}

void TCrowSliding::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	m_SlidingTime += fDT;
	if (m_SlidingTime < 1.0f)
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Pos.x += CTimeMgr::GetInst()->GetfDT() * m_SlidingDir.x * 5000.0f * 3.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * m_SlidingDir.z * 5000.0f * 3.0f;
		GetObj()->Transform()->SetLocalPos(Pos);
	}
	else
	{
		m_ChainMoveTime += fDT;
	}

	if (1.0f < m_ChainMoveTime)
	{
		m_SlidingTime = 0.0f;
		m_ChainMoveTime = 0.0f;
		++m_SlidingCount;

		if (true == m_SpecialPattern && m_SpecialPatrernSlidingCount >= m_SlidingCount)
		{
			// 특수 패턴
			// 방향과 횟수 정해야함
			if (0 == m_SpecialPatternType)
			{
				// 사선
				DiagonalPattern(3); // 위쪽에서 아래쪽
				//DiagonalPattern(13); // 아래쪽에서 위쪽
			}
			else if (1 == m_SpecialPatternType)
			{
				// 십자
				//CrossPattern(0); // 오른쪽에서 왼쪽
				CrossPattern(10);  // 왼쪽에서 오른쪽
			}
			else if (2 == m_SpecialPatternType)
			{
				// 바둑판
				if (0 == m_SlidingCount)
				{
					GoboardPattern(18);
				}
				else if (1 == m_SlidingCount)
				{
					GoboardPattern(3);

				}
				else if (2 == m_SlidingCount)
				{
					GoboardPattern(8);
				}
				else if (3 == m_SlidingCount)
				{
					GoboardPattern(13);
				}
			}
		}
		else
		{
			// dir을 플레이어기준으로 다시 구해준다
			if (2 > m_SlidingCount)
			{
				int Point = CRandomMgrScript::GetRandomintNumber(1, 20);
				SlidingPosSet(Point);
			}
		}

		if (2 == m_SlidingCount && false == m_SpecialPattern ||
			m_SpecialPatrernSlidingCount == m_SlidingCount && true == m_SpecialPattern)
		{
			EndMotion();
			GetFSM()->ChangeState(L"Jump", 0.01f, L"Jump", false);
			//GetFSM()->ChangeState(L"SlidingReady", 0.03f, L"SlidingReady", false);
		}


	
	}

}

void TCrowSliding::Enter()
{
	m_IsSliding = true;
	m_Script = (CCrowScript*)GetScript();
	TCrowSlidingReady* LastDir = (TCrowSlidingReady*)GetFSM()->FindState(L"SlidingReady");
	m_SlidingDir = LastDir->GetLastDir();
	m_SpecialPattern = CRandomMgrScript::GetRandomintNumber(0, 1);
	m_SpecialPatternType = CRandomMgrScript::GetRandomintNumber(0, 2);

	if (true == m_SpecialPattern)
	{
		if (0 == m_SpecialPatternType)
		{
			// 사선
			m_SpecialPatrernSlidingCount = CRandomMgrScript::GetRandomintNumber(2, 4);
		}
		else if (1 == m_SpecialPatternType)
		{
			// 십자
			m_SpecialPatrernSlidingCount = 2;
		}
		else if (2 == m_SpecialPatternType)
		{
			// 바둑판
			m_SpecialPatrernSlidingCount = 4;
		}
	}

}

void TCrowSliding::Exit()
{
	m_SlidingDir = {};
	m_SlidingCount = -1;
	m_SpecialPattern = 0;
	m_SpecialPatternType = -1;
	m_SpecialPatrernSlidingCount = 0;
	m_SpecialPatternEndCheck = false;
	m_SlidingTime = 0.0f;
	m_PrevPoint = 0;
}

TCrowSliding::TCrowSliding()
	: m_SpecialPattern(false)
	, m_SlidingCount(-1)
	, m_SlidingDir{}
	, m_SpecialPatternType(-1)
	, m_SpecialPatrernSlidingCount(0)
	, m_SpecialPatternEndCheck(false)
	, m_Script(nullptr)
	, m_SlidingTime(0.0f)
	, m_PrevPoint(0)
	, m_IsSliding(false)
	, m_ChainMoveTime(0.0f)
{
}

TCrowSliding::~TCrowSliding()
{
}

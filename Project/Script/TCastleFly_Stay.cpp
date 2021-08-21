#include "pch.h"
#include "TCastleFly_Stay.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void TCastleFly_Stay::update()
{

	//CAnimator3D* CurAni = GetObj()->Animator3D();
	//UINT iCurClipIdx = CurAni->GetClipIdx();

	// 레이저 포인트로 이동 후 회전

	if (false == m_IsRotLaserPoint)
	{
		Vec3 LaserPos = m_LaserPoint->Transform()->GetLocalPos();
		if (TurnToLaserPoint(LaserPos, 5.0f))
		{
			Vec3 Pos = GetObj()->Transform()->GetLocalPos();
			Vec3 Diff = LaserPos - Pos;

			float Length = Diff.Length();

			Diff.y = 0.0f;
			Diff.Normalize();

			if (Length > 5.0f)
			{
				Pos += Diff * fDT * 800.0f;
				GetObj()->Transform()->SetLocalPos(Pos);
			}
			else
			{
				m_IsRotLaserPoint = true;
			}
		}
	}
	else
	{
		Vec3 LaserFrontDir = m_LaserPoint->Transform()->GetLocalDir(DIR_TYPE::UP);
		if (TurnToLaserPoint(LaserFrontDir, 5.0f))
		{
			GetFSM()->ChangeState(L"Fly_Finish", 0.1f, L"Fly_Finish", false);
		}
	}
}

void TCastleFly_Stay::Enter()
{
	m_LaserPoint = CSceneMgr::GetInst()->GetCurScene()->FindObjectByName(L"LaserPoint");
}

void TCastleFly_Stay::Exit()
{
	m_IsRotLaserPoint = false;
}

bool TCastleFly_Stay::TurnToLaserPoint(Vec3 _TargetPos, float _RotSpeed)
{
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = _TargetPos - Pos;
	relativePos.y = 0.0f;
	relativePos.Normalize();

	Vec3 Rot = GetObj()->Transform()->GetLocalRot();
	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Vec3 vCross = relativePos.Cross(vFront);
	float dot = vCross.Dot(vUP);

	float dist = (-vFront + relativePos).Length();


	Vec3 relativePos2 = Pos - _TargetPos;
	relativePos2.Normalize();
	float Dot = vFront.Dot(relativePos2);
	Dot = acos(Dot);

	if (Dot > -XM_PI / 2.0f && Dot < XM_PI / 2.0f)
	{
		if (dot > -0.05f && dot < 0.05f)
		{
			return true;
		}
		else if (dot > 0.0f)
			Rot.y -= CTimeMgr::GetInst()->GetfDT() * _RotSpeed;
		else if (dot < 0.0f)
			Rot.y += CTimeMgr::GetInst()->GetfDT() * _RotSpeed;
	}
	else
	{
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _RotSpeed;
	}


	GetObj()->Transform()->SetLocalRot(Rot);

	return false;
}


TCastleFly_Stay::TCastleFly_Stay()
	: m_LaserPoint(nullptr)
	, m_IsRotLaserPoint(false)
{
}

TCastleFly_Stay::~TCastleFly_Stay()
{
}

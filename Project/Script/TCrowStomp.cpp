#include "pch.h"
#include "TCrowStomp.h"
#include "CPlayerScript.h"
#include "TCrowSliding.h"
#include "TCrowJump.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#define CROWRIGHT 1
#define CROWLEFT 2
void TCrowStomp::CheckBehind()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);

	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	if (dot > 0.0f)
		m_DirCheck = CROWLEFT;
	else if (dot < 0.f)
		m_DirCheck = CROWRIGHT;

}

void TCrowStomp::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	Vec3 Pos = m_Pos;
	Vec3 Diff = CPlayerScript::GetPlayerPos() - m_Pos;
	Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	if (682 >= CurAni->GetFrameIdx())
	{
		if (30.0f < abs(Diff.y))
		{
			Vec3 Down = -GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			Pos.y += CTimeMgr::GetInst()->GetfDT() * Down.y * 2500.0f;
		}

		Pos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * m_Speed * 2.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * m_Speed * 2.0f;
	}
	else
	{
		Pos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * m_Speed * 2.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * m_Speed * 2.0f;
	
		if (m_SlidingState->GetIsSliding())
		{
			m_Speed -= fDT * 500.0f;
		}
		
	}

	if (false == m_SlidingState->GetIsSliding())
	{
		m_Speed -= fDT * 1000.0f;
	}
	GetObj()->Transform()->SetLocalPos(Pos);
	m_Pos = GetObj()->Transform()->GetLocalPos();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		CheckBehind();
		if (CROWLEFT == m_DirCheck)
		{
			GetFSM()->ChangeState(L"LeftSpin", 0.03f, L"LeftSpin", false);
		}
		else if (CROWRIGHT == m_DirCheck)
		{
			GetFSM()->ChangeState(L"RightSpin", 0.03f, L"RightSpin", false);
		}

	}
}

void TCrowStomp::Enter()
{
	m_SlidingState = (TCrowSliding*)GetFSM()->FindState(L"Sliding");

	m_JumpState = (TCrowJump*)GetFSM()->FindState(L"Jump");

	m_Pos = m_JumpState->GetLastPos();

	GetObj()->Transform()->SetLocalPos(m_Pos);
}

void TCrowStomp::Exit()
{
	m_DirCheck = 0;
	m_Speed = 1500.0f;
	m_SlidingState->StompEnd();
}

TCrowStomp::TCrowStomp()
	: m_DirCheck(0)
	, m_Speed(1500.0f)
	, m_SlidingState(nullptr)
	, m_JumpState(nullptr)
{
}

TCrowStomp::~TCrowStomp()
{
}

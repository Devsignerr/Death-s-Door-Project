#include "pch.h"
#include "TCrowRun.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"
#include "CCrowScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#define CROWRIGHT 1
#define CROWLEFT 2

bool TCrowRun::CheckBehind()
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
	float Distance = Vec3::Distance(PlayerPos, Pos);



	if (dist > 1.5f && 500.0f < Distance)
	{
		if (dot > 10.0)
			m_DirCheck = CROWLEFT;
		else if (dot < -10.0f)
			m_DirCheck = CROWRIGHT;

		return true;
	}

	return false;

}

void TCrowRun::update()
{
	// 플레이어가 내 뒤에 있을때까지 뛴다
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (308 <= CurAni->GetFrameIdx())
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

		Pos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * 700.0f * 3.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * 700.0f * 3.0f;

		GetObj()->Transform()->SetLocalPos(Pos);
	}

	if (304 <= CurAni->GetFrameIdx() && 308 >= CurAni->GetFrameIdx() ||
		320 <= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(0.5f);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
	
		if (true == CheckBehind())
		{
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
}

void TCrowRun::Enter()
{
	m_Script = (CCrowScript*)GetScript();
}

void TCrowRun::Exit()
{
	m_DirCheck = 0;
}

TCrowRun::TCrowRun()
	: m_DirCheck(0)
	, m_Script(nullptr)
{
}

TCrowRun::~TCrowRun()
{
}

#include "pch.h"
#include "TCrowLeftSpin.h"
#include "CCrowScript.h"
#include "CRandomMgrScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowLeftSpin::update()
{
	// 593
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (593 >= CurAni->GetFrameIdx())
	{
		m_Script->RotateSysTem(8.0f);

	}


	if (593 > CurAni->GetFrameIdx())
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	
		Pos.x += CTimeMgr::GetInst()->GetfDT() * m_LastDir.x * 400.0f * 3.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * m_LastDir.z * 400.0f * 3.0f;

		GetObj()->Transform()->SetLocalPos(Pos);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		int Pattern = CRandomMgrScript::GetRandomintNumber(0, 4);
		if (0 == Pattern)
		{
			GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
		}
		else if (1 == Pattern)
		{
			GetFSM()->ChangeState(L"SlidingReady", 0.03f, L"SlidingReady", false);
		}
		else if (2 == Pattern)
		{
			GetFSM()->ChangeState(L"Jump", 0.03f, L"Jump", false);
		}
		else if (3 == Pattern)
		{
			GetFSM()->ChangeState(L"BatBullet", 0.03f, L"BatBullet", false);
		}
		else if (4 == Pattern)
		{
			// Æ÷È¿
			GetFSM()->ChangeState(L"GuidedBullet", 0.03f, L"GuidedBullet", false);
		}
		//else if (5 == Pattern)
		//{
		//	GetFSM()->ChangeState(L"Spitting", 0.03f, L"Spitting", false);
		//}
	}
}

void TCrowLeftSpin::Enter()
{
	m_Script = (CCrowScript*)GetScript();
	Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	m_LastDir = Front;
}

void TCrowLeftSpin::Exit()
{
}

TCrowLeftSpin::TCrowLeftSpin()
	: m_Script(nullptr)
	, m_LastDir{}
{
}

TCrowLeftSpin::~TCrowLeftSpin()
{
}

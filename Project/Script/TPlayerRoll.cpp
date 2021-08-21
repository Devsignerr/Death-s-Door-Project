#include "pch.h"
#include "TPlayerRoll.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerRoll::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (KEY_TAP(KEY_TYPE::MBTN))
	{
		m_IsRollSlash = true;
	}

	Vec3 PlayerFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();

	Pos.x += PlayerFront.x * fDT * 2000.0f;
	Pos.z += PlayerFront.z * fDT * 2000.0f;

	GetObj()->Transform()->SetLocalPos(Pos);


	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (m_IsRollSlash)
		{
			GetFSM()->ChangeState(L"Roll_Slash", 0.03f, L"Roll_Slash", false);
		}
		else
		{ 
			GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
		}
	}
}

void TPlayerRoll::Enter()
{
}

void TPlayerRoll::Exit()
{
	m_IsRollSlash = false;
}

TPlayerRoll::TPlayerRoll()
	: m_IsRollSlash(false)
{
}

TPlayerRoll::~TPlayerRoll()
{
}

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

	Vec3 PlayerFront = ((CPlayerScript*)GetScript())->GetPlayerFront();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 vMovePos = {  };

	vMovePos.x += PlayerFront.x * fDT * 2000.0f;
	vMovePos.z += PlayerFront.z * fDT * 2000.0f;

	bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(Pos + vMovePos);

	if (!IsGround)
		IsGround = ((CPlayerScript*)GetScript())->ResearchNode(Pos + vMovePos);

	if (true == IsGround)
	{


		GetObj()->Transform()->SetLocalPos(Pos + vMovePos);
	}

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

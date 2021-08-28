#include "pch.h"
#include "TCastleWalk.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleWalk::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (22 >= CurAni->GetFrameIdx() && 7 <= CurAni->GetFrameIdx()
		|| 30 <= CurAni->GetFrameIdx())
	{
		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Vec3 vMovePos = {};

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * 300.0f;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * 300.0f;

		bool IsGround = m_Script->GroundCheck(Pos + vMovePos);
		if (!IsGround)
			IsGround = m_Script->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
			m_Script->Transform()->SetLocalPos(Pos + vMovePos);

		m_Script->RotateSysTem(1.0f);
	}

	if (m_Script->RangeSearch(1000.0f))
	{
		m_Script->CheckAttackDirection();
	}

}

void TCastleWalk::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleWalk::Exit()
{
}

TCastleWalk::TCastleWalk()
	: m_Script(nullptr)
{
}

TCastleWalk::~TCastleWalk()
{
}

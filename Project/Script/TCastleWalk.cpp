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


		Pos.x += Front.x * 300.0f * fDT;
		Pos.z += Front.z * 300.0f * fDT;

		m_Script->RotateSysTem(1.0f);
		GetObj()->Transform()->SetLocalPos(Pos);
	}

	if (m_Script->RangeSearch(1500.0f))
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

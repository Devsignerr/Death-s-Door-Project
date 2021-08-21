#include "pch.h"
#include "TIronJumpAttack2.h"
#include "CIronmaceScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronJumpAttack2::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (624 <= CurAni->GetFrameIdx() && 631 >= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(8.0f);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackEnd", 0.03f, L"ChopAttackEnd", false);
	}
}

void TIronJumpAttack2::Enter()
{
	m_Script = (CIronmaceScript*)GetScript();
}

void TIronJumpAttack2::Exit()
{
}

TIronJumpAttack2::TIronJumpAttack2()
	: m_Script(nullptr)
{
}

TIronJumpAttack2::~TIronJumpAttack2()
{
}

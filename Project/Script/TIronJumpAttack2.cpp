#include "pch.h"
#include "TIronJumpAttack2.h"
#include "CIronmaceScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronJumpAttack2::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (631 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);
		((CIronmaceScript*)GetScript())->PlaySound(L"KnightSlam1", true, 0.5f);
	}
		
	if (633 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (624 <= CurAni->GetFrameIdx() && 631 >= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(8.0f);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackEnd", 0.03f, L"ChopAttackEnd", false);
	}
}

void TIronJumpAttack2::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronJumpAttack2::Exit()
{
	((CIronmaceScript*)GetScript())->PlaySound(L"Revert3", true, 0.5f);
}

TIronJumpAttack2::TIronJumpAttack2()
	: m_Script(nullptr)
{
}

TIronJumpAttack2::~TIronJumpAttack2()
{
}

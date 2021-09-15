#include "pch.h"
#include "TIronChopAttackComboEnd.h"
#include "CIronmaceScript.h"

#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

void TIronChopAttackComboEnd::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (603 == CurAni->GetFrameIdx())
	{
		((CIronmaceScript*)GetScript())->PlaySound(L"KnightSlam4", true, 0.5f);
		m_Script->OnOffAttackCol(true);
	}
	
	if (605 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (596 <= CurAni->GetFrameIdx() && 599 >= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(8.0f);
	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		// 다음 공격으로 넘어갈 경우
		if (m_Script->RangeSearch(1800.0f))
			GetFSM()->ChangeState(L"JumpAttack2", 0.03f, L"JumpAttack2", false);
		// 공격을 끝낼 경우
		else
			GetFSM()->ChangeState(L"ChopAttackEnd", 0.05f, L"ChopAttackEnd", false);
	}
}

void TIronChopAttackComboEnd::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();

	((CIronmaceScript*)GetScript())->PlaySound(L"Revert3", true, 0.5f);
}

void TIronChopAttackComboEnd::Exit()
{
}

TIronChopAttackComboEnd::TIronChopAttackComboEnd()
	: m_Script(nullptr)
{
}

TIronChopAttackComboEnd::~TIronChopAttackComboEnd()
{
}

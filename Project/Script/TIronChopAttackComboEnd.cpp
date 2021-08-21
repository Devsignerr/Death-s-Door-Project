#include "pch.h"
#include "TIronChopAttackComboEnd.h"
#include "CIronmaceScript.h"

#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

void TIronChopAttackComboEnd::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (596 <= CurAni->GetFrameIdx() && 599 >= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(8.0f);
	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		// ���� �������� �Ѿ ���
		if (m_Script->RangeSearch(1800.0f))
			GetFSM()->ChangeState(L"JumpAttack2", 0.03f, L"JumpAttack2", false);
		// ������ ���� ���
		else
			GetFSM()->ChangeState(L"ChopAttackEnd", 0.05f, L"ChopAttackEnd", false);
	}
}

void TIronChopAttackComboEnd::Enter()
{
	m_Script = (CIronmaceScript*)GetScript();
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

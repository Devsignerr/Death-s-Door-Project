#include "pch.h"
#include "TIronChopAttack.h"
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

void TIronChopAttack::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackCombo", 0.1f, L"ChopAttackCombo", false);

		//GetFSM()->ChangeState(L"ChopAttackEnd", 0.05f, L"ChopAttackEnd", false);
	}
}

void TIronChopAttack::Enter()
{
}

void TIronChopAttack::Exit()
{
}

TIronChopAttack::TIronChopAttack()
{
}

TIronChopAttack::~TIronChopAttack()
{
}

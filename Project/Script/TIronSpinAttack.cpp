#include "pch.h"
#include "TIronSpinAttack.h"
#include "CIronmaceScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronSpinAttack::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (528 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(true);
	if (530 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (526 >= CurAni->GetFrameIdx())
		m_Script->RotateSysTem(8.0f);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Walk", 0.03f, L"Walk", false);

	}
}

void TIronSpinAttack::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronSpinAttack::Exit()
{

}

TIronSpinAttack::TIronSpinAttack()

{
}

TIronSpinAttack::~TIronSpinAttack()
{
}

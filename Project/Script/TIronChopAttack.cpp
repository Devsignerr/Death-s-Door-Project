#include "pch.h"
#include "TIronChopAttack.h"
#include "CIronmaceScript.h"
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

void TIronChopAttack::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	m_Script->OnOffAttackCol(true);
	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackCombo", 0.1f, L"ChopAttackCombo", false);

		//GetFSM()->ChangeState(L"ChopAttackEnd", 0.05f, L"ChopAttackEnd", false);
	}
}

void TIronChopAttack::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronChopAttack::Exit()
{
	m_Script->OnOffAttackCol(false);
}

TIronChopAttack::TIronChopAttack()
	: m_Script(nullptr)
{
}

TIronChopAttack::~TIronChopAttack()
{
}

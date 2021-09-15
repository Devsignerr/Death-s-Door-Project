#include "pch.h"
#include "TIronIdle.h"
#include "CIronmaceScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronIdle::update()
{

	if (m_Script->RangeSearch(2000.0f))
		GetFSM()->ChangeState(L"CutScene", 0.03f, L"CutScene", false);
}

void TIronIdle::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronIdle::Exit()
{
}

TIronIdle::TIronIdle()
{
}

TIronIdle::~TIronIdle()
{
}
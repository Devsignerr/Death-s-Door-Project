#include "pch.h"
#include "TCrowIdle.h"
#include "CCrowScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>


void TCrowIdle::update()
{
	if (m_Script->RangeSearch(2000.0f))
		GetFSM()->ChangeState(L"CutScene", 0.03f, L"CutScene", false);
}

void TCrowIdle::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCrowScript*)GetScript();
}

void TCrowIdle::Exit()
{
}

TCrowIdle::TCrowIdle()
{
}

TCrowIdle::~TCrowIdle()
{
}

#include "pch.h"
#include "TCrowSlidingReady.h"
#include "CCrowScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowSlidingReady::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{	
		GetFSM()->ChangeState(L"Sliding", 0.03f, L"Sliding", false);
	}
}

void TCrowSlidingReady::Enter()
{
}

void TCrowSlidingReady::Exit()
{
	m_LastDir = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
}

TCrowSlidingReady::TCrowSlidingReady()
{
}

TCrowSlidingReady::~TCrowSlidingReady()
{
}

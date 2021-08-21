#include "pch.h"
#include "TPlayerGet_Item.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerGet_Item::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_TAP(KEY_TYPE::KEY_E))
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
	//if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	//{
	//	GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	//}
}

void TPlayerGet_Item::Enter()
{
}

void TPlayerGet_Item::Exit()
{
}

TPlayerGet_Item::TPlayerGet_Item()
{
}

TPlayerGet_Item::~TPlayerGet_Item()
{
}

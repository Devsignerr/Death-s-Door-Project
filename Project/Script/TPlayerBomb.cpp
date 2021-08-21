#include "pch.h"
#include "TPlayerBomb.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerBomb::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_HOLD(KEY_TYPE::RBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_NONE(KEY_TYPE::RBTN))
	{
		GetFSM()->ChangeState(L"Bomb_Finish", 0.03f, L"Bomb_Finish", false);
	}
}

void TPlayerBomb::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerBomb::Exit()
{
}

TPlayerBomb::TPlayerBomb()
	: m_Script(nullptr)
{
}

TPlayerBomb::~TPlayerBomb()
{
}

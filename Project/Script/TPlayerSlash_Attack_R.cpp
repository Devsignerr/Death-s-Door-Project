#include "pch.h"
#include "TPlayerSlash_Attack_R.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerSlash_Attack_R::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (false == m_IsLeftSlash && KEY_TAP(KEY_TYPE::LBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		m_IsLeftSlash = true;
	}


	if (1150 > CurAni->GetFrameIdx())
	{
		Vec3 PlayerFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Pos.x += PlayerFront.x * fDT * 200.0f;
		Pos.z += PlayerFront.z * fDT * 200.0f;

		GetObj()->Transform()->SetLocalPos(Pos);

	}

	if (1150 == CurAni->GetFrameIdx())
	{
		if (m_IsLeftSlash)
		{
			GetFSM()->ChangeState(L"Slash_L", 0.01f, L"Slash_L", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.01f, L"Idle", false);
	}

}

void TPlayerSlash_Attack_R::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerSlash_Attack_R::Exit()
{
	m_IsLeftSlash = false;
}

TPlayerSlash_Attack_R::TPlayerSlash_Attack_R()
	: m_IsLeftSlash(false)
	, m_Script(nullptr)
{

}

TPlayerSlash_Attack_R::~TPlayerSlash_Attack_R()
{
}

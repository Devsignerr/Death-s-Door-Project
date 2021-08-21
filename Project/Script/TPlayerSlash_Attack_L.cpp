#include "pch.h"
#include "TPlayerSlash_Attack_L.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerSlash_Attack_L::update()
{


	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (false == m_IsRightSlash && KEY_TAP(KEY_TYPE::LBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		m_IsRightSlash = true;
	}

	if (1185 > CurAni->GetFrameIdx())
	{
		Vec3 PlayerFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Pos.x += PlayerFront.x * fDT * 200.0f;
		Pos.z += PlayerFront.z * fDT * 200.0f;

		GetObj()->Transform()->SetLocalPos(Pos);

	}
	if (1185 == CurAni->GetFrameIdx())
	{
		if (m_IsRightSlash && m_LimitAttackCount != 2)
		{
			GetFSM()->ChangeState(L"Slash_R", 0.01f, L"Slash_R", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.01f, L"Idle", false);
	}

}

void TPlayerSlash_Attack_L::Enter()
{
	++m_LimitAttackCount;
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerSlash_Attack_L::Exit()
{
	m_IsRightSlash = false;

	if (2 == m_LimitAttackCount)
	{
		m_LimitAttackCount = 0;
	}
}

TPlayerSlash_Attack_L::TPlayerSlash_Attack_L()
	: m_IsRightSlash(false)
	, m_LimitAttackCount(0)
	, m_Script(nullptr)
{

}

TPlayerSlash_Attack_L::~TPlayerSlash_Attack_L()
{
}

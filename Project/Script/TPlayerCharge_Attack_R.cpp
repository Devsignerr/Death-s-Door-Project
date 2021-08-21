#include "pch.h"
#include "TPlayerCharge_Attack_R.h"
#include "TPlayerCharge_Attack_L.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_Attack_R::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (1150 > CurAni->GetFrameIdx())
	{

		Vec3 PlayerFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Pos.x += PlayerFront.x * fDT * 100.0f;
		Pos.z += PlayerFront.z * fDT * 100.0f;

		GetObj()->Transform()->SetLocalPos(Pos);

	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.01f, L"Idle", false);
	}
}

void TPlayerCharge_Attack_R::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
	Vec3 Pos = m_Script->GetMouseClickPos();
	m_Script->RotatetoClick(Pos);

	m_IsChargeAttack = true;
}

void TPlayerCharge_Attack_R::Exit()
{
}

TPlayerCharge_Attack_R::TPlayerCharge_Attack_R()
	: m_IsChargeAttack(false)
	, m_Script(nullptr)
{
}

TPlayerCharge_Attack_R::~TPlayerCharge_Attack_R()
{
}

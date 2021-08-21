#include "pch.h"
#include "TPlayerCharge_Attack_L.h"
#include "TPlayerCharge_Attack_R.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_Attack_L::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (1185 > CurAni->GetFrameIdx())
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

void TPlayerCharge_Attack_L::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
	Vec3 Pos = m_Script->GetMouseClickPos();
	m_Script->RotatetoClick(Pos);

	TPlayerCharge_Attack_R* AttackCheck = (TPlayerCharge_Attack_R*)GetFSM()->FindState(L"Charge_Attack_R");
	AttackCheck->ResetIsChargeAttack();
}

void TPlayerCharge_Attack_L::Exit()
{
}

TPlayerCharge_Attack_L::TPlayerCharge_Attack_L()
	: m_Script(nullptr)
{
}

TPlayerCharge_Attack_L::~TPlayerCharge_Attack_L()
{
}

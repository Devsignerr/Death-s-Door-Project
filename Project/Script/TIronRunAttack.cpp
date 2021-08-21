#include "pch.h"
#include "TIronRunAttack.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronRunAttack::update()
{
	// SpinAttakc Or JumpAttac2k Or ChopAttackComboEnd
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (326 >= CurAni->GetFrameIdx())
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

		Pos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * 3600.0f;
		Pos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * 3600.0f;

		GetObj()->Transform()->SetLocalPos(Pos);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackComboEnd", 0.05f, L"ChopAttackComboEnd", false);
	}
}

void TIronRunAttack::Enter()
{
}

void TIronRunAttack::Exit()
{
}

TIronRunAttack::TIronRunAttack()
{
}

TIronRunAttack::~TIronRunAttack()
{
}

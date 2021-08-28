#include "pch.h"
#include "TIronSpinAttackCombo.h"
#include "CIronmaceScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

bool TIronSpinAttackCombo::ChopAttackCheck()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	Vec3 vCross = relativePos.Cross(vFront);

	Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);

	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	//플레이어를 똑바로 바라보고 있다 
	if (dot > -20.0 && dot < 20.0 && dist < 1.f)
		return true;

	return false;
}

void TIronSpinAttackCombo::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (690 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(true);
	if (692 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (679 >= CurAni->GetFrameIdx())
	{
		Vec3 Rot = GetObj()->Transform()->GetLocalRot();
		Rot.y += fDT * (-XM_PI * 2.0f);
		GetObj()->Transform()->SetLocalRot(Rot);
	}
	else if (685 >= CurAni->GetFrameIdx() && 680 <= CurAni->GetFrameIdx())
	{
		Vec3 Rot = GetObj()->Transform()->GetLocalRot();
		Rot.y += fDT * (-XM_PI / 2.0f);
		GetObj()->Transform()->SetLocalRot(Rot);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (true == ChopAttackCheck())
		{
			GetFSM()->ChangeState(L"ChopAttackComboEnd", 0.03f, L"ChopAttackComboEnd", false);
		}
		else
		{
			GetFSM()->ChangeState(L"SpinAttack", 0.03f, L"SpinAttack", false);
		}
	}
}

void TIronSpinAttackCombo::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronSpinAttackCombo::Exit()
{
}

TIronSpinAttackCombo::TIronSpinAttackCombo()
	: m_Script(nullptr)
{
}

TIronSpinAttackCombo::~TIronSpinAttackCombo()
{
}

#include "pch.h"
#include "TIronChopAttackCombo.h"
#include "CIronmaceScript.h"
#include "CPlayerScript.h"

#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>


bool TIronChopAttackCombo::SpinAttackCheck()
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

	// 플레이어가 뒤쪽에 있다
	if (dist > 1.5f)
		return true;

	return false;
}

void TIronChopAttackCombo::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	CIronmaceScript* Script = (CIronmaceScript*)GetScript();

	if (661 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(true);
	if (663 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (Script->RangeSearch(1800.0f) && true == SpinAttackCheck())
			GetFSM()->ChangeState(L"SpinAttack", 0.03f, L"SpinAttack", false);
		// 다음 공격으로 넘어갈 경우
		else if (Script->RangeSearch(1800.0f))
			GetFSM()->ChangeState(L"ChopAttackComboEnd", 0.03f, L"ChopAttackComboEnd", false);
		// 공격을 끝낼 경우
		else
			GetFSM()->ChangeState(L"ChopAttackEnd", 0.05f, L"ChopAttackEnd", false);
	}
}

void TIronChopAttackCombo::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronChopAttackCombo::Exit()
{
}

TIronChopAttackCombo::TIronChopAttackCombo()
	: m_Script(nullptr)
{
}

TIronChopAttackCombo::~TIronChopAttackCombo()
{
}

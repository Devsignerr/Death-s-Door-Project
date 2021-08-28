#include "pch.h"
#include "TIronChopAttackEnd.h"
#include "CIronmaceScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronChopAttackEnd::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		// 회전 공격
		if (m_Script->RangeSearch(1800.0f) && true == m_SpinAttackCheck)
		{
			GetFSM()->ChangeState(L"SpinAttackCombo", 0.05f, L"SpinAttackCombo", false);
		}
		else if(m_Script->RangeSearch(1800.0f))
		{
			GetFSM()->ChangeState(L"ChopAttackCombo", 0.05f, L"ChopAttackCombo", false);
		}
		else if (false == m_Script->RangeSearch(1800.0f))
		{
			GetFSM()->ChangeState(L"Walk", 0.05f, L"Walk", false);
		}

	}
}

void TIronChopAttackEnd::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();

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

	//플레이어는 내 왼쪽에 있다 
	if (dot > 90.0) 
		m_SpinAttackCheck = true;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < -90.0)
		m_SpinAttackCheck = true;

}

void TIronChopAttackEnd::Exit()
{
	m_SpinAttackCheck = false;
}

TIronChopAttackEnd::TIronChopAttackEnd()
	: m_SpinAttackCheck(false)
	, m_Script(nullptr)
{
}

TIronChopAttackEnd::~TIronChopAttackEnd()
{
}

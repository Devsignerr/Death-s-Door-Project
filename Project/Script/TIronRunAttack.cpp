#include "pch.h"
#include "TIronRunAttack.h"
#include "CIronmaceScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronRunAttack::update()
{
	// SpinAttakc Or JumpAttac2k Or ChopAttackComboEnd
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (329 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(true);
	if (331 == CurAni->GetFrameIdx())
		m_Script->OnOffAttackCol(false);

	if (326 >= CurAni->GetFrameIdx())
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

		Vec3 vMovePos = {};

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * 3600.0f;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * 3600.0f;

		bool IsGround = m_Script->GroundCheck(Pos + vMovePos);
		if (!IsGround)
			IsGround = m_Script->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
			m_Script->Transform()->SetLocalPos(Pos + vMovePos);

	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"ChopAttackComboEnd", 0.05f, L"ChopAttackComboEnd", false);
	}
}

void TIronRunAttack::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronRunAttack::Exit()
{
}

TIronRunAttack::TIronRunAttack()
	: m_Script(nullptr)
{
}

TIronRunAttack::~TIronRunAttack()
{
}

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
	{
		m_Script->OnOffAttackCol(true);
		((CIronmaceScript*)GetScript())->PlaySound(L"KnightSlam1",true,0.5f);
	}
		
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


		m_fSoundTimer += fDT;

		if (0.4f < m_fSoundTimer)
		{
			m_fSoundTimer = 0.0f;

			Ptr<CSound> Sound = nullptr;

			if (m_bWalkSoundChange)
			{
				Sound = CResMgr::GetInst()->FindRes<CSound>(L"KnightStep1");

				if (Sound == nullptr)
					Sound = CResMgr::GetInst()->Load<CSound>(L"KnightStep1", L"sound\\Allsound\\KnightStep1.ogg");
			}
			else
			{
				Sound = CResMgr::GetInst()->FindRes<CSound>(L"KnightStep4");

				if (Sound == nullptr)
					Sound = CResMgr::GetInst()->Load<CSound>(L"KnightStep4", L"sound\\Allsound\\KnightStep4.ogg");
			}

			Sound->Stop();

			m_bWalkSoundChange = m_bWalkSoundChange ? false : true;


			if (m_bWalkSoundChange)
			{
				Sound = CResMgr::GetInst()->FindRes<CSound>(L"KnightStep1");

				if (Sound == nullptr)
					Sound = CResMgr::GetInst()->Load<CSound>(L"KnightStep1", L"sound\\Allsound\\KnightStep1.ogg");
			}
			else
			{
				Sound = CResMgr::GetInst()->FindRes<CSound>(L"KnightStep4");

				if (Sound == nullptr)
					Sound = CResMgr::GetInst()->Load<CSound>(L"KnightStep4", L"sound\\Allsound\\KnightStep4.ogg");
			}

			Sound->Play(1, false, 0.8f);
		}

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

	m_fSoundTimer = 0.39f;
}

void TIronRunAttack::Exit()
{
	m_fSoundTimer = 0.f;
}

TIronRunAttack::TIronRunAttack()
	: m_Script(nullptr)
{
}

TIronRunAttack::~TIronRunAttack()
{
}

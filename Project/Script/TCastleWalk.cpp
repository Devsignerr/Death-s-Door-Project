#include "pch.h"
#include "TCastleWalk.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleWalk::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (22 >= CurAni->GetFrameIdx() && 7 <= CurAni->GetFrameIdx()
		|| 30 <= CurAni->GetFrameIdx())
	{
		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Vec3 vMovePos = {};

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * Front.x * 300.0f;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * Front.z * 300.0f;

		bool IsGround = m_Script->GroundCheck(Pos + vMovePos);
		if (!IsGround)
			IsGround = m_Script->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
			m_Script->Transform()->SetLocalPos(Pos + vMovePos);

		m_Script->RotateSysTem(1.0f);
	}

	if (m_Script->RangeSearch(1000.0f))
	{
		m_Script->CheckAttackDirection();
	}

    m_fSoundTimer += fDT;

    if (1.f < m_fSoundTimer)
    {
        m_fSoundTimer = 0.0f;

        Ptr<CSound> Sound = nullptr;

        if (m_iRunSoundChange == 1)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep1");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep1", L"sound\\Allsound\\RedeemerStep1.ogg");

        }

        else if (m_iRunSoundChange == 2)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep2");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep2", L"sound\\Allsound\\RedeemerStep2.ogg");

        }

        else if (m_iRunSoundChange == 3)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep3");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep3", L"sound\\Allsound\\RedeemerStep3.ogg");

        }

        else if (m_iRunSoundChange == 4)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep4");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep4", L"sound\\Allsound\\RedeemerStep4.ogg");
        }


        Sound->Stop();

        if (m_iRunSoundChange == 1)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep1");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep1", L"sound\\Allsound\\RedeemerStep1.ogg");

            m_iRunSoundChange = 2;
        }

        else if (m_iRunSoundChange == 2)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep2");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep2", L"sound\\Allsound\\RedeemerStep2.ogg");

            m_iRunSoundChange = 3;
        }

        else if (m_iRunSoundChange == 3)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep3");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep3", L"sound\\Allsound\\RedeemerStep3.ogg");

            m_iRunSoundChange = 4;
        }

        else if (m_iRunSoundChange == 4)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"RedeemerStep4");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"RedeemerStep4", L"sound\\Allsound\\RedeemerStep4.ogg");
            m_iRunSoundChange = 1;
        }


        Sound->Play(1, false, 0.3f);
    }








}

void TCastleWalk::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleWalk::Exit()
{
    m_iRunSoundChange = 1;
}

TCastleWalk::TCastleWalk()
	: m_Script(nullptr)
    , m_iRunSoundChange(1)
{
}

TCastleWalk::~TCastleWalk()
{
}

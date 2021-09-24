#include "pch.h"
#include "TCastleCutSceneWalk.h"
#include "CCastleScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>


void TCastleCutSceneWalk::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (false == m_CurSceneCameraCheck)
	{
		if (m_Script->RangeSearch(2500.0f))
		{
			CurAni->StopAnimation();
			m_CurSceneCameraCheck = true;
			CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
			CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
			CameraScript->CutSceneCamera(Vec3(57179.543f, 10074.590f, -568.015f), Vec3(7.557f / 180.f * XM_PI, 2.531f / 180.f * XM_PI, 0.0f));
		}
	}
    else
    {

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
        }


        m_fSoundTimer += fDT;

        if (0.7f < m_fSoundTimer)
        {
            m_fSoundTimer = 0.0f;
            ++m_NextCutSceneWalkCount;
     
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


    if (3 <= m_NextCutSceneWalkCount)
    {
        GetFSM()->ChangeState(L"CutScene2", 0.04f, L"CutScene2", false);
    }
}

void TCastleCutSceneWalk::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();

	CAnimator3D* CurAni = GetObj()->Animator3D();
	CurAni->StopAnimation();
}

void TCastleCutSceneWalk::Exit()
{
    m_iRunSoundChange = 1;
}

TCastleCutSceneWalk::TCastleCutSceneWalk()
	: m_CurSceneCameraCheck(false)
	, m_Script(nullptr)
    , m_fSoundTimer(0.0f)
    , m_iRunSoundChange(1)
    , m_NextCutSceneWalkCount(0)
{
}

TCastleCutSceneWalk::~TCastleCutSceneWalk()
{
}

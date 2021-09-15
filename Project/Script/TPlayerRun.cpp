#include "pch.h"
#include "TPlayerRun.h"

#include "CPlayerScript.h"
#include "CSlashEffect.h"
#include "CRandomMgrScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

#define m_fRotationSpeed 12.0f

void TPlayerRun::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

    CGameObject* pCamera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByName(L"Camera Object");

    // 키 입력에 따른 이동
    Vec3 vPlayerPos = GetObj()->Transform()->GetLocalPos();
    Vec3 vPlayerRot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFDirPlayer = ((CPlayerScript*)GetScript())->GetPlayerFront();
    Vec3 vBDirPlayer = -((CPlayerScript*)GetScript())->GetPlayerFront();
    Vec3 vUDirPlayer = ((CPlayerScript*)GetScript())->GetPlayerUp();
    Vec3 FDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    FDirCamera.y = 0.f;

    Vec3 vMovePos = {};

    Vec3 vWantToGoDir = {};

    if (KEY_HOLD(KEY_TYPE::KEY_A))
    {
        Vec3 vLDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
        vWantToGoDir += vLDirCamera;

        //Pos 이동파트
        vMovePos += vLDirCamera * 600.f * fDT;
    }

    if (KEY_HOLD(KEY_TYPE::KEY_D))
    {
       // //회전파트
        Vec3 vRDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);

        vWantToGoDir += vRDirCamera;

        //Pos 이동파트
        vMovePos += vRDirCamera * 600.f * fDT;
    }
 
  if (KEY_HOLD(KEY_TYPE::KEY_W))
  {
      vWantToGoDir += FDirCamera;

      Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
      Vec2 GoDir = Vec2(vFront.x, vFront.z);
      
      GoDir.Normalize();
      vMovePos.x += GoDir.x * 600.f * fDT;
      vMovePos.z += GoDir.y * 600.f * fDT;
  }
 
  if (KEY_HOLD(KEY_TYPE::KEY_S))
  {
      vWantToGoDir -= FDirCamera;

      Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
      Vec2 GoDir = Vec2(vFront.x, vFront.z);
 
      GoDir.Normalize();
      vMovePos.x -= GoDir.x * 600.f * fDT;
      vMovePos.z -= GoDir.y * 600.f * fDT;
  }


	if (KEY_NONE(KEY_TYPE::KEY_W) &&
		KEY_NONE(KEY_TYPE::KEY_S) &&
		KEY_NONE(KEY_TYPE::KEY_A) &&
		KEY_NONE(KEY_TYPE::KEY_D))
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}


	if (KEY_TAP(KEY_TYPE::LBTN))
	{
        if (nullptr == CPlayerScript::m_pHorizonSlashL)
        {
            CPlayerScript::m_pHorizonSlashL = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"SLASH_L", (UINT)LAYER_TYPE::PLAYER_EFFECT);
            CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
            CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
            CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalPos(Vec3(0.f, 45.f, -170.f));
            GetObj()->AddChild(CPlayerScript::m_pHorizonSlashL);
        }

        ((CSlashEffect*)CPlayerScript::m_pHorizonSlashL->GetScript())->SetActive(true);
        CPlayerScript::m_pHorizonSlashL->SetAllMeshrenderActive(true);

        if (nullptr != CPlayerScript::m_pHorizonSlashR)
        {
            ((CSlashEffect*)CPlayerScript::m_pHorizonSlashR->GetScript())->SetActive(false);
            CPlayerScript::m_pHorizonSlashR->SetAllMeshrenderActive(false);
        }
		GetFSM()->ChangeState(L"Slash_L", 0.1f, L"Slash_L", false);
	}

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
       
		GetFSM()->ChangeState(L"Roll", 0.03f, L"Roll", false);
	}

    if (vWantToGoDir.Length() > 0.f)
    {
        vWantToGoDir.Normalize();
        float FWDot = vWantToGoDir.Dot(vFDirPlayer);
        FWDot = acos(FWDot);

        Vec3 vCross = vUDirPlayer.Cross(vWantToGoDir);


        float CFDot = vCross.Dot(vFDirPlayer);

        if (FWDot > 0.2f) 
        {
            if (CFDot > 0.f)
            {
                vPlayerRot.y -= 10.f * fDT;
            }
            else
            {
                vPlayerRot.y += 10.f * fDT;
            }
        }
    }

    vMovePos += CPlayerScript::GetOtherPower();

    bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(vPlayerPos + vMovePos);
  
    if (!IsGround)
        IsGround =((CPlayerScript*)GetScript())->ResearchNode(vPlayerPos + vMovePos);

    if(true==IsGround)
         GetObj()->Transform()->SetLocalPos(vPlayerPos+ vMovePos);

    GetObj()->Transform()->SetLocalRot(vPlayerRot);
   

    m_SoundTimer += fDT;

    if (0.4f < m_SoundTimer)
    {
        m_SoundTimer = 0.0f;

        Ptr<CSound> Sound = nullptr;

        if (m_iRunSoundChange == 1)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep1");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep1", L"sound\\Allsound\\MeleeStep1.ogg");

        }

        else if (m_iRunSoundChange == 2)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep2");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep2", L"sound\\Allsound\\MeleeStep2.ogg");

        }

        else if (m_iRunSoundChange == 4)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep4");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep4", L"sound\\Allsound\\MeleeStep4.ogg");

        }

        else if (m_iRunSoundChange == 6)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep6");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep6", L"sound\\Allsound\\MeleeStep6.ogg");
        }


        Sound->Stop();

        if (m_iRunSoundChange == 1)
        {
            Sound =CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep1");

            if(Sound==nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep1", L"sound\\Allsound\\MeleeStep1.ogg");

            m_iRunSoundChange = 2;
        }

        else if (m_iRunSoundChange == 2)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep2");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep2", L"sound\\Allsound\\MeleeStep2.ogg");

            m_iRunSoundChange = 4;
        }

        else if (m_iRunSoundChange == 4)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep4");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep4", L"sound\\Allsound\\MeleeStep4.ogg");

            m_iRunSoundChange = 6;
        }

        else if (m_iRunSoundChange == 6)
        {
            Sound = CResMgr::GetInst()->FindRes<CSound>(L"MeleeStep6");

            if (Sound == nullptr)
                Sound = CResMgr::GetInst()->Load<CSound>(L"MeleeStep6", L"sound\\Allsound\\MeleeStep6.ogg");
            m_iRunSoundChange = 1;
        }


        Sound->Play(1,false,0.02f);
    }
}

void TPlayerRun::Enter()
{
}

void TPlayerRun::Exit()
{
}

TPlayerRun::TPlayerRun():
    m_iRunSoundChange(1)
{
}

TPlayerRun::~TPlayerRun()
{
}

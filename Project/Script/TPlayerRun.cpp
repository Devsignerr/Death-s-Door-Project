#include "pch.h"
#include "TPlayerRun.h"

#include "CPlayerScript.h"
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
    Vec3 vFDirPlayer = GetObj()->Transform()->GetWorldDir(DIR_TYPE::UP);
    Vec3 vBDirPlayer = -GetObj()->Transform()->GetWorldDir(DIR_TYPE::UP);
    Vec3 vUDirPlayer = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 FDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
    Vec3 vLDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
    FDirCamera.y = 0.f;

    Vec3 vWantToGoDir = {};

    if (KEY_HOLD(KEY_TYPE::KEY_A))
    {      
        vWantToGoDir += vLDirCamera;

        //Pos 이동파트
        Vec3 vRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        vPlayerPos -= vRight * 600.f * fDT;

    }

    if (KEY_HOLD(KEY_TYPE::KEY_D))
    {
       // //회전파트
        Vec3 vRDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);

        vWantToGoDir += vRDirCamera;


        //Pos 이동파트
        vPlayerPos += vRDirCamera * 600.f * fDT;
    }
 
  if (KEY_HOLD(KEY_TYPE::KEY_W))
  {
      vWantToGoDir += FDirCamera;

      Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
      Vec2 GoDir = Vec2(vFront.x, vFront.z);
      
      GoDir.Normalize();
      vPlayerPos.x += GoDir.x * 600.f * fDT;
      vPlayerPos.z += GoDir.y * 600.f * fDT;
 
  }
 
  if (KEY_HOLD(KEY_TYPE::KEY_S))
  {
      vWantToGoDir -= FDirCamera;

      Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
      Vec2 GoDir = Vec2(vFront.x, vFront.z);
 
      GoDir.Normalize();
      vPlayerPos.x -= GoDir.x * 600.f * fDT;
      vPlayerPos.z -= GoDir.y * 600.f * fDT;
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



    GetObj()->Transform()->SetLocalPos(vPlayerPos);
    GetObj()->Transform()->SetLocalRot(vPlayerRot);
}

void TPlayerRun::Enter()
{
}

void TPlayerRun::Exit()
{
}

TPlayerRun::TPlayerRun()
{
}

TPlayerRun::~TPlayerRun()
{
}

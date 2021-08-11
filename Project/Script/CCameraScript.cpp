#include "pch.h"
#include "CCameraScript.h"
#include <Engine/CCamera.h>

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CEventMgr.h>


CCameraScript::CCameraScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT),
	m_pPlayerPivot(nullptr),
	m_eMode(CAMERA_MODE::FREE)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::update()
{
	Vec2 vScale = Camera()->GetScale();

	if (KEY_HOLD(KEY_TYPE::NUM_1))
	{
		vScale += 1.f * fDT;
	}
	else if (KEY_HOLD(KEY_TYPE::NUM_2))
	{
		vScale -= 1.f * fDT;
	}

	if (KEY_TAP(KEY_TYPE::KEY_M)) 
	{
		
	}

	CameraFreeMove();
}

void CCameraScript::start()
{
	
}

void CCameraScript::SetCameraPos()
{
	
}

void CCameraScript::CameraFreeMove()
{

	Vec3 vPos = Transform()->GetLocalPos();

	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vFront * fDT * 2000.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vBack * fDT * 2000.f;
		}
	}
	else 
	{
		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			Vec3 vUP = Transform()->GetWorldDir(DIR_TYPE::UP);
			vPos += vUP * fDT * 2000.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			Vec3 vDown = -Transform()->GetWorldDir(DIR_TYPE::UP);
			vPos += vDown * fDT * 2000.f;
		}
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * fDT * 2000.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * fDT * 2000.f;
	}

	if (KEY_HOLD(KEY_TYPE::RBTN) )
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

		Vec3 vRot = Transform()->GetLocalRot();

		vRot.x -= vMouseDir.y * fDT * 1.5f;
		vRot.y += vMouseDir.x * fDT * 1.5f;

		Transform()->SetLocalRot(vRot);
	}
	  
	Transform()->SetLocalPos(vPos);

}

void CCameraScript::FollowMove()
{
	//CGameObject* pPlayer = CSceneMgr::GetInst()->GetCurScene()->FindObjectByName(L"Player Object");
	//
	//Vec3 vPlayerPos = pPlayer->Transform()->GetLocalPos();
	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();
	//
	//Vec3 vDiff = vPos - vPlayerPos;
	//
	//float Radian = atan2f(vDiff.x, vDiff.y);
	//
	//vRot.y = Radian;
	//
	//
	//if (KEY_HOLD(KEY_TYPE::RBTN) && PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	//{
	//	//이전 마우스 좌표와 현재 마우스 좌표의 차이값 
	//	Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
	//
	//	Vec3 vPivotRot = m_pPlayerPivot->Transform()->GetLocalRot();
	//	
	//	vPivotRot.y += vMouseDir.x * fDT * 1.5f;
	//	vPivotRot.x -= vMouseDir.y * fDT * 1.5f;
	//
	//	m_pPlayerPivot->Transform()->SetLocalRot(vPivotRot);
	//	m_pPlayerPivot->Transform()->SetLocalPos(vPlayerPos);
	//}
	//
	//Transform()->SetLocalRot(vRot);
	//Transform()->SetLocalPos(vPos);
}


void CCameraScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CCameraScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}
#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CAnimator3D.h>
#include <Script/CCameraScript.h>
#include "CMissileScript.h"


CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT),
	m_eState(PLAYER_STATE::READY),
	m_fRotationSpeed(10.f)
{
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::INT, "Int Data", &m_iData));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "float Data", &m_fData));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::VEC2, "Vec2 Data", &m_v2Data));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::VEC4, "Vec4 Data", &m_v4Data));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::PREFAB, "Missile", &m_pMissilePrefab));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::STRING, "Player State", &m_strState));
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::awake()
{
	m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Missile");
}

void CPlayerScript::update()
{
	//현재 상태 Update
	switch (m_eState)
	{
	case PLAYER_STATE::READY:
		m_strState = "Ready";
		Player_Ready();
		Move();
		break;
	case PLAYER_STATE::ATTACK:
		m_strState = "Attack";
		Player_Attack();
		break;
	case PLAYER_STATE::RUN:
		m_strState = "Run";
		Player_Run();
		Move();
		break;
	}
}

void CPlayerScript::Move()
{
	CGameObject* pCamera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByName(L"Camera Object");

	// 키 입력에 따른 이동
	Vec3 vPlayerPos = Transform()->GetLocalPos();
	Vec3 vPlayerRot = Transform()->GetLocalRot();
	Vec3 vFDirPlayer = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	
	float PlayerDir = atan2f(vFDirPlayer.x, vFDirPlayer.z);
	//PlayerDir += XM_PI / 2.f;

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{
		Vec3 vLDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		float Rot = atan2f(vLDirCamera.x, vLDirCamera.z);

		float Diff = Rot- PlayerDir;


		if (Diff < -XM_PI) {
			Diff += XM_PI;
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;
		}
		else if (Diff > XM_PI) {
			Diff -= XM_PI;
			vPlayerRot.y -= Diff * m_fRotationSpeed * fDT;
		}
		else
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		//시계방향 회전 
		//vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		//반시계방향 회전 
		//vPlayerRot.y += Diff * m_fRotationSpeed * fDT;
			

		Vec3 vRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPlayerPos -= vRight * 600.f * fDT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		Vec3 vRDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		float Rot = atan2f(vRDirCamera.x, vRDirCamera.z);

		float Diff = Rot - PlayerDir;

		if (Diff < -XM_PI) {
			Diff += XM_PI;
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;
		}
		else if (Diff > XM_PI) {
			Diff -= XM_PI;
			vPlayerRot.y -= Diff * m_fRotationSpeed * fDT;
		}
		else
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		Vec3 vRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPlayerPos += vRight * 600.f * fDT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		Vec3 vFDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		float Rot = atan2f(vFDirCamera.x, vFDirCamera.z);

		float Diff = Rot - PlayerDir;

		if (Diff < -XM_PI) {
			Diff += XM_PI;
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;
		}
		else if (Diff > XM_PI) {
			Diff -= XM_PI;
			vPlayerRot.y -= Diff * m_fRotationSpeed * fDT;
		}
		else
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec2 GoDir = Vec2(vFront.x, vFront.z);

		GoDir.Normalize();
		vPlayerPos.x += GoDir.x * 600.f * fDT;
		vPlayerPos.z += GoDir.y * 600.f * fDT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		Vec3 vBDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		float Rot = atan2f(vBDirCamera.x, vBDirCamera.z);

		float Diff = Rot - PlayerDir;

		if (Diff < -XM_PI) {
			Diff += XM_PI;
			vPlayerRot.y -= Diff * m_fRotationSpeed * fDT;
		}
		else if (Diff > XM_PI) {
			Diff -= XM_PI;
			vPlayerRot.y += Diff * m_fRotationSpeed * fDT;
		}
		else
		vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		Vec3 vFront = pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec2 GoDir = Vec2(vFront.x, vFront.z);

		GoDir.Normalize();
		vPlayerPos.x -= GoDir.x * 600.f * fDT;
		vPlayerPos.z -= GoDir.y * 600.f * fDT;
	}


	Transform()->SetLocalPos(vPlayerPos);
	Transform()->SetLocalRot(vPlayerRot);
}

void CPlayerScript::Attack()
{

}


void CPlayerScript::CreateMissile()
{
	Vec3 vStartPos = Transform()->GetWorldPos();
	vStartPos.y += Transform()->GetWorldScale().y / 2.f;

	Instantiate(m_pMissilePrefab, vStartPos);	
}

void CPlayerScript::ChangeState(PLAYER_STATE _NextState)
{
	CAnimator3D* pAnimator = Animator3D();

	switch (_NextState)
	{
	case PLAYER_STATE::READY:
		if (m_eState == PLAYER_STATE::ATTACK)
			pAnimator->ChangeAnimation((int)_NextState + 1, 0.0f);
		else
			pAnimator->ChangeAnimation((int)_NextState + 1, 0.1f);
		break;
	case PLAYER_STATE::ATTACK:
		pAnimator->ChangeAnimation((int)_NextState + 1, 0.1f);
		Attack();
		break;
	case PLAYER_STATE::RUN:
		pAnimator->ChangeAnimation((int)_NextState + 1, 0.1f);
		break;
	}	

	
	m_eState = _NextState;
}


void CPlayerScript::Player_Ready()
{
	if (KEY_HOLD(KEY_TYPE::KEY_LEFT) || KEY_HOLD(KEY_TYPE::KEY_RIGHT) || KEY_HOLD(KEY_TYPE::KEY_UP) || KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		ChangeState(PLAYER_STATE::RUN);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		ChangeState(PLAYER_STATE::ATTACK);
	}
}

void CPlayerScript::Player_Attack()
{
	CAnimator3D* pAnimator = Animator3D();

	int idx = pAnimator->GetClipIdx();
	vector<tMTAnimClip>* pvecAnim = (vector<tMTAnimClip>*)pAnimator->GetMTAnimClip();

	if (pvecAnim->at(idx).bFinish == true)
	{
		ChangeState(PLAYER_STATE::READY);
	}
}

void CPlayerScript::Player_Run()
{
	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		ChangeState(PLAYER_STATE::ATTACK);
	}

	if (KEY_NONE(KEY_TYPE::KEY_LEFT) && KEY_NONE(KEY_TYPE::KEY_RIGHT) && KEY_NONE(KEY_TYPE::KEY_UP) && KEY_NONE(KEY_TYPE::KEY_DOWN))
	{
		ChangeState(PLAYER_STATE::READY);
	}
}


void CPlayerScript::OnCollisionEnter(CGameObject* _pOther)
{
	
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

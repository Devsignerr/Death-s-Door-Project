#include "pch.h"
#include "CHsh.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

CHsh::CHsh():
	CScript((UINT)SCRIPT_TYPE::HSH)
{
}

CHsh::~CHsh()
{
}

void CHsh::update()
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

		float Diff = Rot - PlayerDir;


		if (Diff < -XM_PI) {
			Diff += XM_PI;
			vPlayerRot.y += Diff * 400.f * fDT;
		}
		else if (Diff > XM_PI) {
			Diff -= XM_PI;
			vPlayerRot.y -= Diff * 400.f * fDT;
		}
		else
			vPlayerRot.y += Diff * 400.f * fDT;

		//시계방향 회전 
		//vPlayerRot.y += Diff * m_fRotationSpeed * fDT;

		//반시계방향 회전 
		//vPlayerRot.y += Diff * m_fRotationSpeed * fDT;


		Vec3 vRight = pCamera->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPlayerPos -= vRight * 600.f * fDT;
	}
	Transform()->SetLocalPos(vPlayerPos);
	Transform()->SetLocalRot(vPlayerRot);
}

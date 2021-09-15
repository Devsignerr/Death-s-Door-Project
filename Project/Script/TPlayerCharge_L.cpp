#include "pch.h"
#include "TPlayerCharge_L.h"
#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_L::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (1310 >= CurAni->GetFrameIdx() && true == m_IsChargeStay)
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::IDLE, 0.03f, L"Idle", false);
		}
	}

	if (1310 < CurAni->GetFrameIdx())
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			m_IsChargeStay = true;
		}

		if (m_IsChargeStay)
		{
			if (nullptr == CPlayerScript::m_pHeavySlashR)
			{
				CPlayerScript::m_pHeavySlashR = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"SLASH_HEAVY_R", (UINT)LAYER_TYPE::PLAYER_EFFECT);
				CPlayerScript::m_pHeavySlashR->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
				CPlayerScript::m_pHeavySlashR->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
				CPlayerScript::m_pHeavySlashR->Transform()->SetLocalPos(Vec3(30.f, 50.f, -250.f));
				GetObj()->AddChild(CPlayerScript::m_pHeavySlashR);
			}

			((CSlashEffect*)CPlayerScript::m_pHeavySlashR->GetScript())->SetActive(true);
			CPlayerScript::m_pHeavySlashR->SetAllMeshrenderActive(true);

			if (nullptr != CPlayerScript::m_pHeavySlashL)
			{
				CPlayerScript::m_pHeavySlashL->SetAllMeshrenderActive(false);
				((CSlashEffect*)CPlayerScript::m_pHeavySlashL->GetScript())->SetActive(false);
			}
				

			((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_CHARGE_ATTACK_R, 0.03f, L"Charge_Attack_R", false);
		}
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_CHARGE_MAX_L, 0.03f, L"Charge_Max_L", false);
	}
}

void TPlayerCharge_L::Enter()
{
	GetScript()->Play_Sound(L"SwordCharge", 1, true, 0.1f);
}

void TPlayerCharge_L::Exit()
{
	m_IsChargeStay = false;
}

TPlayerCharge_L::TPlayerCharge_L()
	: m_IsChargeStay(false)
{
}

TPlayerCharge_L::~TPlayerCharge_L()
{
}

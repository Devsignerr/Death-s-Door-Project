#include "pch.h"
#include "TPlayerCharge_R.h"

#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_R::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (1430 >= CurAni->GetFrameIdx() && true == m_IsChargeStay)
	{
		if (KEY_AWAY(KEY_TYPE::MBTN))
		{
			((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::IDLE, 0.03f, L"Idle", false);
		}
	}

	if (1430 < CurAni->GetFrameIdx())
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			m_IsChargeStay = true;
		}

		if (m_IsChargeStay)
		{
			if (nullptr == CPlayerScript::m_pHeavySlashL)
			{
				CPlayerScript::m_pHeavySlashL = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"SLASH_HEAVY_L", (UINT)LAYER_TYPE::PLAYER_EFFECT);
				CPlayerScript::m_pHeavySlashL->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
				CPlayerScript::m_pHeavySlashL->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
				CPlayerScript::m_pHeavySlashL->Transform()->SetLocalPos(Vec3(-30.f, 50.f, -250.f));
				GetObj()->AddChild(CPlayerScript::m_pHeavySlashL);
			}

			((CSlashEffect*)CPlayerScript::m_pHeavySlashL->GetScript())->SetActive(true);
			((CSlashEffect*)CPlayerScript::m_pHeavySlashR->GetScript())->SetActive(false);

			CPlayerScript::m_pHeavySlashL->SetAllMeshrenderActive(true);
			CPlayerScript::m_pHeavySlashR->SetAllMeshrenderActive(false);

			((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_CHARGE_ATTACK_L, 0.03f, L"Charge_Attack_L", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_CHARGE_MAX_R, 0.03f, L"Charge_Max_R", false);
	}
}

void TPlayerCharge_R::Enter()
{
}

void TPlayerCharge_R::Exit()
{
	m_IsChargeStay = false;
}

TPlayerCharge_R::TPlayerCharge_R()
	: m_IsChargeStay(false)
{
}

TPlayerCharge_R::~TPlayerCharge_R()
{
}

#include "pch.h"
#include "TPlayerChargeMax_L.h"

#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerChargeMax_L::update()
{
	if (KEY_NONE(KEY_TYPE::MBTN))
	{
		if (nullptr == CPlayerScript::m_pHeavySlashR)
		{
			CPlayerScript::m_pHeavySlashR = ((CPlayerScript*)GetScript())->IstanciatePrefab(L"SLASH_HEAVY_R", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			CPlayerScript::m_pHeavySlashR->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
			CPlayerScript::m_pHeavySlashR->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
			CPlayerScript::m_pHeavySlashR->Transform()->SetLocalPos(Vec3(30.f, 50.f, -250.f));
			GetObj()->AddChild(CPlayerScript::m_pHeavySlashR);
		}

		((CSlashEffect*)CPlayerScript::m_pHeavySlashR->GetScript())->SetActive(true);
		CPlayerScript::m_pHeavySlashR->SetAllMeshrenderActive(true);

		if (nullptr != CPlayerScript::m_pHeavySlashL)
		{
			((CSlashEffect*)CPlayerScript::m_pHeavySlashL->GetScript())->SetActive(false);
			CPlayerScript::m_pHeavySlashL->SetAllMeshrenderActive(false);
		}

		GetFSM()->ChangeState(L"Charge_Attack_R", 0.03f, L"Charge_Attack_R", false);
	}

}

void TPlayerChargeMax_L::Enter()
{
}

void TPlayerChargeMax_L::Exit()
{
}

TPlayerChargeMax_L::TPlayerChargeMax_L()
{
}

TPlayerChargeMax_L::~TPlayerChargeMax_L()
{
}

#include "pch.h"
#include "TPlayerChargeMax_R.h"


#include "CPlayerScript.h"
#include "CSlashEffect.h"


#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerChargeMax_R::update()
{
	if (KEY_NONE(KEY_TYPE::MBTN))
	{
		if (nullptr == CPlayerScript::m_pHeavySlashL)
		{
			CPlayerScript::m_pHeavySlashL = ((CPlayerScript*)GetScript())->IstanciatePrefab(L"SLASH_HEAVY_L", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			CPlayerScript::m_pHeavySlashL->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
			CPlayerScript::m_pHeavySlashL->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
			CPlayerScript::m_pHeavySlashL->Transform()->SetLocalPos(Vec3(-30.f, 50.f, -250.f));
			GetObj()->AddChild(CPlayerScript::m_pHeavySlashL);
		}

		if (nullptr != CPlayerScript::m_pHeavySlashR)
		{
			((CSlashEffect*)CPlayerScript::m_pHeavySlashR->GetScript())->SetActive(false);
			CPlayerScript::m_pHeavySlashR->SetAllMeshrenderActive(false);
		}

		((CSlashEffect*)CPlayerScript::m_pHeavySlashL->GetScript())->SetActive(true);
		CPlayerScript::m_pHeavySlashL->SetAllMeshrenderActive(true);



		GetFSM()->ChangeState(L"Charge_Attack_L", 0.03f, L"Charge_Attack_L", false);
	}
}

void TPlayerChargeMax_R::Enter()
{
}

void TPlayerChargeMax_R::Exit()
{
}

TPlayerChargeMax_R::TPlayerChargeMax_R()
{
}

TPlayerChargeMax_R::~TPlayerChargeMax_R()
{
}

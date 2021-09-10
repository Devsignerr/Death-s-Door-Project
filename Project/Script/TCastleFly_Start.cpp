#include "pch.h"
#include "TCastleFly_Start.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>



void TCastleFly_Start::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Fly_Stay", 0.1f, L"Fly_Stay", false);
	}

	((CCastleScript*)GetScript())->ActivateFlyCloud();

}


void TCastleFly_Start::Enter()
{
	if (nullptr == m_Script)
	{
		m_Script = (CCastleScript*)GetScript();
	}
	m_Script->OnOffAttackCol(false, LAYER_TYPE::BOSS_COL);
	m_Script->SetFlyCloudTime(0.8f);
}

void TCastleFly_Start::Exit()
{
}

TCastleFly_Start::TCastleFly_Start()
	: m_Script(nullptr)
{
}

TCastleFly_Start::~TCastleFly_Start()
{
}

#include "pch.h"
#include "TCastleFly_Finish.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleFly_Finish::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	//

	if (435 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);
		m_Script->TransColPos(Vec3(0.0f, -10000.0f, 0.0f));
		m_Script->TransColScale(Vec3(160000.0f, 20000.0f, 160000.0f));

	}
	if (437 == CurAni->GetFrameIdx())
	{
		m_Script->TransColScale(Vec3(50000.0f, 30000.0f, 50000.0f));
		m_Script->OnOffAttackCol(false);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Laser_Ready", 0.1f, L"Laser_Ready", true);
	}
}

void TCastleFly_Finish::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();

	m_Script->OnOffAttackCol(true, LAYER_TYPE::BOSS_COL);
	m_Script->TransColPos(Vec3(0.0f, -50000.0f, 0.0f), LAYER_TYPE::BOSS_COL);
	
}

void TCastleFly_Finish::Exit()
{
}

TCastleFly_Finish::TCastleFly_Finish()
	: m_Script(nullptr)
{
}

TCastleFly_Finish::~TCastleFly_Finish()
{
}

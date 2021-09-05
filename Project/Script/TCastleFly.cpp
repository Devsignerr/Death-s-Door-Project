#include "pch.h"
#include "TCastleFly.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleFly::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (172 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);
		m_Script->TransColPos(Vec3(0.0f, -10000.0f, 0.0f));
		m_Script->TransColScale(Vec3(160000.0f, 20000.0f, 160000.0f));

	}
	if (174 == CurAni->GetFrameIdx())
	{
		m_Script->TransColScale(Vec3(50000.0f, 30000.0f, 50000.0f));
		m_Script->OnOffAttackCol(false);

		m_Script->TransColPos(Vec3(0.0f, -50000.0f, 0.0f), LAYER_TYPE::BOSS_COL);
		m_Script->OnOffAttackCol(true, LAYER_TYPE::BOSS_COL);
	}

	if (149 <= CurAni->GetFrameIdx() && 169 >= CurAni->GetFrameIdx())
	{
		m_Script->RotateSysTem(3.0f);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Missile_Open", 0.1f, L"Missile_Open", true);
	}
}

void TCastleFly::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();
	m_Script->OnOffAttackCol(false, LAYER_TYPE::BOSS_COL);
}

void TCastleFly::Exit()
{
}

TCastleFly::TCastleFly()
	: m_Script(nullptr)
{
}

TCastleFly::~TCastleFly()
{
}

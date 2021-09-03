#include "pch.h"
#include "TPlayerBomb.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerBomb::update()
{
	m_fCurTime += fDT;

	if (m_fRotateTime > m_fCurTime)
	{
		float x = 0.f;
		float y = 200.f - cos(m_fCurTime * 4) * m_fRadius;
		float z = -20.f - sin(m_fCurTime * 4) * m_fRadius;
		
		CPlayerScript::m_pBomb->Transform()->SetLocalPos(Vec3(x, y, z));
	}

	if (KEY_HOLD(KEY_TYPE::RBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_NONE(KEY_TYPE::RBTN))
	{
		GetFSM()->ChangeState(L"Bomb_Finish", 0.03f, L"Bomb_Finish", false);
	}
}

void TPlayerBomb::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
	
	CPlayerScript::m_Weapon->MeshRender()->Activate(false);
}

void TPlayerBomb::Exit()
{
	m_fCurTime = 0.f;
}

TPlayerBomb::TPlayerBomb()
	: m_Script(nullptr)
	, m_fRotateTime(0.8f)
	, m_fRadius(100.f)
{
}

TPlayerBomb::~TPlayerBomb()
{
}

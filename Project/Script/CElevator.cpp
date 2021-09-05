#include "pch.h"
#include "CElevator.h"

void CElevator::awake()
{
	Vec3 Down = -Transform()->GetLocalDir(DIR_TYPE::UP);
	m_DestPos = Transform()->GetLocalPos() + Down * m_Depth;
}

void CElevator::update()
{
	if (true == m_IsOn)
	{
		Vec3 Pos = Transform()->GetLocalPos();

		if (m_DestPos.y < Pos.y)
		{
			Pos.y -= fDT * 500.0f;
			Transform()->SetLocalPos(Pos);
		}
		else
		{
			m_IsOn = false;
		}
	}

	//if (KEY_TAP(KEY_TYPE::SPACE))
	//{
	//	m_IsOpen = true;
	//}
}

void CElevator::OnCollisionEnter(CGameObject* _pOther)
{
}

void CElevator::OnCollision(CGameObject* _pOther)
{
}

void CElevator::OnCollisionExit(CGameObject* _pOther)
{
}

void CElevator::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CElevator::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

CElevator::CElevator()
	: m_IsOn(false)
	, m_Depth(2000.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::ELEVATOR;
	m_GimicType = GIMICTYPE::ELEVATOR;
}

CElevator::~CElevator()
{
}


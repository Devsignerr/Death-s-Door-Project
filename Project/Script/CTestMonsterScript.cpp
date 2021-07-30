#include "pch.h"
#include "CTestMonsterScript.h"

void CTestMonsterScript::awake()
{
}

void CTestMonsterScript::update()
{
	CMonsterScript::update();

	switch (m_CurState)
	{
	case CMonsterScript::MONSTERSTATE::IDLE: Idle(); break;
	case CMonsterScript::MONSTERSTATE::MOVE: Move(); break;
	case CMonsterScript::MONSTERSTATE::CHASE: Chase(); break;
	case CMonsterScript::MONSTERSTATE::READY_ACTION: ReadyAction(); break;
	case CMonsterScript::MONSTERSTATE::ATTACK: Attack(); break;
	case CMonsterScript::MONSTERSTATE::FINISH_ACTION: FinishAction(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}
}



void CTestMonsterScript::Idle()
{
	if (RangeSearch(700.0f))
	{
		ChangeState(MONSTERSTATE::CHASE);
	}
}

void CTestMonsterScript::Move()
{
}

void CTestMonsterScript::Chase()
{
	if (RangeSearch(300.0f))
	{
		ChangeState(MONSTERSTATE::IDLE);
	}
}

void CTestMonsterScript::ReadyAction()
{
}

void CTestMonsterScript::Attack()
{
}

void CTestMonsterScript::FinishAction()
{
}

void CTestMonsterScript::Death()
{
}

CTestMonsterScript::CTestMonsterScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::TESTMONSTERSCRIPT;
}

CTestMonsterScript::~CTestMonsterScript()
{
}

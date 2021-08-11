#include "pch.h"
#include "CSpearManScript.h"

void CSpearManScript::awake()
{
}

void CSpearManScript::update()
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
	case CMonsterScript::MONSTERSTATE::JUMP: Jump(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}
}

void CSpearManScript::Idle()
{
}

void CSpearManScript::Move()
{
}

void CSpearManScript::Chase()
{
}

void CSpearManScript::ReadyAction()
{
}

void CSpearManScript::Attack()
{
	// 공격 두번후 반대 방향으로 점프하면서 회피하는것같다
	// 플레이어 위치에 맞춰 공격 방향 달라짐

}

void CSpearManScript::FinishAction()
{
}

void CSpearManScript::Jump()
{
}

void CSpearManScript::Death()
{
}

CSpearManScript::CSpearManScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::SPEARMANSCRIPT;
}

CSpearManScript::~CSpearManScript()
{
}

void CSpearManScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CSpearManScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
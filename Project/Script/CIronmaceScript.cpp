#include "pch.h"
#include "CIronmaceScript.h"

void CIronmaceScript::awake()
{
}

void CIronmaceScript::update()
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

void CIronmaceScript::Idle()
{
	// 처음 등장 점프후 메이스로 찍기
}

void CIronmaceScript::Move()
{
}

void CIronmaceScript::Chase()
{
}

void CIronmaceScript::ReadyAction()
{
}

void CIronmaceScript::Attack()
{
	// 한번에 찍을 수있는 공격 횟수는 3번
	// 플레이어의 위치에 맞춰 찍는다
	// 도중에 플레이어가 멀어지면 공격을 멈춤
	// 공격이 끝난후 플레이어가 멀어져있는 경우 달려간후 찍는다
}

void CIronmaceScript::FinishAction()
{
}

void CIronmaceScript::Jump()
{
}

void CIronmaceScript::Death()
{
}


CIronmaceScript::CIronmaceScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::IRONMACESCRIPT;
}

CIronmaceScript::~CIronmaceScript()
{
}

void CIronmaceScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CIronmaceScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
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
	// ó�� ���� ������ ���̽��� ���
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
	// �ѹ��� ���� ���ִ� ���� Ƚ���� 3��
	// �÷��̾��� ��ġ�� ���� ��´�
	// ���߿� �÷��̾ �־����� ������ ����
	// ������ ������ �÷��̾ �־����ִ� ��� �޷����� ��´�
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
#include "pch.h"
#include "CMonsterScript.h"

#include "CPlayerScript.h"

CMonsterScript::CMonsterScript()
	: m_CurState(MONSTERSTATE::IDLE)
{
	m_iScriptType = (int)SCRIPT_TYPE::MONSTERSCRIPT;
}

CMonsterScript::~CMonsterScript()
{
}

bool CMonsterScript::RangeSearch(float _Range)
{
	return false;
}

void CMonsterScript::ChangeState(MONSTERSTATE _State, float _BlendingTime)
{
}

void CMonsterScript::update()
{
	CActorScript::update();
}

void CMonsterScript::OnCollisionEnter(CGameObject* _pOther)
{	
	CPlayerScript* pScript = dynamic_cast<CPlayerScript*>(_pOther->GetComponent(COMPONENT_TYPE::SCRIPT));
	if (nullptr != pScript)
	{
		DeleteObject(GetObj());
	}
}
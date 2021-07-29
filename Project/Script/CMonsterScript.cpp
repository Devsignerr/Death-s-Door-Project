#include "pch.h"
#include "CMonsterScript.h"

#include "CPlayerScript.h"

CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::update()
{
}

void CMonsterScript::OnCollisionEnter(CGameObject* _pOther)
{	
	CPlayerScript* pScript = dynamic_cast<CPlayerScript*>(_pOther->GetComponent(COMPONENT_TYPE::SCRIPT));
	if (nullptr != pScript)
	{
		DeleteObject(GetObj());
	}
}
#include "pch.h"
#include "CScriptMgr.h"

#include "CActorScript.h"
#include "CCameraScript.h"
#include "CHsh.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CSkyBoxScript.h"
#include "CTestMonsterScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CActorScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CHsh");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSkyBoxScript");
	_vec.push_back(L"CTestMonsterScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CActorScript" == _strScriptName)
		return new CActorScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CHsh" == _strScriptName)
		return new CHsh;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSkyBoxScript" == _strScriptName)
		return new CSkyBoxScript;
	if (L"CTestMonsterScript" == _strScriptName)
		return new CTestMonsterScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ACTORSCRIPT:
		return new CActorScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::HSH:
		return new CHsh;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SKYBOXSCRIPT:
		return new CSkyBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return new CTestMonsterScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ACTORSCRIPT:
		return L"CActorScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::HSH:
		return L"CHsh";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SKYBOXSCRIPT:
		return L"CSkyBoxScript";
		break;

	case SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return L"CTestMonsterScript";
		break;

	}
	return nullptr;
}
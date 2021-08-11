#include "pch.h"
#include "CScriptMgr.h"

#include "CActorScript.h"
#include "CBatScript.h"
#include "CBazookaBullet.h"
#include "CBazookaScript.h"
#include "CCameraScript.h"
#include "CIronmaceScript.h"
#include "CMonsterScript.h"
#include "CNailScript.h"
#include "CPlantBullet.h"
#include "CPlantScript.h"
#include "CPlayerScript.h"
#include "CProjectile.h"
#include "CRandomMgrScript.h"
#include "CSkullBullet.h"
#include "CSkullScript.h"
#include "CSkyBoxScript.h"
#include "CSpearManScript.h"
#include "CSpiderScript.h"
#include "CTestMonsterAttackEffect.h"
#include "CTestMonsterScript.h"
#include "CTongueBullet.h"
#include "CTongueScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CActorScript");
	_vec.push_back(L"CBatScript");
	_vec.push_back(L"CBazookaBullet");
	_vec.push_back(L"CBazookaScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CIronmaceScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNailScript");
	_vec.push_back(L"CPlantBullet");
	_vec.push_back(L"CPlantScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectile");
	_vec.push_back(L"CRandomMgrScript");
	_vec.push_back(L"CSkullBullet");
	_vec.push_back(L"CSkullScript");
	_vec.push_back(L"CSkyBoxScript");
	_vec.push_back(L"CSpearManScript");
	_vec.push_back(L"CSpiderScript");
	_vec.push_back(L"CTestMonsterAttackEffect");
	_vec.push_back(L"CTestMonsterScript");
	_vec.push_back(L"CTongueBullet");
	_vec.push_back(L"CTongueScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CActorScript" == _strScriptName)
		return new CActorScript;
	if (L"CBatScript" == _strScriptName)
		return new CBatScript;
	if (L"CBazookaBullet" == _strScriptName)
		return new CBazookaBullet;
	if (L"CBazookaScript" == _strScriptName)
		return new CBazookaScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CIronmaceScript" == _strScriptName)
		return new CIronmaceScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNailScript" == _strScriptName)
		return new CNailScript;
	if (L"CPlantBullet" == _strScriptName)
		return new CPlantBullet;
	if (L"CPlantScript" == _strScriptName)
		return new CPlantScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectile" == _strScriptName)
		return new CProjectile;
	if (L"CRandomMgrScript" == _strScriptName)
		return new CRandomMgrScript;
	if (L"CSkullBullet" == _strScriptName)
		return new CSkullBullet;
	if (L"CSkullScript" == _strScriptName)
		return new CSkullScript;
	if (L"CSkyBoxScript" == _strScriptName)
		return new CSkyBoxScript;
	if (L"CSpearManScript" == _strScriptName)
		return new CSpearManScript;
	if (L"CSpiderScript" == _strScriptName)
		return new CSpiderScript;
	if (L"CTestMonsterAttackEffect" == _strScriptName)
		return new CTestMonsterAttackEffect;
	if (L"CTestMonsterScript" == _strScriptName)
		return new CTestMonsterScript;
	if (L"CTongueBullet" == _strScriptName)
		return new CTongueBullet;
	if (L"CTongueScript" == _strScriptName)
		return new CTongueScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ACTORSCRIPT:
		return new CActorScript;
		break;
	case (UINT)SCRIPT_TYPE::BATSCRIPT:
		return new CBatScript;
		break;
	case (UINT)SCRIPT_TYPE::BAZOOKABULLET:
		return new CBazookaBullet;
		break;
	case (UINT)SCRIPT_TYPE::BAZOOKASCRIPT:
		return new CBazookaScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::IRONMACESCRIPT:
		return new CIronmaceScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::NAILSCRIPT:
		return new CNailScript;
		break;
	case (UINT)SCRIPT_TYPE::PLANTBULLET:
		return new CPlantBullet;
		break;
	case (UINT)SCRIPT_TYPE::PLANTSCRIPT:
		return new CPlantScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PROJECTILE:
		return new CProjectile;
		break;
	case (UINT)SCRIPT_TYPE::RANDOMMGRSCRIPT:
		return new CRandomMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::SKULLBULLET:
		return new CSkullBullet;
		break;
	case (UINT)SCRIPT_TYPE::SKULLSCRIPT:
		return new CSkullScript;
		break;
	case (UINT)SCRIPT_TYPE::SKYBOXSCRIPT:
		return new CSkyBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::SPEARMANSCRIPT:
		return new CSpearManScript;
		break;
	case (UINT)SCRIPT_TYPE::SPIDERSCRIPT:
		return new CSpiderScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTMONSTERATTACKEFFECT:
		return new CTestMonsterAttackEffect;
		break;
	case (UINT)SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return new CTestMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::TONGUEBULLET:
		return new CTongueBullet;
		break;
	case (UINT)SCRIPT_TYPE::TONGUESCRIPT:
		return new CTongueScript;
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

	case SCRIPT_TYPE::BATSCRIPT:
		return L"CBatScript";
		break;

	case SCRIPT_TYPE::BAZOOKABULLET:
		return L"CBazookaBullet";
		break;

	case SCRIPT_TYPE::BAZOOKASCRIPT:
		return L"CBazookaScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::IRONMACESCRIPT:
		return L"CIronmaceScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::NAILSCRIPT:
		return L"CNailScript";
		break;

	case SCRIPT_TYPE::PLANTBULLET:
		return L"CPlantBullet";
		break;

	case SCRIPT_TYPE::PLANTSCRIPT:
		return L"CPlantScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PROJECTILE:
		return L"CProjectile";
		break;

	case SCRIPT_TYPE::RANDOMMGRSCRIPT:
		return L"CRandomMgrScript";
		break;

	case SCRIPT_TYPE::SKULLBULLET:
		return L"CSkullBullet";
		break;

	case SCRIPT_TYPE::SKULLSCRIPT:
		return L"CSkullScript";
		break;

	case SCRIPT_TYPE::SKYBOXSCRIPT:
		return L"CSkyBoxScript";
		break;

	case SCRIPT_TYPE::SPEARMANSCRIPT:
		return L"CSpearManScript";
		break;

	case SCRIPT_TYPE::SPIDERSCRIPT:
		return L"CSpiderScript";
		break;

	case SCRIPT_TYPE::TESTMONSTERATTACKEFFECT:
		return L"CTestMonsterAttackEffect";
		break;

	case SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return L"CTestMonsterScript";
		break;

	case SCRIPT_TYPE::TONGUEBULLET:
		return L"CTongueBullet";
		break;

	case SCRIPT_TYPE::TONGUESCRIPT:
		return L"CTongueScript";
		break;

	}
	return nullptr;
}
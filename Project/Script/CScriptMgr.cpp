#include "pch.h"
#include "CScriptMgr.h"

#include "CActorScript.h"
#include "CAnchor.h"
#include "CAttackImpactScript.h"
#include "CBatScript.h"
#include "CBazookaBullet.h"
#include "CBazookaScript.h"
#include "CBossScript.h"
#include "CCameraScript.h"
#include "CCastleBullet.h"
#include "CCastleLaser.h"
#include "CCastleScript.h"
#include "CCrowBatBullet.h"
#include "CCrowBullet.h"
#include "CCrowEggBullet.h"
#include "CCrowScript.h"
#include "CDoorScript.h"
#include "CEffectScript.h"
#include "CElevator.h"
#include "CEnding.h"
#include "CExplosionParticle.h"
#include "CFadeScript.h"
#include "CFence.h"
#include "CFireDamageParticle.h"
#include "CFogScript.h"
#include "CGateScript.h"
#include "CHpBar.h"
#include "CIronmaceScript.h"
#include "CLeverScript.h"
#include "CMapChange.h"
#include "CMapGimic.h"
#include "CMemoryPoolScript.h"
#include "CMonsterScript.h"
#include "CNailScript.h"
#include "CPlantBullet.h"
#include "CPlantScript.h"
#include "CPlayerArrow.h"
#include "CPlayerBomb.h"
#include "CPlayerChain.h"
#include "CPlayerHook.h"
#include "CPlayerMagic.h"
#include "CPlayerScript.h"
#include "CProjectile.h"
#include "CRandomMgrScript.h"
#include "CSceneChange.h"
#include "CSkullBullet.h"
#include "CSkullScript.h"
#include "CSkyBoxScript.h"
#include "CSlashEffect.h"
#include "CSpearManScript.h"
#include "CSpiderScript.h"
#include "CSpinLaser.h"
#include "CTestMonsterAttackEffect.h"
#include "CTestMonsterScript.h"
#include "CTitle.h"
#include "CTongueBullet.h"
#include "CTongueScript.h"
#include "CUpDownSwitch.h"
#include "CWeaponType.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CActorScript");
	_vec.push_back(L"CAnchor");
	_vec.push_back(L"CAttackImpactScript");
	_vec.push_back(L"CBatScript");
	_vec.push_back(L"CBazookaBullet");
	_vec.push_back(L"CBazookaScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CCastleBullet");
	_vec.push_back(L"CCastleLaser");
	_vec.push_back(L"CCastleScript");
	_vec.push_back(L"CCrowBatBullet");
	_vec.push_back(L"CCrowBullet");
	_vec.push_back(L"CCrowEggBullet");
	_vec.push_back(L"CCrowScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CElevator");
	_vec.push_back(L"CEnding");
	_vec.push_back(L"CExplosionParticle");
	_vec.push_back(L"CFadeScript");
	_vec.push_back(L"CFence");
	_vec.push_back(L"CFireDamageParticle");
	_vec.push_back(L"CFogScript");
	_vec.push_back(L"CGateScript");
	_vec.push_back(L"CHpBar");
	_vec.push_back(L"CIronmaceScript");
	_vec.push_back(L"CLeverScript");
	_vec.push_back(L"CMapChange");
	_vec.push_back(L"CMapGimic");
	_vec.push_back(L"CMemoryPoolScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNailScript");
	_vec.push_back(L"CPlantBullet");
	_vec.push_back(L"CPlantScript");
	_vec.push_back(L"CPlayerArrow");
	_vec.push_back(L"CPlayerBomb");
	_vec.push_back(L"CPlayerChain");
	_vec.push_back(L"CPlayerHook");
	_vec.push_back(L"CPlayerMagic");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectile");
	_vec.push_back(L"CRandomMgrScript");
	_vec.push_back(L"CSceneChange");
	_vec.push_back(L"CSkullBullet");
	_vec.push_back(L"CSkullScript");
	_vec.push_back(L"CSkyBoxScript");
	_vec.push_back(L"CSlashEffect");
	_vec.push_back(L"CSpearManScript");
	_vec.push_back(L"CSpiderScript");
	_vec.push_back(L"CSpinLaser");
	_vec.push_back(L"CTestMonsterAttackEffect");
	_vec.push_back(L"CTestMonsterScript");
	_vec.push_back(L"CTitle");
	_vec.push_back(L"CTongueBullet");
	_vec.push_back(L"CTongueScript");
	_vec.push_back(L"CUpDownSwitch");
	_vec.push_back(L"CWeaponType");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CActorScript" == _strScriptName)
		return new CActorScript;
	if (L"CAnchor" == _strScriptName)
		return new CAnchor;
	if (L"CAttackImpactScript" == _strScriptName)
		return new CAttackImpactScript;
	if (L"CBatScript" == _strScriptName)
		return new CBatScript;
	if (L"CBazookaBullet" == _strScriptName)
		return new CBazookaBullet;
	if (L"CBazookaScript" == _strScriptName)
		return new CBazookaScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CCastleBullet" == _strScriptName)
		return new CCastleBullet;
	if (L"CCastleLaser" == _strScriptName)
		return new CCastleLaser;
	if (L"CCastleScript" == _strScriptName)
		return new CCastleScript;
	if (L"CCrowBatBullet" == _strScriptName)
		return new CCrowBatBullet;
	if (L"CCrowBullet" == _strScriptName)
		return new CCrowBullet;
	if (L"CCrowEggBullet" == _strScriptName)
		return new CCrowEggBullet;
	if (L"CCrowScript" == _strScriptName)
		return new CCrowScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CElevator" == _strScriptName)
		return new CElevator;
	if (L"CEnding" == _strScriptName)
		return new CEnding;
	if (L"CExplosionParticle" == _strScriptName)
		return new CExplosionParticle;
	if (L"CFadeScript" == _strScriptName)
		return new CFadeScript;
	if (L"CFence" == _strScriptName)
		return new CFence;
	if (L"CFireDamageParticle" == _strScriptName)
		return new CFireDamageParticle;
	if (L"CFogScript" == _strScriptName)
		return new CFogScript;
	if (L"CGateScript" == _strScriptName)
		return new CGateScript;
	if (L"CHpBar" == _strScriptName)
		return new CHpBar;
	if (L"CIronmaceScript" == _strScriptName)
		return new CIronmaceScript;
	if (L"CLeverScript" == _strScriptName)
		return new CLeverScript;
	if (L"CMapChange" == _strScriptName)
		return new CMapChange;
	if (L"CMapGimic" == _strScriptName)
		return new CMapGimic;
	if (L"CMemoryPoolScript" == _strScriptName)
		return new CMemoryPoolScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNailScript" == _strScriptName)
		return new CNailScript;
	if (L"CPlantBullet" == _strScriptName)
		return new CPlantBullet;
	if (L"CPlantScript" == _strScriptName)
		return new CPlantScript;
	if (L"CPlayerArrow" == _strScriptName)
		return new CPlayerArrow;
	if (L"CPlayerBomb" == _strScriptName)
		return new CPlayerBomb;
	if (L"CPlayerChain" == _strScriptName)
		return new CPlayerChain;
	if (L"CPlayerHook" == _strScriptName)
		return new CPlayerHook;
	if (L"CPlayerMagic" == _strScriptName)
		return new CPlayerMagic;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectile" == _strScriptName)
		return new CProjectile;
	if (L"CRandomMgrScript" == _strScriptName)
		return new CRandomMgrScript;
	if (L"CSceneChange" == _strScriptName)
		return new CSceneChange;
	if (L"CSkullBullet" == _strScriptName)
		return new CSkullBullet;
	if (L"CSkullScript" == _strScriptName)
		return new CSkullScript;
	if (L"CSkyBoxScript" == _strScriptName)
		return new CSkyBoxScript;
	if (L"CSlashEffect" == _strScriptName)
		return new CSlashEffect;
	if (L"CSpearManScript" == _strScriptName)
		return new CSpearManScript;
	if (L"CSpiderScript" == _strScriptName)
		return new CSpiderScript;
	if (L"CSpinLaser" == _strScriptName)
		return new CSpinLaser;
	if (L"CTestMonsterAttackEffect" == _strScriptName)
		return new CTestMonsterAttackEffect;
	if (L"CTestMonsterScript" == _strScriptName)
		return new CTestMonsterScript;
	if (L"CTitle" == _strScriptName)
		return new CTitle;
	if (L"CTongueBullet" == _strScriptName)
		return new CTongueBullet;
	if (L"CTongueScript" == _strScriptName)
		return new CTongueScript;
	if (L"CUpDownSwitch" == _strScriptName)
		return new CUpDownSwitch;
	if (L"CWeaponType" == _strScriptName)
		return new CWeaponType;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ACTORSCRIPT:
		return new CActorScript;
		break;
	case (UINT)SCRIPT_TYPE::ANCHOR:
		return new CAnchor;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKIMPACTSCRIPT:
		return new CAttackImpactScript;
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
	case (UINT)SCRIPT_TYPE::BOSSSCRIPT:
		return new CBossScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::CASTLEBULLET:
		return new CCastleBullet;
		break;
	case (UINT)SCRIPT_TYPE::CASTLELASER:
		return new CCastleLaser;
		break;
	case (UINT)SCRIPT_TYPE::CASTLESCRIPT:
		return new CCastleScript;
		break;
	case (UINT)SCRIPT_TYPE::CROWBATBULLET:
		return new CCrowBatBullet;
		break;
	case (UINT)SCRIPT_TYPE::CROWBULLET:
		return new CCrowBullet;
		break;
	case (UINT)SCRIPT_TYPE::CROWEGGBULLET:
		return new CCrowEggBullet;
		break;
	case (UINT)SCRIPT_TYPE::CROWSCRIPT:
		return new CCrowScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ELEVATOR:
		return new CElevator;
		break;
	case (UINT)SCRIPT_TYPE::ENDING:
		return new CEnding;
		break;
	case (UINT)SCRIPT_TYPE::EXPLOSIONPARTICLE:
		return new CExplosionParticle;
		break;
	case (UINT)SCRIPT_TYPE::FADESCRIPT:
		return new CFadeScript;
		break;
	case (UINT)SCRIPT_TYPE::FENCE:
		return new CFence;
		break;
	case (UINT)SCRIPT_TYPE::FIREDAMAGEPARTICLE:
		return new CFireDamageParticle;
		break;
	case (UINT)SCRIPT_TYPE::FOGSCRIPT:
		return new CFogScript;
		break;
	case (UINT)SCRIPT_TYPE::GATESCRIPT:
		return new CGateScript;
		break;
	case (UINT)SCRIPT_TYPE::HPBAR:
		return new CHpBar;
		break;
	case (UINT)SCRIPT_TYPE::IRONMACESCRIPT:
		return new CIronmaceScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVERSCRIPT:
		return new CLeverScript;
		break;
	case (UINT)SCRIPT_TYPE::MAPCHANGE:
		return new CMapChange;
		break;
	case (UINT)SCRIPT_TYPE::MAPGIMIC:
		return new CMapGimic;
		break;
	case (UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT:
		return new CMemoryPoolScript;
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
	case (UINT)SCRIPT_TYPE::PLAYERARROW:
		return new CPlayerArrow;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERBOMB:
		return new CPlayerBomb;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCHAIN:
		return new CPlayerChain;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHOOK:
		return new CPlayerHook;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERMAGIC:
		return new CPlayerMagic;
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
	case (UINT)SCRIPT_TYPE::SCENECHANGE:
		return new CSceneChange;
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
	case (UINT)SCRIPT_TYPE::SLASHEFFECT:
		return new CSlashEffect;
		break;
	case (UINT)SCRIPT_TYPE::SPEARMANSCRIPT:
		return new CSpearManScript;
		break;
	case (UINT)SCRIPT_TYPE::SPIDERSCRIPT:
		return new CSpiderScript;
		break;
	case (UINT)SCRIPT_TYPE::SPINLASER:
		return new CSpinLaser;
		break;
	case (UINT)SCRIPT_TYPE::TESTMONSTERATTACKEFFECT:
		return new CTestMonsterAttackEffect;
		break;
	case (UINT)SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return new CTestMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::TITLE:
		return new CTitle;
		break;
	case (UINT)SCRIPT_TYPE::TONGUEBULLET:
		return new CTongueBullet;
		break;
	case (UINT)SCRIPT_TYPE::TONGUESCRIPT:
		return new CTongueScript;
		break;
	case (UINT)SCRIPT_TYPE::UPDOWNSWITCH:
		return new CUpDownSwitch;
		break;
	case (UINT)SCRIPT_TYPE::WEAPONTYPE:
		return new CWeaponType;
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

	case SCRIPT_TYPE::ANCHOR:
		return L"CAnchor";
		break;

	case SCRIPT_TYPE::ATTACKIMPACTSCRIPT:
		return L"CAttackImpactScript";
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

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::CASTLEBULLET:
		return L"CCastleBullet";
		break;

	case SCRIPT_TYPE::CASTLELASER:
		return L"CCastleLaser";
		break;

	case SCRIPT_TYPE::CASTLESCRIPT:
		return L"CCastleScript";
		break;

	case SCRIPT_TYPE::CROWBATBULLET:
		return L"CCrowBatBullet";
		break;

	case SCRIPT_TYPE::CROWBULLET:
		return L"CCrowBullet";
		break;

	case SCRIPT_TYPE::CROWEGGBULLET:
		return L"CCrowEggBullet";
		break;

	case SCRIPT_TYPE::CROWSCRIPT:
		return L"CCrowScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::ELEVATOR:
		return L"CElevator";
		break;

	case SCRIPT_TYPE::ENDING:
		return L"CEnding";
		break;

	case SCRIPT_TYPE::EXPLOSIONPARTICLE:
		return L"CExplosionParticle";
		break;

	case SCRIPT_TYPE::FADESCRIPT:
		return L"CFadeScript";
		break;

	case SCRIPT_TYPE::FENCE:
		return L"CFence";
		break;

	case SCRIPT_TYPE::FIREDAMAGEPARTICLE:
		return L"CFireDamageParticle";
		break;

	case SCRIPT_TYPE::FOGSCRIPT:
		return L"CFogScript";
		break;

	case SCRIPT_TYPE::GATESCRIPT:
		return L"CGateScript";
		break;

	case SCRIPT_TYPE::HPBAR:
		return L"CHpBar";
		break;

	case SCRIPT_TYPE::IRONMACESCRIPT:
		return L"CIronmaceScript";
		break;

	case SCRIPT_TYPE::LEVERSCRIPT:
		return L"CLeverScript";
		break;

	case SCRIPT_TYPE::MAPCHANGE:
		return L"CMapChange";
		break;

	case SCRIPT_TYPE::MAPGIMIC:
		return L"CMapGimic";
		break;

	case SCRIPT_TYPE::MEMORYPOOLSCRIPT:
		return L"CMemoryPoolScript";
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

	case SCRIPT_TYPE::PLAYERARROW:
		return L"CPlayerArrow";
		break;

	case SCRIPT_TYPE::PLAYERBOMB:
		return L"CPlayerBomb";
		break;

	case SCRIPT_TYPE::PLAYERCHAIN:
		return L"CPlayerChain";
		break;

	case SCRIPT_TYPE::PLAYERHOOK:
		return L"CPlayerHook";
		break;

	case SCRIPT_TYPE::PLAYERMAGIC:
		return L"CPlayerMagic";
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

	case SCRIPT_TYPE::SCENECHANGE:
		return L"CSceneChange";
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

	case SCRIPT_TYPE::SLASHEFFECT:
		return L"CSlashEffect";
		break;

	case SCRIPT_TYPE::SPEARMANSCRIPT:
		return L"CSpearManScript";
		break;

	case SCRIPT_TYPE::SPIDERSCRIPT:
		return L"CSpiderScript";
		break;

	case SCRIPT_TYPE::SPINLASER:
		return L"CSpinLaser";
		break;

	case SCRIPT_TYPE::TESTMONSTERATTACKEFFECT:
		return L"CTestMonsterAttackEffect";
		break;

	case SCRIPT_TYPE::TESTMONSTERSCRIPT:
		return L"CTestMonsterScript";
		break;

	case SCRIPT_TYPE::TITLE:
		return L"CTitle";
		break;

	case SCRIPT_TYPE::TONGUEBULLET:
		return L"CTongueBullet";
		break;

	case SCRIPT_TYPE::TONGUESCRIPT:
		return L"CTongueScript";
		break;

	case SCRIPT_TYPE::UPDOWNSWITCH:
		return L"CUpDownSwitch";
		break;

	case SCRIPT_TYPE::WEAPONTYPE:
		return L"CWeaponType";
		break;

	}
	return nullptr;
}
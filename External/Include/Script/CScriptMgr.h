#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	ACTORSCRIPT,
	BATSCRIPT,
	BAZOOKABULLET,
	BAZOOKASCRIPT,
	CAMERASCRIPT,
	IRONMACESCRIPT,
	MONSTERSCRIPT,
	NAILSCRIPT,
	PLANTBULLET,
	PLANTSCRIPT,
	PLAYERSCRIPT,
	PROJECTILE,
	RANDOMMGRSCRIPT,
	SKULLBULLET,
	SKULLSCRIPT,
	SKYBOXSCRIPT,
	SPEARMANSCRIPT,
	SPIDERSCRIPT,
	TESTMONSTERATTACKEFFECT,
	TESTMONSTERSCRIPT,
	TONGUEBULLET,
	TONGUESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};

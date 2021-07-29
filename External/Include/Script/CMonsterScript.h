#pragma once

#include <Engine\CScript.h>

class CMonsterScript
		: public CScript
{


public:
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	~CMonsterScript();
};
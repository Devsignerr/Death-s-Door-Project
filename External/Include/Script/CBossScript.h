#pragma once
#include "CActorScript.h"

class CBossScript : public CActorScript
{
public:
	virtual bool RangeSearch(float _Range);
	virtual bool RotateSysTem(float _RotSpeed);
public:
	virtual void awake();
	virtual void start();
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther) {};
	virtual void OnCollision(CGameObject* _pOther) {};
	virtual void OnCollisionExit(CGameObject* _pOther) {};

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CBossScript);

public:
	CBossScript();
	~CBossScript();
};


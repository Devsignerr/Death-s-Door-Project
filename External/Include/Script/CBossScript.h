#pragma once
#include "CActorScript.h"

class CBossScript : public CActorScript
{
public:
	virtual bool RangeSearch(float _Range);
	virtual bool RotateSysTem(float _RotSpeed);

public:
	virtual void OnOffAttackCol(bool _OnOff, LAYER_TYPE _Type = LAYER_TYPE::BOSS_ATTACK_COL);
	virtual void CreateCol(const wstring& _Name, Vec3 _Pos, Vec3 _Scale, LAYER_TYPE _Type);
	virtual void TransColPos(Vec3 _Pos, LAYER_TYPE _Type = LAYER_TYPE::BOSS_ATTACK_COL);
	virtual void TransColScale(Vec3 _Scale, LAYER_TYPE _Type = LAYER_TYPE::BOSS_ATTACK_COL);

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
	virtual ~CBossScript();
};


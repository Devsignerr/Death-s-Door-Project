#pragma once
#include "CMapGimic.h"
class CAnchor :
	public CMapGimic
{
public:
	void awake() override;
	void update() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CAnchor();
	~CAnchor();

public:
	CLONE(CAnchor);
};


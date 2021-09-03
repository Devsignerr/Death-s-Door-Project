#pragma once
#include "CComponent.h"
class CUI :
    public CComponent
{
public:
	virtual void awake();
	virtual void start();
	virtual void update();
	virtual void lateupdate(){}
	virtual void finalupdate(){}

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CLONE(CUI);

public:
	CUI();
	CUI(CUI& _origin);
	virtual ~CUI();
};


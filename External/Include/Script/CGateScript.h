#pragma once
#include "CMapGimic.h"

enum class GATE_STATE
{
	WAIT,
	OPEN,
};

class CGateScript :
    public CMapGimic
{
private:
	GATE_STATE m_eState;
	float	    m_fOpenTime;
	float		m_fCurTime;

public:
	void awake() override;
	void update() override;

public:
	void Open();

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CGateScript();
	~CGateScript();

public:
	CLONE(CGateScript);
};


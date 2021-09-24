#pragma once
#include "CMapGimic.h"

enum class LEVER_STATE
{
	WAIT,
	OPEN,
};

class CLeverScript :
    public CMapGimic
{
private:
	LEVER_STATE m_eState;
	float	    m_fRotateTime;
	float		m_fCurTime;

public:
	void awake() override;
	void update() override;

public:
	void OpenGate();
	void Rotate();
public:
	void OnCollisionEnter(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CLeverScript();
	~CLeverScript();

public:
	CLONE(CLeverScript);
};


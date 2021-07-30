#pragma once
#include <Engine/CScript.h>

class CActorScript : public CScript
{
private:
	float m_Dist;
public:
	bool GroundCheck();
	void ResearchNode();

public:
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	CLONE(CActorScript);

public:
	CActorScript();
	~CActorScript();
};


#pragma once
#include <Engine/CScript.h>

class CActorScript : public CScript
{
private:
	float m_Dist;
	float m_fOffSetPos;
public:
	bool GroundCheck();
	void ResearchNode();

public:
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther);


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


	CLONE(CActorScript);

public:
	CActorScript();
	~CActorScript();
};


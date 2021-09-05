#pragma once
#include "CMapGimic.h"
class CElevator :
	public CMapGimic
{
private:
	bool	m_IsOn;
	Vec3	m_DestPos;
	float	m_Depth;
public:
	void SetOn() { m_IsOn = true; }
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
	CElevator();
	~CElevator();

public:
	CLONE(CElevator);
};


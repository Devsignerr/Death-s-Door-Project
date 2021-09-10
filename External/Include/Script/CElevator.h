#pragma once
#include "CMapGimic.h"

class CGameObject;
class CElevator :
	public CMapGimic
{
private:
	bool			m_IsOn;
	Vec3			m_DestPos;
	float			m_Depth;
	CGameObject* m_Elevator2_Nav;
	Vec3			m_Elevator2_Nav_OriginPos;
private:
	void CreateCol();
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


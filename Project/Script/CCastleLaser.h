#pragma once
#include "CProjectile.h"
class CCastleLaser :
    public CProjectile
{
private:
	Vec3 m_vLaserOriginPos;
	Vec3 m_vLaserPointPos;

	//active 직후 잠시 구체로 머무는 시간 
	float m_fSphereTime;
	float m_fCurTime;
	float m_fShakeTime;

public:
	void SetLaserOriginPos(Vec3 _Pos) { m_vLaserOriginPos = _Pos; }
	void SetLaserPointPos(Vec3 _Pos) { m_vLaserPointPos = _Pos; }

public:
	void awake() override;
	void update() override;


	virtual void SetActive(bool _b);

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CCastleLaser();
	~CCastleLaser();


public:
	CLONE(CCastleLaser);
};


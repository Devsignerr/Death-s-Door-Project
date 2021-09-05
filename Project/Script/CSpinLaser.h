#pragma once
#include "CMapGimic.h"
#include <queue>

class CGameObject;
class CSpinLaser : public CMapGimic
{
private:
	struct COLLIDERECTINFO
	{
		Vec3 Pos;
		Vec3 Scale;
		Vec3 Rot;
	};
private:
	static bool	m_StopCheck;
	static bool	m_PrevStopState;
public:
	static bool GetLaserStopCheck() { return m_StopCheck; }
	static void SetLaserStopCheck(bool _LaserStopCheck) { m_StopCheck = _LaserStopCheck; }

private:
	float						m_Dist[2];
	CGameObject* m_Laser[2];
	CGameObject* m_LaserCol[2];
	vector<CGameObject*>		m_AllRect;
	std::queue<COLLIDERECTINFO> m_CollideRectInfo;
	float						m_StopTime;

private:
	void LaserCreate();
	void CalLaserLength(bool _bRight, float _Dist, CGameObject* _Laser, CGameObject* _LaserCol);
	void CreateRectToCollide();
	void SetCollideRectInfo();
	void ReActivateLaser(bool _StopState, float _ReActivateTime);


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
	CSpinLaser();
	~CSpinLaser();

public:
	CLONE(CSpinLaser);
};


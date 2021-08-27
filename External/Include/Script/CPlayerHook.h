#pragma once
#include "CProjectile.h"
#include <queue>

class CPlayerHook :
    public CProjectile
{
private:
	Vec3	m_vHookDir;
	float	m_fHookSpeed;
	float   m_fMaxRange; //훅이 날라갈수 있는 최대거리 

	float   m_fAccChainDistance;

	bool	m_bHooked;
	bool	m_bFailed;
	bool    m_bArrived;

	Vec3    m_vHookedPos; //훅이 걸린 위치 
	vector<CGameObject*> m_vecChain;

public:
	void SpawnChain();//훅의 위치와 플레이어의 위치 사이에 체인이 새로 들어올 공간이 있다면 체인을 생성 
	void returnChainToMemoryPool();
	void returnAllChainToMemoryPool(); //모든 체인을 메모리풀로 반납 
	void CalculateDistance(); //최대거리에 도달했는지 
	void AdjustChainEmissive();
	void CheckArrived();

public:
	bool  IsHooked() { return m_bHooked; }
	bool  IsFailed() { return m_bFailed; }
	bool  IsArrived() { return m_bArrived; }

	void SetHookDir(Vec3 _Dir) { m_vHookDir = _Dir; }
	void SetHookSpeed(float _Speed) { m_fHookSpeed = _Speed; }

	Vec3 GetHookedPos() { return m_vHookedPos; }

	void Destroy();

public:
	void update() override;

public:
	void Spawnupdate();  //아직 날라가는 중이어서 체인을 계속 메모리풀에서 가져올때 
	void Hookupdate();   //갈고리 걸린후 체인을 메모리풀로 계속 돌려보낼때


public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerHook);

public:
	CPlayerHook();
	~CPlayerHook();
};


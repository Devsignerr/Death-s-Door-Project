#pragma once
#include "CProjectile.h"
#include <queue>

class CPlayerHook :
    public CProjectile
{
private:
	Vec3	m_vHookDir;
	float	m_fHookSpeed;
	float   m_fMaxRange; //���� ���󰥼� �ִ� �ִ�Ÿ� 

	float   m_fAccChainDistance;

	bool	m_bHooked;
	bool	m_bFailed;
	bool    m_bArrived;

	Vec3    m_vHookedPos; //���� �ɸ� ��ġ 
	vector<CGameObject*> m_vecChain;

public:
	void SpawnChain();//���� ��ġ�� �÷��̾��� ��ġ ���̿� ü���� ���� ���� ������ �ִٸ� ü���� ���� 
	void returnChainToMemoryPool();
	void returnAllChainToMemoryPool(); //��� ü���� �޸�Ǯ�� �ݳ� 
	void CalculateDistance(); //�ִ�Ÿ��� �����ߴ��� 
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
	void awake() override;
	void update() override;

public:
	void Spawnupdate();  //���� ���󰡴� ���̾ ü���� ��� �޸�Ǯ���� �����ö� 
	void Hookupdate();   //���� �ɸ��� ü���� �޸�Ǯ�� ��� ����������


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


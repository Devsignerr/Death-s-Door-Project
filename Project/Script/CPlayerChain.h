#pragma once
#include "CProjectile.h"
class CPlayerChain :
    public CProjectile
{
private:
	static float m_fScale; //체인 크기 
	static float m_fStep; //체인 간격

	float m_fEmissive; // Hookfly 상태일때 플레이어와 가까운 체인일수록 밝기가 낮아짐 

public:
	void SetEmissive(float _Ratio) { m_fEmissive = _Ratio; }
	static float GetScale() { return m_fScale; }
	static float GetStep() { return m_fStep; }

public:
	void update() override;

	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;

	CLONE(CPlayerChain);

public:
	CPlayerChain();
	~CPlayerChain();
};


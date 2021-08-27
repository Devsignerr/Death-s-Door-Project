#pragma once
#include "CProjectile.h"
class CPlayerChain :
    public CProjectile
{
private:
	static float m_fScale; //ü�� ũ�� 
	static float m_fStep; //ü�� ����

	float m_fEmissive; // Hookfly �����϶� �÷��̾�� ����� ü���ϼ��� ��Ⱑ ������ 

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


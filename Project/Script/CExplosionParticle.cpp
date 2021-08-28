#include "pch.h"
#include "CExplosionParticle.h"

#include <Engine/CParticleSystem.h>

CExplosionParticle::CExplosionParticle()
{
	m_iScriptType = (UINT)SCRIPT_TYPE::EXPLOSIONPARTICLE;
}

CExplosionParticle::~CExplosionParticle()
{
}



void CExplosionParticle::awake()
{
	m_iMaxCount = GetObj()->ParticleSystem()->GetMaxCount();
}

void CExplosionParticle::update()
{
}

void CExplosionParticle::SetActive(bool _b)
{
	if (_b == false && m_bActive==true)
	{
		GetObj()->ParticleSystem()->SetMaxCount(0);
		GetObj()->ParticleSystem()->Activate(false);
	}

	if (_b == true && m_bActive == false)
	{
		GetObj()->ParticleSystem()->Activate(true);
		GetObj()->ParticleSystem()->SetMaxCount(m_iMaxCount);
		GetObj()->ParticleSystem()->SetAccLiveCount(0);
		GetObj()->ParticleSystem()->SetAccTime(0.f);
		
	}

	m_bActive = _b;

}


void CExplosionParticle::SaveToScene(FILE* _pFile)
{
	CEffectScript::SaveToScene(_pFile);
}

void CExplosionParticle::LoadFromScene(FILE* _pFile)
{
	CEffectScript::LoadFromScene(_pFile);
}

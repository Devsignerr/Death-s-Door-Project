#include "pch.h"
#include "CFireDamageParticle.h"
#include "CMemoryPoolScript.h"
#include <Engine/CParticleSystem.h>
#include <Engine/CFrustumSphere.h>

CFireDamageParticle::CFireDamageParticle()
{
	m_iScriptType = (UINT)SCRIPT_TYPE::FIREDAMAGEPARTICLE;
	m_fLifeTime = 5.f;
}

CFireDamageParticle::~CFireDamageParticle()
{
}


void CFireDamageParticle::awake()
{

}

void CFireDamageParticle::update()
{
	if (m_bActive)
	{
		m_fCurTime += fDT;

		if (m_fCurTime > m_fLifeTime)
		{
			m_fCurTime = 0.f;

			CMemoryPoolScript::ReturnObj(GetObj());
		}

		if (m_pTarget)
		{
			Vec3 LocalPos = m_pTarget->Transform()->GetLocalPos();
			LocalPos.y += 100.f;
			Transform()->SetLocalPos(LocalPos);
		}

	}
}

void CFireDamageParticle::SetActive(bool _b)
{
	if(_b == false && m_bActive == true)
	{
		GetObj()->ParticleSystem()->Activate(false);
		m_pTarget = nullptr;
	}

	if (_b == true && m_bActive == false)
	{
		GetObj()->ParticleSystem()->Activate(true);
	}

	m_bActive = _b;
}


void CFireDamageParticle::SaveToScene(FILE* _pFile)
{
	CEffectScript::SaveToScene(_pFile);
}

void CFireDamageParticle::LoadFromScene(FILE* _pFile)
{
	CEffectScript::LoadFromScene(_pFile);
}

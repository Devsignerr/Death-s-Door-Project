#include "pch.h"
#include "CAttackImpactScript.h"
#include "CMemoryPoolScript.h"

CAttackImpactScript::CAttackImpactScript()
	:m_vDir(0.f),
	m_fMoveSpeed(4000.f)
	, m_fScale(100.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::ATTACKIMPACTSCRIPT;
	m_fLifeTime = 0.2f;
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "SCALE", &m_fScale));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "SPEED", &m_fMoveSpeed));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "LIFETIME", &m_fLifeTime));
}

CAttackImpactScript::~CAttackImpactScript()
{
}

void CAttackImpactScript::SetActive(bool _b)
{
	if (_b == false && m_bActive == true)
	{
		GetObj()->SetAllMeshrenderActive(false);
	}

	if (_b == true && m_bActive == false)
	{
		GetObj()->SetAllMeshrenderActive(true);
	}

	m_bActive = _b;
}

void CAttackImpactScript::update()
{
	if (!m_bActive)
		return;

	m_fCurTime += fDT;

	Vec3 LocalPos = Transform()->GetLocalPos();
	Vec3 Front = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float Ratio = m_fCurTime / m_fLifeTime; // 0~1
	LocalPos += Front * m_fMoveSpeed * fDT * (cos(Ratio * XM_PI*2.f)+1)/2.f;
	Transform()->SetLocalPos(LocalPos);

	Vec3 LocalScale = Transform()->GetLocalScale();

	LocalScale.z = m_fScale *pow(sin(Ratio * XM_PI),4);
	LocalScale.y = 2.f;
	LocalScale.x = 2.f;

	Transform()->SetLocalScale(LocalScale);

	if (m_fCurTime > m_fLifeTime)
	{
		m_fCurTime = 0.f;

		m_fMoveSpeed = 0.f;
		m_fScale = 0.f;
		GetObj()->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		CMemoryPoolScript::ReturnObj(GetObj());
	}

}


void CAttackImpactScript::SaveToScene(FILE* _pFile)
{
	CEffectScript::SaveToScene(_pFile);
}

void CAttackImpactScript::LoadFromScene(FILE* _pFile)
{
	CEffectScript::LoadFromScene(_pFile);
}

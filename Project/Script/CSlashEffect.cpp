#include "pch.h"
#include "CSlashEffect.h"


CSlashEffect::CSlashEffect():
	m_fBurnRatio(0.f),
	m_vBurnColor(Vec3(0.6f,0.1f,0.1f)),
	m_eType(SLASH_TYPE::HORIZON)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::SLASHEFFECT;
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "BurnRatio", &m_fBurnRatio));
}

CSlashEffect::~CSlashEffect()
{
}


void CSlashEffect::SetActive(bool _b)
{
	m_bActive = _b;

	if (true == _b)
	{
		GetObj()->GetChild()[0]->MeshRender()->Activate(true);
		GetObj()->GetChild()[0]->SetDynamicShadow(false);
		m_fBurnRatio = 0.f;
	}
		
	else if (false == _b)
	{
		m_fBurnRatio = 0.f;
		GetObj()->GetChild()[0]->SetDynamicShadow(false);
		GetObj()->GetChild()[0]->MeshRender()->Activate(false);
	}
}

void CSlashEffect::update()
{
	if (!m_bActive)
		return;

	m_fBurnRatio += fDT*3.f;

	GetObj()->GetChild()[0]->SetDynamicShadow(false);
	CGameObject* MeshObj = GetObj()->GetChild()[0];

	Ptr<CMaterial> Mtrl = MeshObj->MeshRender()->GetSharedMaterial(0);

	int BurnType = (UINT)BURN_TYPE::SLASH_BURN;

	Mtrl->SetData(SHADER_PARAM::INT_1, &BurnType);

	Vec4 BurnInfo = Vec4(m_vBurnColor, m_fBurnRatio);

	Mtrl->SetData(SHADER_PARAM::VEC4_0, &BurnInfo);
}


void CSlashEffect::SaveToScene(FILE* _pFile)
{
	CEffectScript::SaveToScene(_pFile);
}

void CSlashEffect::LoadFromScene(FILE* _pFile)
{
	CEffectScript::LoadFromScene(_pFile);
}

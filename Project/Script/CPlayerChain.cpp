#include "pch.h"
#include "CPlayerChain.h"

float CPlayerChain::m_fScale = 26.f;
float CPlayerChain::m_fStep= 80.f;

CPlayerChain::CPlayerChain():
	m_fEmissive(1.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::PLAYERCHAIN;
}

CPlayerChain::~CPlayerChain()
{
}

void CPlayerChain::awake()
{
	CProjectile::awake();
}

void CPlayerChain::update()
{
	if (!m_bActive)
		return;

	//Ptr<CMaterial> Mtrl =GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0);
	//tMtrlInfo info= Mtrl.Get()->GetMtrlInfo();
	//info.vEmis.x *= m_fEmissive;
	//info.vEmis.y *= m_fEmissive;
	//info.vEmis.z *= m_fEmissive;
	//GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0)
}

void CPlayerChain::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlayerChain::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}


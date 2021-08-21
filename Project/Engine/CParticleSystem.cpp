#include "pch.h"
#include "CParticleSystem.h"

#include "CTransform.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"
#include "CParticleUpdateShader.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCopyShaderCS.h"
#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CComponent(COMPONENT_TYPE::PARTICLE)
	, m_iMaxCount(100)
	, m_iAliveCount(0)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(0.8f)
	, m_fMinSpeed(20.f)
	, m_fMaxSpeed(40.f)
	, m_fFrequency(0.1f)
	, m_fAccTime(0.f)
	, m_vCreateRange(Vec3(80.f, 80.f, 0.f))
	, m_vStartColor(Vec4(0.87f, 0.224f, 0.0f, 0.3f))
	, m_vEndColor(Vec4(1.f, 0.0f, 0.0f, 1.f))
	, m_vStartScale(Vec3(40.f, 40.f, 1.f))
	, m_vEndScale(Vec3(10.f, 10.f, 1.f))
	, m_ePOV(SHADER_POV::PARTICLE)
{
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl");
	m_pUpdateShader = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_pParticleBuffer = new CStructuredBuffer;
}

CParticleSystem::~CParticleSystem()
{
	delete m_pParticleBuffer;
}

void CParticleSystem::awake()
{
	int i = GetMaxCount();
	SetMaxParticleCount(++i);
}

void CParticleSystem::finalupdate()
{
	if(m_ePOV==SHADER_POV::PARTICLE)
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl");

	else if (m_ePOV == SHADER_POV::DEFERRED_PARTICLE)
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefferedParticleRenderMtrl");



	m_fAccTime += fDT;
	if (m_fAccTime >= m_fFrequency)
	{
		m_fAccTime = 0.f;
		m_iAliveCount = 2;
	}
	else
	{
		m_iAliveCount = 0;
	}

	Vec3 vPos = Transform()->GetWorldPos();

	m_pUpdateShader->SetMaxParticle(m_iMaxCount);
	m_pUpdateShader->SetAliveCount(m_iAliveCount);
	m_pUpdateShader->SetMinLifeTime(m_fMinLifeTime);
	m_pUpdateShader->SetMaxLifeTime(m_fMaxLifeTime);
	m_pUpdateShader->SetMinSpeed(m_fMinSpeed);
	m_pUpdateShader->SetMaxSpeed(m_fMaxSpeed);
	m_pUpdateShader->SetParticelCreateRange(m_vCreateRange);
	m_pUpdateShader->SetParticleWorldPos(Transform()->GetWorldPos());
	m_pUpdateShader->SetParticleBuffer(m_pParticleBuffer);


	m_pUpdateShader->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	// 렌더링에 필요한 데이터 전달
	m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vStartScale);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vEndScale);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_2, &m_vStartColor);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_3, &m_vEndColor);

	m_pMtrl->SetData(SHADER_PARAM::TEX_0, m_pTex.Get());

	// 리소스 바인딩
	m_pMtrl->UpdateData();
	m_pMesh->UpdateData(0);
	m_pParticleBuffer->UpdateData(100);

	// 렌더
	m_pMesh->render_particle(m_iMaxCount);

	m_pParticleBuffer->Clear();
}

void CParticleSystem::SetMaxParticleCount(int _iMaxCount)
{
	if (m_iMaxCount < _iMaxCount)
	{
		m_pParticleBuffer->CreateRW(sizeof(tParticle), _iMaxCount, nullptr);
	}

	m_iMaxCount = _iMaxCount;
}


void CParticleSystem::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_iMaxCount, sizeof(int), 1, _pFile);

	fwrite(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMinSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fFrequency, sizeof(float), 1, _pFile);
	fwrite(&m_vCreateRange, sizeof(Vec4), 1, _pFile);

	fwrite(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vStartScale, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vEndScale, sizeof(Vec4), 1, _pFile);

	SaveResRefInfo<CTexture>(m_pTex, _pFile);
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_iMaxCount, sizeof(int), 1, _pFile);
	SetMaxParticleCount(m_iMaxCount);

	fread(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMinSpeed, sizeof(float), 1, _pFile);
	fread(&m_fMaxSpeed, sizeof(float), 1, _pFile);
	fread(&m_fFrequency, sizeof(float), 1, _pFile);
	fread(&m_vCreateRange, sizeof(Vec4), 1, _pFile);

	fread(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vStartScale, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndScale, sizeof(Vec4), 1, _pFile);

	LoadResRefInfo<CTexture>(m_pTex, _pFile);
}
#include "pch.h"
#include "CRenderMgr.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CCamera.h"
#include "CLight3D.h"
#include "CTransform.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CCopyShaderCS.h"
#include "CHorizontalBlur.h"
#include "CVerticalBlur.h"

#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_iCurNoise(0)
	, m_fAccTime(0.f)
	, m_pLight2DBuffer(nullptr)
	, m_pLight3DBuffer(nullptr)
	, m_pToolCam(nullptr)
	, m_pAnim2DBuffer(nullptr)
	, m_arrMRT{}
{
}

CRenderMgr::~CRenderMgr()
{
	SAFE_DELETE(m_pLight2DBuffer);
	SAFE_DELETE(m_pLight3DBuffer);
	SAFE_DELETE(m_pAnim2DBuffer);
	CGameObject* pUICam = m_UICam->GetObj();
	SAFE_DELETE(pUICam);
	Safe_Delete_Array(m_arrMRT);
}

void CRenderMgr::init()
{
	Ptr<CTexture> pTex = nullptr;
	pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	m_vecNoiseTex.push_back(pTex);
	/*pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	m_vecNoiseTex.push_back(pTex);*/

	g_global.vNoiseResolution = Vec2(pTex->Width(), pTex->Height());

	// Light StructuredBuffer Create
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 1, nullptr);

	m_pLight3DBuffer = new CStructuredBuffer;
	m_pLight3DBuffer->Create(sizeof(tLightInfo), 1, nullptr);

	// Animation 2D Buffer
	m_pAnim2DBuffer = new CStructuredBuffer;
	m_pAnim2DBuffer->Create(sizeof(tAnim2D), 1, nullptr);

	// CopyTarget Texture Create
	Vec2 vResolution = Vec2((float)CDevice::GetInst()->GetBufferResolution().x, (float)CDevice::GetInst()->GetBufferResolution().y);
	m_pCopyTarget = CResMgr::GetInst()->CreateTexture(L"PostEffectTargetTex"
		, (UINT)vResolution.x, (UINT)vResolution.y
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM, true);

	m_pSwapChainTarget = CResMgr::GetInst()->CreateTexture(L"SwapChianPassTex"
		, (UINT)vResolution.x, (UINT)vResolution.y
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM, true);

	// PostEffectMtrl �� CopyTexture ����
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PostEffectMtrl");

	pMtrl->SetData(SHADER_PARAM::TEX_0, m_pCopyTarget.Get());

	// MRT ����
	CreateMRT();
}

void CRenderMgr::update()
{
	// =================
	// Noise Texture ��ü
	// =================
	m_fAccTime += fDT;
	if (m_fAccTime > 0.1f)
	{
		m_fAccTime = 0.f;
		m_iCurNoise += 1;
		if (m_vecNoiseTex.size() == m_iCurNoise)
			m_iCurNoise = 0;
	}

	m_vecNoiseTex[m_iCurNoise]->UpdateData(70, (UINT)PIPELINE_STAGE::PS_ALL);

	// ===================
	// ���� ������ ������Ʈ
	// ===================
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
	g_global.iLight3DCount = (UINT)m_vecLight3D.size();

	vector<tLightInfo> vecLight[2];
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight[0].push_back(m_vecLight2D[i]->GetInfo());
	}
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vecLight[1].push_back(m_vecLight3D[i]->GetInfo());
	}

	UpdateLights(vecLight[0], m_pLight2DBuffer, 61);
	UpdateLights(vecLight[1], m_pLight3DBuffer, 62);

	m_vecLight2D.clear();
	m_vecLight3D.clear();

	// ==================
	// ���� ������ ������Ʈ
	// ==================
	static const CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL_VALUE);
	pGlobalBuffer->SetData(&g_global);
	pGlobalBuffer->UpdateData((UINT)PIPELINE_STAGE::PS_ALL);
}

void CRenderMgr::render()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	if (nullptr == pCurScene)
		return;

	// MRT Ŭ����
	ClearMRT();

	if (SCENE_STATE::PLAY == pCurScene->GetState())
	{
		render_play();
	}
	else
	{
		render_tool();
	}

	m_vecCam.clear();
}


void CRenderMgr::render_play()
{
	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();

		GetMRT(MRT_TYPE::DEFERRED)->OMSet();
		m_vecCam[i]->render_deferred();

		CRenderMgr::GetInst()->render_shadowmap();

		Ptr<CTexture> pSrcTex = CResMgr::GetInst()->FindDataTexture(L"DiffuseLightTargetTex");
		Ptr<CTexture> pDstTex = CResMgr::GetInst()->FindDataTexture(L"BloomLightTargetTex");
	
		CustomCopy(pSrcTex, pDstTex, COPY_TYPE::BLOOM);

		// Lighting
		GetMRT(MRT_TYPE::LIGHT)->OMSet();

		// main camera ���� view, proj ����
		g_transform.matView = GetCurCam()->GetViewMat();
		g_transform.matViewInv = GetCurCam()->GetViewInvMat();
		g_transform.matProj = GetCurCam()->GetProjMat();

		for (size_t i = 0; i < m_vecLight3D.size(); ++i)
		{
			m_vecLight3D[i]->render();
		}

		// ��ü ����, Lights Merge
		GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
		LightMerge();
	}

	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	
	m_vecCam[0]->render_forward();

	Ptr<CTexture> pSrcTex2 = CResMgr::GetInst()->FindDataTexture(L"SwapChainRenderTargetTex");
	Ptr<CTexture> pDstTex2 = CResMgr::GetInst()->FindDataTexture(L"DownSampleTex");
	CONTEXT->CopyResource(m_pSwapChainTarget->GetTex2D().Get(), pSrcTex2->GetTex2D().Get());

	CustomCopy(m_pSwapChainTarget, pDstTex2, COPY_TYPE::DOF);


	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	m_vecCam[0]->render_posteffect();

	if (nullptr != m_UICam)
	{
		m_UICam->Transform()->finalupdate();
		m_UICam->finalupdate();
		m_UICam->render_UI();
	}

	g_transform.matView = GetCurCam()->GetViewMat();
	g_transform.matViewInv = GetCurCam()->GetViewInvMat();
	g_transform.matProj = GetCurCam()->GetProjMat();
}

void CRenderMgr::render_tool()
{
	if (nullptr == m_pToolCam)
		return;

	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	// ��ü �з�
	m_pToolCam->SortObject();

	// Deferred ��ü �׸���
	GetMRT(MRT_TYPE::DEFERRED)->OMSet();
	m_pToolCam->render_deferred();

	// ShadowMap
	CRenderMgr::GetInst()->render_shadowmap();

	Ptr<CTexture> pSrcTex = CResMgr::GetInst()->FindDataTexture(L"DiffuseLightTargetTex");
	Ptr<CTexture> pDstTex = CResMgr::GetInst()->FindDataTexture(L"BloomLightTargetTex");

	CustomCopy(pSrcTex, pDstTex ,COPY_TYPE::BLOOM);
	
	// Lighting
	GetMRT(MRT_TYPE::LIGHT)->OMSet();

	// main camera ���� view, proj ����
	g_transform.matView = m_pToolCam->GetViewMat();
	g_transform.matViewInv = m_pToolCam->GetViewInvMat();
	g_transform.matProj = m_pToolCam->GetProjMat();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->render();
	}

	// ��ü ����, Lights Merge
	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	LightMerge();

	// forward ��ü �׸���
	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	m_pToolCam->render_forward();

	pSrcTex = CResMgr::GetInst()->FindDataTexture(L"SwapChainRenderTargetTex");
	pDstTex = CResMgr::GetInst()->FindDataTexture(L"DownSampleTex");
	CONTEXT->CopyResource(m_pSwapChainTarget->GetTex2D().Get(), pSrcTex->GetTex2D().Get());

	CustomCopy(m_pSwapChainTarget, pDstTex, COPY_TYPE::DOF);

	GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

	m_pToolCam->render_posteffect();

	if (nullptr != m_UICam)
	{
		m_UICam->Transform()->finalupdate();
		m_UICam->finalupdate();
		m_UICam->render_UI();
	}
		
	g_transform.matView = GetCurCam()->GetViewMat();
	g_transform.matViewInv = GetCurCam()->GetViewInvMat();
	g_transform.matProj = GetCurCam()->GetProjMat();
}


void CRenderMgr::render_shadowmap()
{
	m_arrMRT[(UINT)MRT_TYPE::DYNAMIC_SHADDOWMAP]->OMSet();

	// ���� �������� ���̸� �׸�
	for (UINT i = 0; i < m_vecLight3D.size(); ++i)
	{
		if (m_vecLight3D[i]->GetInfo().eType != LIGHT_TYPE::DIR)
			continue;

		m_vecLight3D[i]->render_shadowmap();
	}

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(); // ����ī�޶� ���̷� �ǵ����� ���ؼ�
}




void CRenderMgr::CopyTarget()
{
	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindDataTexture(L"SwapChainRenderTargetTex");
	CONTEXT->CopyResource(m_pCopyTarget->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}


CCamera* CRenderMgr::GetCurCam()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	if (pCurScene->GetState() == SCENE_STATE::PLAY)
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_pToolCam;
	}
}


void CRenderMgr::CustomCopy(Ptr<CTexture> _SrcTex, Ptr<CTexture> _DstTex, COPY_TYPE _eType)
{
	CCopyShaderCS* CopyShader = (CCopyShaderCS*)CResMgr::GetInst()->GetRes(RES_TYPE::SHADER).find(L"CopyTextureShader")->second;

	CopyShader->SetSrcTex(_SrcTex);
	CopyShader->SetDestTex(_DstTex);

	CopyShader->SetCopyType(_eType);
	CopyShader->Excute();

	CopyShader->Clear();
}

void CRenderMgr::LightMerge()
{
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	static Ptr<CMaterial> pMergeMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"LightMergeMtrl");

	pMergeMtrl->UpdateData();

	pRectMesh->UpdateData(0);
	pRectMesh->render(0);

	pMergeMtrl->Clear();
}

void CRenderMgr::UpdateLights(vector<tLightInfo>& _vec, CStructuredBuffer* _buffer, UINT _iRegisterNum)
{
	if (_buffer->GetElementCount() < _vec.size())
		_buffer->Create(sizeof(tLightInfo), (UINT)_vec.size(), nullptr);

	_buffer->SetData(_vec.data(), sizeof(tLightInfo), (UINT)_vec.size());
	_buffer->UpdateData(_iRegisterNum);
}

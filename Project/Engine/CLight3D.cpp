#include "pch.h"
#include "CLight3D.h"

#include "CTransform.h"
#include "CRenderMgr.h"

#include "CResMgr.h"


#include "CTransform.h"
#include "CLightCamera.h"

#include "CCopyShaderCS.h"



CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)	
	, m_pCamObj(nullptr)
{
	m_pCamObj = new CGameObject;
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->AddComponent(new CLightCamera);

	SetLightType(LIGHT_TYPE::DIR);

	// 광원이 관리하는 카메라는 렌더매니저에 등록되면 안된다.	
	m_pCamObj->Camera()->SetLayerAllCheck(); // 모든 레이어를 찍는다(shadow map)
}

CLight3D::CLight3D(const CLight3D& _other)
	: CComponent(_other)
	, m_info(_other.m_info)
	, m_iLightIdx(-1)
	, m_pMesh(_other.m_pMesh)
	, m_pMtrl(_other.m_pMtrl)
	, m_pCamObj(nullptr)
{
	m_pCamObj = _other.m_pCamObj->Clone();
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_pCamObj);
}

void CLight3D::finalupdate()
{
	m_info.vWorldPos = Transform()->GetWorldPos();
	m_info.vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);	


	// 광원 관리 카메라도 광원과 같은 Transform 정보를 가지게 한다.
	m_pCamObj->Transform()->SetLocalPos(Transform()->GetLocalPos());
	m_pCamObj->Transform()->SetLocalScale(Transform()->GetLocalScale());
	m_pCamObj->Transform()->SetLocalRot(Transform()->GetLocalRot());

	m_pCamObj->finalupdate_ex(); // 광원카메라는 렌더매니저에 등록하지 않게 해두었다.
}

void CLight3D::render()
{
	Transform()->UpdateData();

	m_pMtrl->SetData(SHADER_PARAM::INT_0, &m_iLightIdx);

	// Directional Light 인 경우
	if (LIGHT_TYPE::DIR == m_info.eType)
	{
		//CCopyShaderCS* CopyShader = (CCopyShaderCS*)CResMgr::GetInst()->GetRes(RES_TYPE::SHADER).find(L"CopyTextureShader")->second;
		//
		//// 스태틱 쉐도우 로드, 세팅
		//CopyShader->SetStaticShadowTex(m_StaticShadowTex);
		//
		//Ptr<CTexture> ShadowMapTex = CResMgr::GetInst()->FindDataTexture(L"ShadowMapTargetTex");
		//// 다이나믹 쉐도우 세팅
		//CopyShader->SetSrcTex(ShadowMapTex.Get());
		//
		//// 광원 시점 ShadowMap 깊이정보 텍스쳐
		//Ptr<CTexture> FinalShadowMapTex = CResMgr::GetInst()->FindDataTexture(L"FinalShadowMapTargetTex");
		//
		//// 스태틱 쉐도우 + 다이나믹 쉐도우를 합칠 최종 쉐도우텍스쳐 세팅
		//// 스태틱 쉐도우 + 다이나믹 쉐도우 복사
		//CopyShader->SetDestTex(FinalShadowMapTex.Get());
		//CopyShader->UpdateData();
		//CopyShader->Excute();
		//
		//m_pMtrl->SetData(SHADER_PARAM::TEX_3, FinalShadowMapTex.Get());
		//
		//// 광원으로 투영시키기 위한 광원 카메라의 View, Proj 행렬
		//Matrix MatVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		//m_pMtrl->SetData(SHADER_PARAM::MAT_0, &MatVP);
		//
		//CopyShader->Clear();

#pragma region Origin

		// 광원 시점 ShadowMap 깊이정보 텍스쳐
		Ptr<CTexture> pShadowMapTex = CResMgr::GetInst()->FindDataTexture(L"ShadowMapTargetTex");
		m_pMtrl->SetData(SHADER_PARAM::TEX_3, pShadowMapTex.Get());

		// 광원으로 투영시키기 위한 광원 카메라의 View, Proj 행렬
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		m_pMtrl->SetData(SHADER_PARAM::MAT_0, &matVP);
#pragma endregion
	}

	m_pMtrl->SetData(SHADER_PARAM::INT_0, &m_iLightIdx);	
	m_pMtrl->UpdateData();

	m_pMesh->UpdateData(0);
	m_pMesh->render(0);

	m_pMtrl->Clear();
}


void CLight3D::render_shadowmap()
{
	// 광원 시점에서 물체 분류 및 깊이 그리기
	((CLightCamera*)m_pCamObj->Camera())->SortShadowObject();
	((CLightCamera*)m_pCamObj->Camera())->render_shadowmap();
}

void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_info.eType = _eType;

	if (LIGHT_TYPE::DIR == _eType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");

		m_pMtrl->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindDataTexture(L"NormalTargetTex").Get());
		m_pMtrl->SetData(SHADER_PARAM::TEX_1, CResMgr::GetInst()->FindDataTexture(L"PositionTargetTex").Get());

		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pCamObj->Camera()->SetScale(Vec2(1.f, 1.f));
		m_pCamObj->Camera()->SetFar(100000.f);
		m_pCamObj->Camera()->SetOrthographicProjRange(Vec2(23000.f, 23000.f));
	}	
	else if (LIGHT_TYPE::POINT == _eType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");

		m_pMtrl->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindDataTexture(L"NormalTargetTex").Get());
		m_pMtrl->SetData(SHADER_PARAM::TEX_1, CResMgr::GetInst()->FindDataTexture(L"PositionTargetTex").Get());
	}
	else
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");

		m_pMtrl->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindDataTexture(L"NormalTargetTex").Get());
		m_pMtrl->SetData(SHADER_PARAM::TEX_1, CResMgr::GetInst()->FindDataTexture(L"PositionTargetTex").Get());
	}
}

void CLight3D::SetRange(float _fRange)
{
	m_info.fRange = _fRange;
	Transform()->SetWorldScale(Vec3(_fRange, _fRange, _fRange));
}



void CLight3D::SetLightInfo(tLightInfo Info)
{
	m_info.color.vAmb = Info.color.vAmb;
	m_info.color.vDiff = Info.color.vDiff;
	m_info.color.vSpec = Info.color.vSpec;

	m_info.eType = Info.eType;
	m_info.fAngle = Info.fAngle;
	m_info.fRange = Info.fRange;
}

void CLight3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_info, sizeof(tLightInfo), 1, _pFile);
}

void CLight3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_info, sizeof(tLightInfo), 1, _pFile);
}
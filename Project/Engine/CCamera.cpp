#include "pch.h"
#include "CCamera.h"

#include "CTransform.h"
#include "CDevice.h"
#include "CCore.h"

#include "CKeyMgr.h"

#include "CRenderMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CMeshRender.h"
#include "CParticleSystem.h"
#include "CCollider2D.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CTerrain.h"
#include "CFrustumSphere.h"
#include "CUI.h"

#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CInstancingBuffer.h"




CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_frustum(this)
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_vScale(Vec2(3.2f, 3.2f))
	, m_fFOV(XM_PI / 2.f)
	, m_tRay{}
	, m_fFar(1000.f)
	, m_iLayerCheck(0)
	, m_eCamType(CAMERA_TYPE::NORMAL_CAM)
{
	m_frustum.init();
	POINT ptRes = CDevice::GetInst()->GetBufferResolution();
	m_vProjRange = Vec2((float)ptRes.x, (float)ptRes.y);
}

CCamera::~CCamera()
{
}

void CCamera::update()
{

}

void CCamera::finalupdate()
{
	CalViewMat();

	CalProjMat();

	if (m_eCamType == CAMERA_TYPE::NORMAL_CAM)
	{
		CalRay();

		m_frustum.finalupdate();

		CRenderMgr::GetInst()->RegisterCamera(this);

		g_global.g_CamWorldPos = Transform()->GetLocalPos();
		g_global.g_vCamUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	}
	else if (m_eCamType == CAMERA_TYPE::UI_CAM)
	{
		CRenderMgr::GetInst()->RegisterUICamera(this);
	}
	
}


void CCamera::SetLayerCheck(UINT _iLayerIdx, bool _bAdd)
{
	assert(!(_iLayerIdx >= MAX_LAYER));

	if (_bAdd)
	{
		m_iLayerCheck |= 1 << _iLayerIdx;
	}
	else
	{
		m_iLayerCheck &= ~(1 << _iLayerIdx);
	}
}

void CCamera::SortObject()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();

	m_vecParticle.clear();
	m_vecDeferredParticle.clear();
	m_vecPostEffect.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				if (vecObj[j]->UI())
				{
					if (vecObj[j]->UI()->IsEnable())
					{
						//tool 혹은 vecCam[0]이 물체분류중 ui컴포넌트 객체를 만나면 ui카메라의 ui벡터에 넣음 
						CCamera* pUICam = CRenderMgr::GetInst()->GetUICam();

						if (pUICam)
							pUICam->GetUIvector().push_back(vecObj[j]);

						continue;
					}
					else
					{
						continue;
					}

				}

				if (vecObj[j]->ParticleSystem()&& vecObj[j]->ParticleSystem()->IsEnable())
				{
					if (vecObj[j]->ParticleSystem()->GetMaterial()->GetShader()->GetPOV() == SHADER_POV::PARTICLE)
					{
						m_vecParticle.push_back(vecObj[j]);
						continue;
					}

					else if (vecObj[j]->ParticleSystem()->GetMaterial()->GetShader()->GetPOV() == SHADER_POV::DEFERRED_PARTICLE)
					{
						m_vecDeferredParticle.push_back(vecObj[j]);
						continue;
					}
				}

				if (nullptr == vecObj[j]->MeshRender() || nullptr == vecObj[j]->MeshRender()->GetMesh())
				{
					if (nullptr == vecObj[j]->Terrain())
					{
						continue;
					}
				}


				// 절두체 테스트(구현)
				if (vecObj[j]->GetParent())
				{
					if (vecObj[j]->GetParent()->FrustumSphere()&& vecObj[j]->GetParent()->IsFrustum())
					{
						Vec3 vWorldPos = vecObj[j]->Transform()->GetWorldPos();
						Vec3 vOffSetPos = vecObj[j]->GetParent()->FrustumSphere()->GetOffSetPos();
						float Radius = vecObj[j]->GetParent()->FrustumSphere()->GetRadius();

						if (!m_frustum.CheckFrustumSphere(vWorldPos,vOffSetPos, Radius))
						{
							continue;
						}
					}
				}

				UINT iMtrlCount = 0;

				// 메테리얼 개수만큼 반복
				if(vecObj[j]->MeshRender())
					iMtrlCount = vecObj[j]->MeshRender()->GetMtrlCount();
				else if (vecObj[j]->Terrain())
					iMtrlCount = 1;

				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// Material 을 참조하고 있지 않거나, Material 에 아직 Shader 를 연결하지 않은 상태라면 Continue

					Ptr<CMaterial> pMtrl = nullptr;

					if (vecObj[j]->MeshRender())
						pMtrl = vecObj[j]->MeshRender()->GetSharedMaterial(iMtrl);
					else if (vecObj[j]->Terrain())
						pMtrl = vecObj[j]->Terrain()->GetSharedMaterial();

					if (nullptr == pMtrl || pMtrl->GetShader() == nullptr || false==vecObj[j]->MeshRender()->IsEnable())
						continue;

					// Shader 의 POV 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, vector<tInstObj>>* pMap = NULL;
					if (pMtrl->GetShader()->GetPOV() == SHADER_POV::DEFERRED)
						pMap = &m_mapInstGroup_D;
					else if (pMtrl->GetShader()->GetPOV() == SHADER_POV::FORWARD)
						pMap = &m_mapInstGroup_F;
					else if (pMtrl->GetShader()->GetPOV() == SHADER_POV::POSTEFFECT)
					{
						m_vecPostEffect.push_back(vecObj[j]);
						continue;
					}
			
					else
					{
						assert(nullptr);
						continue;
					}

					uInstID uID = {};

					if (vecObj[j]->MeshRender()) {
						uID.llID = vecObj[j]->MeshRender()->GetInstID(iMtrl);

						// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							//같은 instID를 가지고 있지 않다면 새롭게 instID와 벡터를 짝으로 해서 
							//map에 insert 하고 , 위 벡터에 게임오브젝트를 push 한다 
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObj[j], iMtrl }}));
						}
						else
						{
							//만약 같은 메쉬 , 같은 메테리얼 ,같은 부위를 렌더링할 대상이 들어왔다면 
							//해당 instID를 사용하는 벡터에 같이 넣어준다 
							iter->second.push_back(tInstObj{ vecObj[j], iMtrl });
						}
					}
					else if (vecObj[j]->Terrain())
					{
						uID.llID = vecObj[j]->Terrain()->GetInstID(0);

						// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = m_mapInstGroup_D.find(uID.llID);
						if (iter == m_mapInstGroup_D.end())
						{
							m_mapInstGroup_D.insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObj[j], 0 }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObj[j], 0 });
						}
					}
				}
			}
		}
	}
}


void CCamera::render_deferred()
{
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetInstVS() == nullptr
			|| pair.second[0].pObj->Terrain())
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = nullptr;
		if(pObj->MeshRender())
			 pMesh = pObj->MeshRender()->GetMesh();
		/*else if (pObj->Terrain())
			 pMesh = pObj->Terrain()->GetMesh();*/

		Ptr<CMaterial> pMtrl = nullptr;

		if (pObj->MeshRender())
			pMtrl = pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx);
		/*else if (pObj->Terrain())
			pMtrl = pObj->Terrain()->GetSharedMaterial();*/

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->UpdateData(1);
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		pMtrl->Clear();
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		for (auto& instObj : pair.second)
		{
			if (instObj.pObj->Terrain())
			{
				instObj.pObj->Terrain()->render();
			}
			else
			{
				instObj.pObj->MeshRender()->render(instObj.iMtrlIdx);
			}
		}
	}


	for (UINT i = 0; i < m_vecDeferredParticle.size(); ++i)
	{
		//디퍼드 파티클 렌더
		m_vecDeferredParticle[i]->ParticleSystem()->render();
	}
}

void CCamera::render_forward()
{
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetInstVS() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->MeshRender()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->UpdateData(1);
		pMesh->render_instancing(pair.second[0].iMtrlIdx);
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		for (auto& instObj : pair.second)
		{
			instObj.pObj->MeshRender()->render(instObj.iMtrlIdx);
		}
	}

	// Particle Render
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->ParticleSystem()->render();
		
		if (m_vecParticle[i]->Collider2D())
			m_vecParticle[i]->Collider2D()->render();
	}
}

void CCamera::render_posteffect()
{
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecPostEffect.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyTarget();

		Ptr<CTexture> pDownSampleTex = CResMgr::GetInst()->FindDataTexture(L"DownSampleTex");
		Ptr<CTexture> pDOFDepthTex = CResMgr::GetInst()->FindDataTexture(L"DOFTex");
		Ptr<CTexture> BloomTex = CResMgr::GetInst()->FindDataTexture(L"BloomLightTargetTex");

		m_vecPostEffect[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_1, pDownSampleTex.Get());
		m_vecPostEffect[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_2, pDOFDepthTex.Get());
		m_vecPostEffect[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_3, BloomTex.Get());

		m_vecPostEffect[0]->MeshRender()->render();
	}
}

void CCamera::render_UI()
{
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->MeshRender()->render();
	}

	m_vecUI.clear();
}

void CCamera::CalViewMat()
{
	// View 행렬
	// View 행렬 이동파트(카메라 위치 역)
	Vec3 vCamPos = -Transform()->GetWorldPos();
	Matrix matTranslation = XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z);

	// View 행렬 회전파트(우, 상, 전 벡터의 전치)
	Matrix matRot = XMMatrixIdentity();
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matRot._11 = vRight.x;	matRot._12 = vUp.x; matRot._13 = vFront.x;
	matRot._21 = vRight.y;	matRot._22 = vUp.y;	matRot._23 = vFront.y;
	matRot._31 = vRight.z;	matRot._32 = vUp.z;	matRot._33 = vFront.z;

	m_matView = matTranslation * matRot;
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalProjMat()
{
	// Proj 행렬
	// FOV(file of view) 시야각
	// Aspect Ratio 종횡비
	if (PROJ_TYPE::PERSPECTIVE == m_eProjType)
	{
		POINT ptResolution = CDevice::GetInst()->GetBufferResolution();
		float fAR = (float)ptResolution.x / (float)ptResolution.y;
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 4.f, fAR, 1.f, m_fFar);
	}
	else
	{
		POINT ptRes = CCore::GetInst()->GetWndResolution();
		m_matProj = XMMatrixOrthographicLH(m_vProjRange.x * m_vScale.x, m_vProjRange.y * m_vScale.y, 1.f, m_fFar);

		POINT ptResolution = CDevice::GetInst()->GetBufferResolution();
		float fAR = (float)ptResolution.x / (float)ptResolution.y;
		m_matProjPers = XMMatrixPerspectiveFovLH(XM_PI / 2.f, fAR, 1.f, m_fFar);
		m_matProjPersInv= XMMatrixInverse(nullptr, m_matProj);
	}

	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);
}

void CCamera::CalRay()
{
	if (PROJ_TYPE::PERSPECTIVE == m_eProjType)
	{
		// SwapChain 타겟의 ViewPort 정보
		CMRT* pSwapChain = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
		D3D11_VIEWPORT tVP = {};
		pSwapChain->GetViewPort(&tVP);

		//  현재 마우스 좌표
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();


		// 직선은 카메라의 좌표를 반드시 지난다.
		m_tRay.vPoint = Transform()->GetWorldPos();

		// view space 에서의 방향
		m_tRay.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
		m_tRay.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
		m_tRay.vDir.z = 1.f;

		// world space 에서의 방향
		m_tRay.vDir = XMVector3TransformNormal(m_tRay.vDir, m_matViewInv);
		m_tRay.vDir.Normalize();
	}
	else 
	{
		// SwapChain 타겟의 ViewPort 정보
		CMRT* pSwapChain = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
		D3D11_VIEWPORT tVP = {};
		pSwapChain->GetViewPort(&tVP);

		//  현재 마우스 좌표
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();


		// 직선은 카메라의 좌표를 반드시 지난다.
		m_tRay.vPoint = Transform()->GetWorldPos();

		// view space 에서의 방향
		m_tRay.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProjPers._31) / m_matProjPers._11;
		m_tRay.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProjPers._32) / m_matProjPers._22;
		m_tRay.vDir.z = 1.f;

		// world space 에서의 방향
		m_tRay.vDir = XMVector3TransformNormal(m_tRay.vDir, m_matViewInv);
		m_tRay.vDir.Normalize();
	}
}

void CCamera::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_eProjType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vec2), 1, _pFile);
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_iLayerCheck, sizeof(UINT), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);
}

void CCamera::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_eProjType, sizeof(UINT), 1, _pFile);
	fread(&m_vScale, sizeof(Vec2), 1, _pFile);
	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_iLayerCheck, sizeof(UINT), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);
}

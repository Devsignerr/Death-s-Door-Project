#include "pch.h"
#include "CFBXLoader.h"
#include "CMesh.h"

#include "CResMgr.h"
#include "CPathMgr.h"

CFBXLoader::CFBXLoader()
	: m_pManager(NULL)
	, m_pScene(NULL)
	, m_pImporter(NULL)
{
}

CFBXLoader::~CFBXLoader()
{
	m_pScene->Destroy();
	m_pManager->Destroy();
		
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		SAFE_DELETE(m_vecBone[i]);
	}

	for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
	{
		SAFE_DELETE(m_vecAnimClip[i]);
	}

	for (int i = 0; i < m_arrAnimName.Size(); ++i)
	{
		SAFE_DELETE(m_arrAnimName[i]);
	}
}

void CFBXLoader::CreateNavMesh(int _iIdx)
{
	UINT ContainerCnt = _iIdx;

	for (UINT a = 0; a < ContainerCnt; ++a)
	{
		const tContainer* container = &GetContainer(a);

		//정점 갯수. 반드시 3의 배수여야 한다 . 
		UINT iVtxCount = (UINT)container->vecPos.size();

		CMesh* pMesh = CResMgr::GetInst()->GetMeshVec()[a];
		pMesh->SetNavMesh(true);

		//메쉬 폴리곤의 갯수가 100개라면 아래 벡터의 크기도 100
		//현재 불러온 컨테이너의 네비메쉬 
		vector<tNavMeshNode>& vecNavMeshNode = pMesh->GetMeshNodeVec();

		vecNavMeshNode.clear();

		UINT PolygonCnt = iVtxCount / 3;

		//정점의 위치를 모두 받아온다 . 정점 3개당 NavMeshNode 구조체 1개에 들어간다 .
		UINT VecIdx = 0;
		UINT VTXPosIdx = 0;

		for (UINT p = 0; p < PolygonCnt; ++p)
		{
			//네비메쉬 벡터 인덱스는 정점 3번당 1번씩 증가함 
			//정점배열 인덱스는 정점 3번당 0으로 초기화됨 
				
			tNavMeshNode Node = {};

			for (UINT v = 0; v < 3; v++) 
			{
				UINT Idx = p *3 + v;

				//정점의 위치를 벡터에 모두 입력함 			
				Node.VertexPosition[v] = container->vecPos[Idx];
				Node.NodeIdx = p;
			}
			//노드의 인덱스를 기준으로 정렬할것이다 . 
			vecNavMeshNode.push_back(Node);
		}

		for (UINT i = 0; i < PolygonCnt; ++i)
		{
			for (UINT j = 0; j < PolygonCnt; ++j)
			{
				for (UINT k = 0; k < 3; k++)
				{
					for (UINT m = 0; m < 3; m++)
					{
						//자신과 비교하지 않으면서 , 다른 메쉬의 어떤 정점과 내 메쉬의 어떤 정점의 위치가 같을경우 인접노드다 .
						if (i != j && vecNavMeshNode[i].VertexPosition[k] == vecNavMeshNode[j].VertexPosition[m])
						{
							vecNavMeshNode[i].VecNearNodeIdx.push_back(vecNavMeshNode[j].NodeIdx);
						}
					}
				}
			}
		}

		for (UINT n = 0; n < PolygonCnt; n++)
		{
			//중복 원소 제거 
			vecNavMeshNode[n].VecNearNodeIdx.erase(std::unique(
				vecNavMeshNode[n].VecNearNodeIdx.begin(),
				vecNavMeshNode[n].VecNearNodeIdx.end()),
				vecNavMeshNode[n].VecNearNodeIdx.end());
		}

	}
}

void CFBXLoader::init()
{
	m_pManager = FbxManager::Create();
	if (NULL == m_pManager)
		assert(NULL);

	FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIOSettings);

	m_pScene = FbxScene::Create(m_pManager, "");
	if (NULL == m_pScene)
		assert(NULL);
}

void CFBXLoader::LoadFbx(const wstring & _strPath, FBXLOAD_TYPE _LoadType)
{
	m_vecContainer.clear();
	m_pImporter = FbxImporter::Create(m_pManager, "");
	string strPath(_strPath.begin(), _strPath.end());
		
	if (!m_pImporter->Initialize(strPath.c_str(), -1, m_pManager->GetIOSettings()))
		assert(nullptr);

	m_pImporter->Import(m_pScene);

	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);
	LoadSkeleton(m_pScene->GetRootNode());
	m_pScene->FillAnimStackNameArray(m_arrAnimName);


	// Animation Clip 정보
	LoadAnimationClip();

	// 삼각화(Triangulate)
	Triangulate(m_pScene->GetRootNode());

	// 메쉬 데이터 얻기
	LoadMeshDataFromNode(m_pScene->GetRootNode(), _LoadType);

	m_pImporter->Destroy();

	//네비메쉬는 아래 정보가 필요하지 않음 
	if (FBXLOAD_TYPE::NAVMESH_LOAD !=_LoadType) 
	{
		// 필요한 텍스쳐 로드
		LoadTexture();

		// 필요한 메테리얼 생성
		CreateMaterial();
	}
}

void CFBXLoader::LoadMeshDataFromNode(FbxNode * _pNode, FBXLOAD_TYPE _LoadType)
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();
	FbxMesh* pMesh = _pNode->GetMesh();

	FbxAMatrix matrix = _pNode->EvaluateGlobalTransform();
	FbxString MeshName = {};

	//메쉬가 없으면 여기에 들어올수 없음 
	if (pAttr && FbxNodeAttribute::eMesh == pAttr->GetAttributeType())
	{
		MeshName = _pNode->GetName();

		FbxAMatrix matrixGeo;

		matrixGeo.SetIdentity();
				
		const FbxVector4 lT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matrixGeo.SetT(lT);
		matrixGeo.SetR(lR);
		matrixGeo.SetS(lS);
		
		//본인 메쉬의 로컬 행렬 계산 
		FbxAMatrix localMatrix = _pNode->EvaluateLocalTransform();
		
		//부모 메쉬를 얻어옴 (최상위 부모는(메쉬가 아님) 아예 이 스코프에 들어올수조차 없어서 pParentNode는 nullptr이  아님이 보장됨 )				 
		FbxNode* pParentNode = _pNode->GetParent();

		//부모의 로컬 행렬을 계산함 
		FbxAMatrix parentMatrix = pParentNode->EvaluateLocalTransform();

		//더이상 부모가 없을때까지 타고 올라감 
		while ((pParentNode = pParentNode->GetParent()) != NULL)
		{
			//부모들의 매트릭스를 계산해서 총 부모 매트릭스를 계산함 
			parentMatrix = pParentNode->EvaluateLocalTransform() * parentMatrix;
		}

		//부모 * 자신 * 지오메트릭 행렬을 곱해 최종 행렬을 산출해냄 
		matrix = parentMatrix * localMatrix * matrixGeo;
	}

	if (NULL != pMesh) 
	{	
		if (_LoadType == FBXLOAD_TYPE::NAVMESH_LOAD)
		{
			LoadNavMesh(pMesh, MeshName);
		}
		else 
		{
			if (m_arrAnimName.Size() > 0)
			{
				//애니메이션이 하나라도 있다면 이 함수 호출. 
				//이 함수는 부모관계 영향없이 순수하게 애니메이션 전용 호출 함수임 
				LoadMesh(pMesh, MeshName);
			}
			else {
				//이 함수는 부모의 영향까지 계산하여 여러개의 메쉬를 올바른 위치에 생성하는 함수임 
				LoadMesh(pMesh, matrix, MeshName);
			}
		}		
	}	
	
	//네비메쉬는 재질정보가 필요없다 
	if (FBXLOAD_TYPE::NAVMESH_LOAD != _LoadType)
	{
		// 해당 노드의 재질정보 읽기
		UINT iMtrlCnt = _pNode->GetMaterialCount();
		if (iMtrlCnt > 0)
		{
			for (UINT i = 0; i < iMtrlCnt; ++i)
			{
				FbxSurfaceMaterial* pMtrlSur = _pNode->GetMaterial(i);
				LoadMaterial(pMtrlSur);
			}
		}
	}

	// 자식 노드 정보 읽기
	int iChildCnt = _pNode->GetChildCount();

	//메쉬의 이름정보도 들어있다 ! 
	const char* name = _pNode->GetName();

	//자식을 가지고 있다면 똑같은 처리를 하도록 함 
	for (int i = 0; i < iChildCnt; ++i)
	{
		LoadMeshDataFromNode(_pNode->GetChild(i), _LoadType);
	}
	
}

//==================================================================
//						네비메쉬 로드 
//==================================================================
void CFBXLoader::LoadNavMesh(FbxMesh* _pFbxMesh, FbxString _MeshName)
{
	m_vecContainer.push_back(tContainer{});
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];


	int iVtxCnt = _pFbxMesh->GetControlPointsCount();
	Container.Resize(iVtxCnt);

	FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();

	for (int i = 0; i < iVtxCnt; ++i)
	{
		Container.vecPos[i].x = (float)pFbxPos[i].mData[0];
		Container.vecPos[i].y = (float)pFbxPos[i].mData[2];
		Container.vecPos[i].z = (float)pFbxPos[i].mData[1];
	}

	// 폴리곤 개수
	int iPolyCnt = _pFbxMesh->GetPolygonCount();

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize(iMtrlCnt);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	int iPolySize = _pFbxMesh->GetPolygonSize(0);
	if (3 != iPolySize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

	for (int i = 0; i < iPolyCnt; ++i)
	{
		for (int j = 0; j < iPolySize; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
			arrIdx[j] = iIdx;

			//GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder);
			//GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			//GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			//GetUV(_pFbxMesh, &Container, iIdx, _pFbxMesh->GetTextureUVIndex(i, j));

			++iVtxOrder;
		}
		UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
	}
}

//==================================================================
//						애니메이션 메쉬 로드 
//==================================================================
void CFBXLoader::LoadMesh(FbxMesh * _pFbxMesh, FbxString _MeshName)
{
	const char* cName = _MeshName;

	m_vecContainer.push_back(tContainer{});
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

	string strName = _pFbxMesh->GetName();
	Container.strName = wstring(strName.begin(), strName.end());

	int iVtxCnt = _pFbxMesh->GetControlPointsCount();
	Container.Resize(iVtxCnt);

	FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();

	for (int i = 0; i < iVtxCnt; ++i)
	{
		Container.vecPos[i].x = (float)pFbxPos[i].mData[0];
		Container.vecPos[i].y = (float)pFbxPos[i].mData[2];
		Container.vecPos[i].z = (float)pFbxPos[i].mData[1];
	}

	// 폴리곤 개수
	int iPolyCnt = _pFbxMesh->GetPolygonCount();

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize(iMtrlCnt);	

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	int iPolySize = _pFbxMesh->GetPolygonSize(0);
	if (3 != iPolySize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

	for (int i = 0; i < iPolyCnt; ++i)
	{
		for (int j = 0; j < iPolySize; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
			arrIdx[j] = iIdx;

			GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder );
			GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetUV(_pFbxMesh, &Container, iIdx, _pFbxMesh->GetTextureUVIndex(i, j));
			
			++iVtxOrder;
		}
		UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
	}

	LoadAnimationData(_pFbxMesh, &Container);
}


//==================================================================
//						맵 , 오브젝트 로드 
//==================================================================
void CFBXLoader::LoadMesh(FbxMesh* _pFbxMesh, FbxAMatrix _mat, FbxString _MeshName)
{
	const char* cName = _MeshName;

	//컨테이너를 하나 생성해서 넣어준다 
	m_vecContainer.push_back(tContainer{});

	//현재 컨테이너는 가장 최근에 추가된 컨테이너다 
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

	//메쉬의 이름 셋팅 
	string strName = _pFbxMesh->GetName();
	Container.strName = wstring(strName.begin(), strName.end());

	//컨트롤 포인트 (정점) 갯수 를 가져온다 
	int iVtxCnt = _pFbxMesh->GetControlPointsCount();
	Container.Resize(iVtxCnt);

	//정점의 위치를 가져온다 ( 로컬에서 )
	FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();
	
	
	for (int i = 0; i < iVtxCnt; ++i)
	{
		FbxVector4 vec = _mat.MultT(pFbxPos[i].mData);

		Container.vecPos[i].x =-vec[0];
		Container.vecPos[i].y =vec[1];
		Container.vecPos[i].z =vec[2];
	}

	// 폴리곤 개수
	int iPolyCnt = _pFbxMesh->GetPolygonCount();

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize(iMtrlCnt);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	int iPolySize = _pFbxMesh->GetPolygonSize(0);
	if (3 != iPolySize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번

	for (int i = 0; i < iPolyCnt; ++i)
	{
		for (int j = 0; j < iPolySize; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
			arrIdx[j] = iIdx;

			GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
			GetUV(_pFbxMesh, &Container, iIdx, _pFbxMesh->GetTextureUVIndex(i, j));

			++iVtxOrder;
		}
		UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
		Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
	}

	LoadAnimationData(_pFbxMesh, &Container);
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial * _pMtrlSur)
{
	tFbxMaterial tMtrlInfo{};

	string str = _pMtrlSur->GetName();
	tMtrlInfo.strMtrlName = wstring(str.begin(), str.end());

	// Diff
	tMtrlInfo.tMtrl.vDiff = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sDiffuse
		, FbxSurfaceMaterial::sDiffuseFactor);

	// Amb
	tMtrlInfo.tMtrl.vAmb = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sAmbient
		, FbxSurfaceMaterial::sAmbientFactor);
	
	// Spec
	tMtrlInfo.tMtrl.vSpec = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sSpecular
		, FbxSurfaceMaterial::sSpecularFactor);
	
	// Emisv
	tMtrlInfo.tMtrl.vEmis = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sEmissive
		, FbxSurfaceMaterial::sEmissiveFactor);
	
	// Texture Name
	tMtrlInfo.strDiff = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);
	tMtrlInfo.strNormal = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
	tMtrlInfo.strSpec = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sSpecular);
		
	m_vecContainer.back().vecMtrl.push_back(tMtrlInfo);
}

void CFBXLoader::GetTangent(FbxMesh * _pMesh
	, tContainer * _pContainer
	, int _iIdx		 /*해당 정점의 인덱스*/
	, int _iVtxOrder /*폴리곤 단위로 접근하는 순서*/)
{
	int iTangentCnt = _pMesh->GetElementTangentCount();
	if (1 != iTangentCnt)
		assert(NULL); // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	// 탄젠트 data 의 시작 주소
	FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
	UINT iTangentIdx = 0;
	
	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			iTangentIdx = _iVtxOrder;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			iTangentIdx = _iIdx;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);
	
	_pContainer->vecTangent[_iIdx].x = (float)vTangent.mData[0];
	_pContainer->vecTangent[_iIdx].y = (float)vTangent.mData[2];
	_pContainer->vecTangent[_iIdx].z = (float)vTangent.mData[1];
}

void CFBXLoader::GetBinormal(FbxMesh * _pMesh, tContainer * _pContainer, int _iIdx, int _iVtxOrder)
{
	int iBinormalCnt = _pMesh->GetElementBinormalCount();
	if (1 != iBinormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
	UINT iBinormalIdx = 0;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iVtxOrder;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iIdx;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);

	_pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
	_pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
	_pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
}

void CFBXLoader::GetNormal(FbxMesh * _pMesh, tContainer * _pContainer, int _iIdx, int _iVtxOrder)
{
	int iNormalCnt = _pMesh->GetElementNormalCount();
	if (1 != iNormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

					  // 법선 data 의 시작 주소
	FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
	UINT iNormalIdx = 0;

	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);

	_pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
	_pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
	_pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
}

void CFBXLoader::GetUV(FbxMesh * _pMesh, tContainer * _pContainer, int _iIdx, int _iUVIndex)
{
	FbxGeometryElementUV* pUV = _pMesh->GetElementUV();

	UINT iUVIdx = 0;
	if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
		iUVIdx = _iIdx;
	else if (pUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
	{
		iUVIdx = _iUVIndex;
	}
	else
		iUVIdx = pUV->GetIndexArray().GetAt(_iIdx);

	

	FbxLayerElement::EReferenceMode mode = pUV->GetReferenceMode();


	FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);

	_pContainer->vecUV[_iIdx].x = (float)vUV.mData[0];
	_pContainer->vecUV[_iIdx].y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
}

Vec4 CFBXLoader::GetMtrlData(FbxSurfaceMaterial * _pSurface
	, const char * _pMtrlName
	, const char * _pMtrlFactorName)
{
	FbxDouble3  vMtrl;
	FbxDouble	dFactor = 0.;

	FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
	FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

	if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
	{
		vMtrl = tMtrlProperty.Get<FbxDouble3>();
		dFactor = tMtrlFactorProperty.Get<FbxDouble>();
	}

	Vec4 vRetVal = Vec4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
	return vRetVal;
}

wstring CFBXLoader::GetMtrlTextureName(FbxSurfaceMaterial * _pSurface, const char * _pMtrlProperty)
{
	string strName;

	FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
	if (TextureProperty.IsValid())
	{
		UINT iCnt = TextureProperty.GetSrcObjectCount();

		if (1 <= iCnt)
		{
			FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
			if (NULL != pFbxTex)
				strName = pFbxTex->GetFileName();
		}
	}

	return wstring(strName.begin(), strName.end());
}

void CFBXLoader::LoadTexture()
{
	path path_content = CPathMgr::GetResPath();

	path path_fbx_texture = path_content.wstring() + L"texture\\FBXTexture\\";
	if (false == exists(path_fbx_texture))
	{
		create_directory(path_fbx_texture);
	}	
	
	path path_origin;
	path path_filename;
	path path_dest;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{
			vector<path> vecPath;

			if (m_vecContainer[i].vecMtrl[j].strDiff != L"")
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strDiff.c_str());	

			//일단 노말과 디퓨즈맵은 사용 안함 
			//vecPath.push_back(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
			//vecPath.push_back(m_vecContainer[i].vecMtrl[j].strSpec.c_str());

			for (size_t k = 0; k < vecPath.size(); ++k)
			{
				path_origin = vecPath[k];
				path_filename = vecPath[k].filename();
				path_dest = path_fbx_texture.wstring() + path_filename.wstring();

				if (false == exists(path_dest))
				{
					copy(path_origin, path_dest);
				}

				path_dest = CPathMgr::GetRelativePath(path_dest);
				CResMgr::GetInst()->Load<CTexture>(path_dest, path_dest);

				switch (k)
				{
				case 0: m_vecContainer[i].vecMtrl[j].strDiff = path_dest; break;
				//case 1: m_vecContainer[i].vecMtrl[j].strNormal = path_dest; break;
				//case 2: m_vecContainer[i].vecMtrl[j].strSpec = path_dest; break;
				}
			}		
		}
		path_origin = path_origin.parent_path();
		//remove_all(path_origin);
	}
}

void CFBXLoader::CreateMaterial()
{
	wstring strKey;
	wstring strPath;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{			
			CMaterial* pMaterial = new CMaterial;

			// Material 이름짓기
			strKey = m_vecContainer[i].vecMtrl[j].strMtrlName;
			if (strKey.empty())
				strKey = path(m_vecContainer[i].vecMtrl[j].strDiff).stem();

			strPath = L"material\\";
			strPath += strKey;		

			// 상대경로가 곧 이름(확장자명은 제외)
			m_vecContainer[i].vecMtrl[j].strMtrlName = strKey;
			pMaterial->SetKey(strKey);
			pMaterial->SetRelativePath(strPath + L".mtrl");

			pMaterial->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));
						
			wstring strTexKey = m_vecContainer[i].vecMtrl[j].strDiff;
			Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if(NULL != pTex)
				pMaterial->SetData(SHADER_PARAM::TEX_0, pTex.Get());
			
			strTexKey = m_vecContainer[i].vecMtrl[j].strNormal;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetData(SHADER_PARAM::TEX_1, pTex.Get());
			
			strTexKey = m_vecContainer[i].vecMtrl[j].strSpec;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetData(SHADER_PARAM::TEX_2, pTex.Get());
			
			pMaterial->SetMaterialCoefficient(
				m_vecContainer[i].vecMtrl[j].tMtrl.vDiff
				, m_vecContainer[i].vecMtrl[j].tMtrl.vSpec
				, m_vecContainer[i].vecMtrl[j].tMtrl.vAmb
				, m_vecContainer[i].vecMtrl[j].tMtrl.vEmis);

			CResMgr::GetInst()->AddRes<CMaterial>(pMaterial->GetKey(), pMaterial);
		}
	}
}

void CFBXLoader::LoadSkeleton(FbxNode * _pNode)
{
	int iChildCount = _pNode->GetChildCount();

	LoadSkeleton_Re(_pNode, 0, 0, -1);
}

void CFBXLoader::LoadSkeleton_Re(FbxNode * _pNode, int _iDepth, int _iIdx, int _iParentIdx)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();
	
	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		tBone* pBone = new tBone;

		string strBoneName = _pNode->GetName();

		pBone->strBoneName = wstring(strBoneName.begin(), strBoneName.end());
		pBone->iDepth = _iDepth++;
		pBone->iParentIndx = _iParentIdx;

		m_vecBone.push_back(pBone);
	}
	
	int iChildCount = _pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		LoadSkeleton_Re(_pNode->GetChild(i), _iDepth, (int)m_vecBone.size(), _iIdx);
	}
}

void CFBXLoader::LoadAnimationClip()
{
	int iAnimCount = m_arrAnimName.GetCount();

	for (int i = 0; i < iAnimCount; ++i)
	{
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());

		if (!pAnimStack)
			continue;

		tAnimClip* pAnimClip = new tAnimClip;

		string strClipName = pAnimStack->GetName();
		pAnimClip->strName = wstring(strClipName.begin(), strClipName.end());

		FbxTakeInfo* pTakeInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());
		pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

		pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) - pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);

		m_vecAnimClip.push_back(pAnimClip);
	}
}

void CFBXLoader::Triangulate(FbxNode * _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr &&
		(pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(m_pManager);
		converter.Triangulate(pAttr, true);
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(_pNode->GetChild(i));
	}
}

void CFBXLoader::LoadAnimationData(FbxMesh * _pMesh, tContainer * _pContainer)
{
	//이 함수는 메쉬당 1번 호출된다 

	// Animation Data 로드할 필요가 없음
	int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iSkinCount <= 0 || m_vecAnimClip.empty())
		return;

	_pContainer->bAnimation = true;


	FbxNode* RootNode = m_pScene->GetRootNode();

	
	// Skin 개수만큼 반복을하며 읽는다.	
	// SkinCount 는 메쉬당 보통 1개 . 따라서 1번만 실행된다 
	for (int i = 0; i < iSkinCount; ++i)
	{
		//각각의 Deformer안에(보통 매쉬 하나당 deformer 하나임) "Cluster"라는 것들을 갖고있음
		//이 cluster가 joint를 의미하는 것으로 생각할 수 있지만 그것도 아님
		//각각의 cluster안에 "link"라는게 또 들어있는데 이게 진짜 joint이고 여기서 내가 필요한 유용한 정보들을 얻을 수 있다

		//현재 메쉬에 해당하는 스킨 
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (pSkin)
		{
			FbxSkin::EType eType = pSkin->GetSkinningType();
			if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int iClusterCount = pSkin->GetClusterCount();
				
				//관절의 갯수만큼 반복한다 
				for (int j = 0; j < iClusterCount; ++j)
				{
					FbxCluster* pCluster = pSkin->GetCluster(j);

					//관절을 이어주는 Link가 존재하지 않으면 continue
					if (!pCluster->GetLink())
						continue;

					// 현재 본 인덱스를 Link의 이름으로 얻어온다.
					int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());
					if (-1 == iBoneIdx)
						assert(NULL);
					
					//(아마)루트 노드의 행렬을 받아온다
					//대개는 이 값은 단위행렬이다 
					FbxAMatrix matNodeTransform = GetTransform(_pMesh->GetNode());

					// Weights And Indices 정보를 읽는다.
					// 메쉬의 클러스터 , 현재 본 인덱스 ,  콘테이너 전달 
					// 현재 본에 대한 가중치를 계산한다 
					// 가중치 = 정점이 bone에게서 받는 영향 
					// 이 함수는 container의 vecWI(정점갯수만큼의 크기를 가진다) 에 
					// 영향을 받는 본 인덱스 , 가중치값을 넣어준다 
					LoadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);

					// Bone 의 OffSet 행렬 구한다.
					//GetTransformLinkMatrix로 해당 본의 월드 Matrix를 얻고 GetTransformMatrix로는 
					//부모까지 오게 된 Matrix를 얻는다.그래서 해당 부모의 행렬의 역행렬을 곱하면 
					//부모행렬기준 자기 자신의 정보에 대한 행렬값만 남게 되서 계층구조적 행렬값을 갖을 수 있게 된다.
					LoadOffsetMatrix(pCluster, matNodeTransform, iBoneIdx, _pContainer);

					// Bone KeyFrame 별 행렬을 구한다.
					LoadKeyframeTransform(_pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx, _pContainer);
				}
			}
		}
	}
	CheckWeightAndIndices(_pMesh, _pContainer);
}


void CFBXLoader::CheckWeightAndIndices(FbxMesh* _pMesh, tContainer * _pContainer)
{
	vector<vector<tWeightsAndIndices>>::iterator iter = _pContainer->vecWI.begin();

	int iVtxIdx = 0;
	for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
	{
		if ((*iter).size() > 1)
		{
			// 가중치 값 순으로 내림차순 정렬
			sort((*iter).begin(), (*iter).end()
				, [](const tWeightsAndIndices& left, const tWeightsAndIndices& right)
			{
				return left.dWeight > right.dWeight;
			}
			);

			double dWeight = 0.f;
			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				dWeight += (*iter)[i].dWeight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*iter)[0].dWeight += revision;
			}

			if ((*iter).size() >= 4)
			{
				(*iter).erase((*iter).begin() + 4, (*iter).end());
			}
		}

		// 정점 정보로 변환, 
		float fWeights[4] = {};
		float fIndices[4] = {};

		for (UINT i = 0; i < (*iter).size(); ++i)
		{
			fWeights[i] = (float)(*iter)[i].dWeight;
			fIndices[i] = (float)(*iter)[i].iBoneIdx;
		}

		memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(Vec4));
		memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(Vec4));
	}
}

Vec3 CFBXLoader::GetRotation(FbxVector4 _vec1, FbxVector4 _vec2, FbxVector4 _vec3)
{
	float Yaw = 0;
	float Pitch = 0;
	float Roll = 0;

	if (_vec1[0] == 1.0f)
	{
		Yaw = atan2f(_vec1[2], _vec3[3]);
		Pitch = 0;
		Roll = 0;

	}
	else if (_vec1[0] == -1.0f)
	{
		Yaw = atan2f(_vec1[2], _vec3[3]);
		Pitch = 0;
		Roll = 0;
	}
	else
	{

		Yaw = atan2(-_vec3[0], _vec1[0]);
		Pitch = asin(_vec2[0]);
		Roll = atan2(_vec2[2], _vec2[1]);
	}

	return Vec3(Yaw, Pitch, Roll);
}

void CFBXLoader::LoadKeyframeTransform(FbxNode * _pNode, FbxCluster * _pCluster
	, const FbxAMatrix & _matNodeTransform, int _iBoneIdx, tContainer * _pContainer)
{
	//_pNode = 최상위 부모 노드 
	//_pCluster = 현재 스킨에 대한 클러스터 
	//_matNodeTransform = 최상위 부모 노드의 행렬
	//_iBoneIdx = 현재 본 인덱스 
	//_pContainer  = 컨테이너 


	if (m_vecAnimClip.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	//m_vecBone = 뼈대의 배열이 담긴다 
	//현재는 단위행렬이 들어온다 
	m_vecBone[_iBoneIdx]->matBone = _matNodeTransform;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	//애니메이션의 시간, 프레임별 정보가 벡터에 들어있다 

	//일단 여기에서 반복문을 돌려줘야 할 것 같음 
	FbxLongLong llStartFrame = m_vecAnimClip[0]->tStartTime.GetFrameCount(eTimeMode);
	FbxLongLong llEndFrame = m_vecAnimClip[0]->tEndTime.GetFrameCount(eTimeMode);

	for (FbxLongLong i = llStartFrame; i < llEndFrame; ++i)
	{
		//키프레임의 매트릭스 정보와 시간 
		tKeyFrame tFrame = {};
		FbxTime   tTime = 0;

		tTime.SetFrame(i, eTimeMode);

		//특정 시간에서의 매트릭스 정보 x (아마) 루트 노드의 매트릭스 정보 
		FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
		FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
		matCurTrans = matReflect * matCurTrans * matReflect;

		tFrame.dTime = tTime.GetSecondDouble();
		tFrame.matTransform = matCurTrans;

		//특정시간과 행렬정보를 포함하는 구조체를 가진다 
		m_vecBone[_iBoneIdx]->vecKeyFrame.push_back(tFrame);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster * _pCluster
	, const FbxAMatrix & _matNodeTransform
	, int _iBoneIdx, tContainer* _pContainer)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;

	_pCluster->GetTransformMatrix(matClusterTrans);
	_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans *_matNodeTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[_iBoneIdx]->matOffset = matOffset;
}


void CFBXLoader::LoadWeightsAndIndices(FbxCluster * _pCluster
	, int _iBoneIdx
	, tContainer * _pContainer)
{
	//현재 스킨의 클러스터에서 정점 갯수를 가져온다 
	int iIndicesCount = _pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iIndicesCount; ++i)
	{
		tWeightsAndIndices tWI = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		tWI.iBoneIdx = _iBoneIdx;
		tWI.dWeight = _pCluster->GetControlPointWeights()[i];

		int iVtxIdx = _pCluster->GetControlPointIndices()[i];

		
		_pContainer->vecWI[iVtxIdx].push_back(tWI);
	}
}



int CFBXLoader::FindBoneIndex(string _strBoneName)
{
	wstring strBoneName = wstring(_strBoneName.begin(), _strBoneName.end());

	for (UINT i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->strBoneName == strBoneName)
			return i;
	}
	
	return -1;
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode * _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

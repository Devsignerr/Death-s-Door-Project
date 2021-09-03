#include "pch.h"
#include "CActorScript.h"
#include "CAttackImpactScript.h"
#include "CRandomMgrScript.h"
#include "CMemoryPoolScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>


void CActorScript::CreateCollider(UINT _LayerIdx, Vec3 _Scale, Vec3 OffsetPos)
{
	CGameObject* Obj = new CGameObject;

	Obj->SetName(L"PlayerCol");
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalScale(_Scale);
	Obj->Collider3D()->SetOffSetPos(OffsetPos);

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, _LayerIdx);

	AddChild(GetObj(), Obj);

}

CGameObject* CActorScript::IstanciatePrefab(wstring _wstr, UINT _LayerIdx)
{
	Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(_wstr);
	if (nullptr == Prefab)
	{
		wstring PrefabPath = L"prefab\\" + _wstr + L".pref";
		Ptr<CPrefab> Prefab = CResMgr::GetInst()->Load<CPrefab>(_wstr, PrefabPath);
	}

	CGameObject* pGameObject = Prefab->Instantiate();

	int PrefabCount = 0;
	wstring PrefabNumber = L"0";

	while (nullptr != CSceneMgr::GetInst()->FindObjectByName(_wstr + PrefabNumber))
	{
		PrefabCount++;
		wchar_t Str[10] = {};
		_itow_s(PrefabCount, Str, 10);
		PrefabNumber = wstring(Str);
	}

	pGameObject->SetName(_wstr + PrefabNumber);
	pGameObject->awake();
	pGameObject->start();
	
	CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, _LayerIdx);

	return pGameObject;
}


//이 함수는 우선 이전 프레임에 위치했던 폴리곤(네비메쉬노드) 을 대상으로 레이충돌체크를 진행하고 , 
//만약 실패했을경우 현재 폴리곤의 인접 노드에 접근해 다시 한번 충돌체크를 진행한다 . 

//만약 그 판정에서도 실패할 경우 현재 네비메쉬의 모든 폴리곤에 대해 충돌체크를 진행하고 , 
//거기에서도 실패하면 씬에 존재하는 모든 네비메쉬에 대해 다시 한번 검사를 진행한다 . 

//위 검사에서도 실패하면 맵에서 벗어난것이다 .


bool CActorScript::GroundCheck()
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();
	
	Vec3 ObjectPos = Transform()->GetLocalPos();
	ObjectPos.y += m_fOffSetPos;	
	
	//ray는 항상 아래방향을 향한다 
	DirectX::XMVECTOR LayDir = Vec3(0.f, -1.f, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//리소스 매니저에게서 현재 맵에 존재하는 모든 네비메쉬 벡터를 얻어온다 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();
	
	if (VecNavMesh.size() == 0)
		return false;

	//이전 프레임에 내가 위치했던 메쉬의 인덱스로 탐색할것이다. 
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	//이전 프레임에 내가 위치했던 메쉬의 모든 노드벡터 
	vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	//현재 네비메쉬의 월드매트릭스 
	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};	

	//이전 프레임에 위치했던 폴리곤 위에서 충돌검사를 먼저 1번 진행한다 
	MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[0], MapMat);
	MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[1], MapMat);
	MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[2], MapMat);

	//만약 충돌한다면 그대로 잘 있는것이므로 트루를 리턴한다 . 
	if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		NodeCheck = true;

		return NodeCheck;
	}

	//충돌에 실패했다면 인접 노드들을 대상으로 다시 한번 충돌체크를 진행한다 . 
	else 
	{
		vector<int> NearNodeVec = vecNavMesh[m_iCurNavNodeIdx].VecNearNodeIdx;
		UINT NearNodeCnt = NearNodeVec.size();

		for (UINT NCnt = 0; NCnt < NearNodeCnt; ++NCnt)
		{
			UINT NearNodeIdx = NearNodeVec[NCnt];
				
			MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[0], MapMat);
			MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[1], MapMat);
			MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[2], MapMat);

			//만약 성공한다면 내 인접노드중 하나로 건너간 것이다 . 
			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//현재 위치하고 있는 노드의 인덱스로 갱신해준다 . 
				m_iCurNavNodeIdx = NearNodeIdx;

				return NodeCheck;
			}

		}
	}
	
	//이 값이 반환될 경우 , 이전 프레임에 위치했던 네비메쉬에서 벗어났다는 뜻이다. 
	//따라서 ResearchNode() 함수를 호출해 씬에 존재하는 모든 메쉬에 대해 재검색을 실시한다 . 
	return false;
}

bool CActorScript::ResearchNode()
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = Transform()->GetLocalPos();
	ObjectPos.y += m_fOffSetPos;

	//ray는 항상 아래방향을 향한다 
	DirectX::XMVECTOR LayDir = Vec3(0.f, -1.f, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//리소스 매니저에게서 현재 맵에 존재하는 모든 네비메쉬 벡터를 얻어온다 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//일단 내가 위치했던 네비메쉬인덱스로 접근해 검사를 진행해본다 .
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	const vector<tNavMeshNode>& vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};

	for (int idx = 0; idx < vecNavMesh.size(); ++idx)
	{
		MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[0], MapMat);
		MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[1], MapMat);
		MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[2], MapMat);

		if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
		{
			NodeCheck = true;

			//만약 성공한다면 현재 메쉬에서 위치한 노드인덱스를 갱신해준다 . 메쉬인덱스는 그대로다 . 
			m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

			return NodeCheck;
		}
	}

	//이 함수가 여기까지 진행되었다는 것은 아예 다른 네비메쉬로 넘어갔거나 , 맵을 벗어났다는 뜻이다 . 
	//다른 네비메쉬로 넘어갔는지에 대해 검사를 진행한다 . 

	UINT NavMeshCount = VecNavMesh.size();

	for (UINT i = 0; i < NavMeshCount; ++i)
	{
		CGameObject* pNavMesh = VecNavMesh[i];

		vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

		const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

		// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
		DirectX::XMVECTOR MapPoint1 = {};
		DirectX::XMVECTOR MapPoint2 = {};
		DirectX::XMVECTOR MapPoint3 = {};

		for (int idx = 0; idx < vecNavMesh.size(); ++idx)
		{
			MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[0], MapMat);
			MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[1], MapMat);
			MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[2], MapMat);

			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//현재 내가 올라와있는 네비메쉬 인덱스 . 
				//만약 이전 폴리곤에서 충돌이 실패하면 아래 Idx에 있는 폴리곤의 nearIdx 폴리곤을 탐색해 충돌여부를 검사한다 
				m_iCurNavMeshIdx = i;

				//씬에 존재하는 모든 네비메쉬들을 담은 벡터에서 몇번째 인덱스에 위치하고 있는지 
				m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

				return NodeCheck;
			}
		}
	}

	return false;
}

bool CActorScript::GroundCheck(Vec3 _MovePos, int _RayDir)
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = _MovePos;
	ObjectPos.y += m_fOffSetPos;

	//ray는 항상 아래방향을 향한다 
	DirectX::XMVECTOR LayDir = Vec3(0.f, _RayDir, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//리소스 매니저에게서 현재 맵에 존재하는 모든 네비메쉬 벡터를 얻어온다 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//이전 프레임에 내가 위치했던 메쉬의 인덱스로 탐색할것이다. 
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	//이전 프레임에 내가 위치했던 메쉬의 모든 노드벡터 
	const vector<tNavMeshNode>& vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	//현재 네비메쉬의 월드매트릭스 
	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};

	//이전 프레임에 위치했던 폴리곤 위에서 충돌검사를 먼저 1번 진행한다 
	MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[0], MapMat);
	MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[1], MapMat);
	MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[2], MapMat);

	//만약 충돌한다면 그대로 잘 있는것이므로 트루를 리턴한다 . 
	if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		NodeCheck = true;

		return NodeCheck;
	}

	//충돌에 실패했다면 인접 노드들을 대상으로 다시 한번 충돌체크를 진행한다 . 
	else
	{
		vector<int> NearNodeVec = vecNavMesh[m_iCurNavNodeIdx].VecNearNodeIdx;
		UINT NearNodeCnt = NearNodeVec.size();

		for (UINT NCnt = 0; NCnt < NearNodeCnt; ++NCnt)
		{
			UINT NearNodeIdx = NearNodeVec[NCnt];

			MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[0], MapMat);
			MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[1], MapMat);
			MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[NearNodeIdx].VertexPosition[2], MapMat);

			//만약 성공한다면 내 인접노드중 하나로 건너간 것이다 . 
			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//현재 위치하고 있는 노드의 인덱스로 갱신해준다 . 
				m_iCurNavNodeIdx = NearNodeIdx;

				return NodeCheck;
			}

		}
	}

	//이 값이 반환될 경우 , 이전 프레임에 위치했던 네비메쉬에서 벗어났다는 뜻이다. 
	//따라서 ResearchNode() 함수를 호출해 씬에 존재하는 모든 메쉬에 대해 재검색을 실시한다 . 
	return false;
}

bool CActorScript::ResearchNode(Vec3 _MovePos, int _RayDir)
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = _MovePos;
	ObjectPos.y += m_fOffSetPos;

	//ray는 항상 아래방향을 향한다 
	DirectX::XMVECTOR LayDir = Vec3(0.f, _RayDir, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//리소스 매니저에게서 현재 맵에 존재하는 모든 네비메쉬 벡터를 얻어온다 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//일단 내가 위치했던 네비메쉬인덱스로 접근해 검사를 진행해본다 .
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	vector<tNavMeshNode>& vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};

	for (int idx = 0; idx < vecNavMesh.size(); ++idx)
	{
		MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[0], MapMat);
		MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[1], MapMat);
		MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[2], MapMat);

		if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
		{
			NodeCheck = true;

			//만약 성공한다면 현재 메쉬에서 위치한 노드인덱스를 갱신해준다 . 메쉬인덱스는 그대로다 . 
			m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

			return NodeCheck;
		}
	}

	//이 함수가 여기까지 진행되었다는 것은 아예 다른 네비메쉬로 넘어갔거나 , 맵을 벗어났다는 뜻이다 . 
	//다른 네비메쉬로 넘어갔는지에 대해 검사를 진행한다 . 

	UINT NavMeshCount = VecNavMesh.size();

	for (UINT i = 0; i < NavMeshCount; ++i)
	{
		CGameObject* pNavMesh = VecNavMesh[i];

		vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

		const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

		// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
		DirectX::XMVECTOR MapPoint1 = {};
		DirectX::XMVECTOR MapPoint2 = {};
		DirectX::XMVECTOR MapPoint3 = {};

		for (int idx = 0; idx < vecNavMesh.size(); ++idx)
		{
			MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[0], MapMat);
			MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[1], MapMat);
			MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[2], MapMat);

			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//현재 내가 올라와있는 네비메쉬 인덱스 . 
				//만약 이전 폴리곤에서 충돌이 실패하면 아래 Idx에 있는 폴리곤의 nearIdx 폴리곤을 탐색해 충돌여부를 검사한다 
				m_iCurNavMeshIdx = i;

				//씬에 존재하는 모든 네비메쉬들을 담은 벡터에서 몇번째 인덱스에 위치하고 있는지 
				m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

				return NodeCheck;
			}
		}
	}

	return false;
}

void CActorScript::update()
{
	 bool IsGround = GroundCheck();

	 //현재 폴리곤 혹은 인접 폴리곤과의 충돌체크에 실패하면 현재 네비메쉬의 모든 폴리곤에 대해 충돌체크하고 
	 //위 체크에 대해서도 실패하면 씬의 모든 네비메쉬와 충돌체크를 진행한다 . 
	 
	 //만약 개선한다면 , GUI로 네비메쉬 자체의 Idx와 Near네비메쉬 Idx를 넣도록 해줄수도 있을것이다 .  
	 if (!IsGround)
		 IsGround = ResearchNode();

	 if (false == m_bJump)
	 {
		 //만약 충돌했는데 거리가 100보다 가까우면 (경사를 올라갔다면 등)
		 if (IsGround && m_Dist < 100.f)
		 {
			 float YMove = 100.f - m_Dist;
			 Vec3 Pos = Transform()->GetLocalPos();
			 Pos.y += YMove;
			 Transform()->SetLocalPos(Pos);
		 }

		 //거리가 100보다 크면 

		 if (m_Dist > 100.f)
		 {
			 float YMove = m_Dist - 100.f;
			 Vec3 Pos = Transform()->GetLocalPos();
			 Pos.y -= YMove;
			 Transform()->SetLocalPos(Pos);
		 }
	 }

}

void CActorScript::awake()
{
	m_PaperBurnTex = CResMgr::GetInst()->FindRes<CTexture>(L"PaperBurnTexture");

	if (nullptr == m_PaperBurnTex)
		m_PaperBurnTex = CResMgr::GetInst()->Load<CTexture>(L"PaperBurnTexture", L"texture\\PaperBurn\\PaperBurnTexture.jpg");

}

void CActorScript::OnCollisionEnter(CGameObject* _pOther)
{
}


void CActorScript::ActivateImpactParticle(Vec3 _Pos ,Vec3 _Dir, int ActivateCount, float SpreadRange)
{
	for (int i = 0; i < ActivateCount; ++i)
	{
		CGameObject* pAttackImpact = CMemoryPoolScript::GetAttackImpact();

		if (nullptr == pAttackImpact)
			return;

		Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 vRight = vUp.Cross(_Dir);


		float iRand = CRandomMgrScript::GetRandomintNumber(-SpreadRange, SpreadRange);
		iRand /= 10.f;

		XMMATRIX matR = XMMatrixRotationAxis(vRight, iRand);

		iRand = CRandomMgrScript::GetRandomintNumber(-SpreadRange, SpreadRange);
		iRand /= 10.f;

		XMMATRIX matU = XMMatrixRotationAxis(vUp, iRand);

		Vec3 Dir = _Dir;

		Dir = XMVector3TransformCoord(Dir, matU);
		Dir = XMVector3TransformCoord(Dir, matR);


		((CAttackImpactScript*)pAttackImpact->GetScript())->SetDir(Dir);


		iRand = CRandomMgrScript::GetRandomintNumber(50, 200);

		((CAttackImpactScript*)pAttackImpact->GetScript())->SetScale(iRand);

		iRand = CRandomMgrScript::GetRandomintNumber(4000, 8000);

		((CAttackImpactScript*)pAttackImpact->GetScript())->SetSpeed(iRand);

		pAttackImpact->Transform()->SetLocalPos(_Pos);


		Vec3 Rot = pAttackImpact->Transform()->GetLocalRot();
		Vec3 vFront = pAttackImpact->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Vec3 vCross = Dir.Cross(vFront);
		Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

		float dot = vCross.Dot(vUP);
		Dir.Normalize();

		float RotAngle = vFront.Dot(Dir);
		RotAngle = acos(RotAngle);

		if (dot > 0.0)
			Rot.y -= RotAngle;

		else if (dot < 0.0)
			Rot.y += RotAngle;

		pAttackImpact->Transform()->SetLocalRot(Rot);
	}
}

CActorScript::CActorScript()
	: CScript((UINT)SCRIPT_TYPE::ACTORSCRIPT)
	, m_fOffSetPos(10.f)
	, m_bJump(false)
	, RayDir(-1)
{

}

CActorScript::~CActorScript()
{
}

void CActorScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CActorScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

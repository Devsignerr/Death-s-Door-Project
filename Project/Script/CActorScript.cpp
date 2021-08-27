#include "pch.h"
#include "CActorScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>


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


//�� �Լ��� �켱 ���� �����ӿ� ��ġ�ߴ� ������(�׺�޽����) �� ������� �����浹üũ�� �����ϰ� , 
//���� ����������� ���� �������� ���� ��忡 ������ �ٽ� �ѹ� �浹üũ�� �����Ѵ� . 

//���� �� ���������� ������ ��� ���� �׺�޽��� ��� �����￡ ���� �浹üũ�� �����ϰ� , 
//�ű⿡���� �����ϸ� ���� �����ϴ� ��� �׺�޽��� ���� �ٽ� �ѹ� �˻縦 �����Ѵ� . 

//�� �˻翡���� �����ϸ� �ʿ��� ������̴� .


bool CActorScript::GroundCheck()
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();
	
	Vec3 ObjectPos = Transform()->GetLocalPos();
	ObjectPos.y += m_fOffSetPos;	
	
	//ray�� �׻� �Ʒ������� ���Ѵ� 
	DirectX::XMVECTOR LayDir = Vec3(0.f, -1.f, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//���ҽ� �Ŵ������Լ� ���� �ʿ� �����ϴ� ��� �׺�޽� ���͸� ���´� 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();
	
	if (VecNavMesh.size() == 0)
		return false;

	//���� �����ӿ� ���� ��ġ�ߴ� �޽��� �ε����� Ž���Ұ��̴�. 
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	//���� �����ӿ� ���� ��ġ�ߴ� �޽��� ��� ��庤�� 
	vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	//���� �׺�޽��� �����Ʈ���� 
	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	// XMVECTOR�� XMVector3TransformCoord �Ҷ� y�� z�� ���� �ٲ㼭 �־�� ����ε� ���� ���´�
	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};	

	//���� �����ӿ� ��ġ�ߴ� ������ ������ �浹�˻縦 ���� 1�� �����Ѵ� 
	MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[0], MapMat);
	MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[1], MapMat);
	MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[2], MapMat);

	//���� �浹�Ѵٸ� �״�� �� �ִ°��̹Ƿ� Ʈ�縦 �����Ѵ� . 
	if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		NodeCheck = true;

		return NodeCheck;
	}

	//�浹�� �����ߴٸ� ���� ������ ������� �ٽ� �ѹ� �浹üũ�� �����Ѵ� . 
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

			//���� �����Ѵٸ� �� ��������� �ϳ��� �ǳʰ� ���̴� . 
			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//���� ��ġ�ϰ� �ִ� ����� �ε����� �������ش� . 
				m_iCurNavNodeIdx = NearNodeIdx;

				return NodeCheck;
			}

		}
	}
	
	//�� ���� ��ȯ�� ��� , ���� �����ӿ� ��ġ�ߴ� �׺�޽����� ����ٴ� ���̴�. 
	//���� ResearchNode() �Լ��� ȣ���� ���� �����ϴ� ��� �޽��� ���� ��˻��� �ǽ��Ѵ� . 
	return false;
}

bool CActorScript::ResearchNode()
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = Transform()->GetLocalPos();
	ObjectPos.y += m_fOffSetPos;

	//ray�� �׻� �Ʒ������� ���Ѵ� 
	DirectX::XMVECTOR LayDir = Vec3(0.f, -1.f, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//���ҽ� �Ŵ������Լ� ���� �ʿ� �����ϴ� ��� �׺�޽� ���͸� ���´� 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//�ϴ� ���� ��ġ�ߴ� �׺�޽��ε����� ������ �˻縦 �����غ��� .
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

			//���� �����Ѵٸ� ���� �޽����� ��ġ�� ����ε����� �������ش� . �޽��ε����� �״�δ� . 
			m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

			return NodeCheck;
		}
	}

	//�� �Լ��� ������� ����Ǿ��ٴ� ���� �ƿ� �ٸ� �׺�޽��� �Ѿ�ų� , ���� ����ٴ� ���̴� . 
	//�ٸ� �׺�޽��� �Ѿ������ ���� �˻縦 �����Ѵ� . 

	UINT NavMeshCount = VecNavMesh.size();

	for (UINT i = 0; i < NavMeshCount; ++i)
	{
		CGameObject* pNavMesh = VecNavMesh[i];

		vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

		const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

		// XMVECTOR�� XMVector3TransformCoord �Ҷ� y�� z�� ���� �ٲ㼭 �־�� ����ε� ���� ���´�
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

				//���� ���� �ö���ִ� �׺�޽� �ε��� . 
				//���� ���� �����￡�� �浹�� �����ϸ� �Ʒ� Idx�� �ִ� �������� nearIdx �������� Ž���� �浹���θ� �˻��Ѵ� 
				m_iCurNavMeshIdx = i;

				//���� �����ϴ� ��� �׺�޽����� ���� ���Ϳ��� ���° �ε����� ��ġ�ϰ� �ִ��� 
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

	//ray�� �׻� �Ʒ������� ���Ѵ� 
	DirectX::XMVECTOR LayDir = Vec3(0.f, _RayDir, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//���ҽ� �Ŵ������Լ� ���� �ʿ� �����ϴ� ��� �׺�޽� ���͸� ���´� 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//���� �����ӿ� ���� ��ġ�ߴ� �޽��� �ε����� Ž���Ұ��̴�. 
	CGameObject* pNavMesh = VecNavMesh[m_iCurNavMeshIdx];

	//���� �����ӿ� ���� ��ġ�ߴ� �޽��� ��� ��庤�� 
	const vector<tNavMeshNode>& vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

	//���� �׺�޽��� �����Ʈ���� 
	const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

	// XMVECTOR�� XMVector3TransformCoord �Ҷ� y�� z�� ���� �ٲ㼭 �־�� ����ε� ���� ���´�
	DirectX::XMVECTOR MapPoint1 = {};
	DirectX::XMVECTOR MapPoint2 = {};
	DirectX::XMVECTOR MapPoint3 = {};

	//���� �����ӿ� ��ġ�ߴ� ������ ������ �浹�˻縦 ���� 1�� �����Ѵ� 
	MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[0], MapMat);
	MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[1], MapMat);
	MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[m_iCurNavNodeIdx].VertexPosition[2], MapMat);

	//���� �浹�Ѵٸ� �״�� �� �ִ°��̹Ƿ� Ʈ�縦 �����Ѵ� . 
	if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		NodeCheck = true;

		return NodeCheck;
	}

	//�浹�� �����ߴٸ� ���� ������ ������� �ٽ� �ѹ� �浹üũ�� �����Ѵ� . 
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

			//���� �����Ѵٸ� �� ��������� �ϳ��� �ǳʰ� ���̴� . 
			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				//���� ��ġ�ϰ� �ִ� ����� �ε����� �������ش� . 
				m_iCurNavNodeIdx = NearNodeIdx;

				return NodeCheck;
			}

		}
	}

	//�� ���� ��ȯ�� ��� , ���� �����ӿ� ��ġ�ߴ� �׺�޽����� ����ٴ� ���̴�. 
	//���� ResearchNode() �Լ��� ȣ���� ���� �����ϴ� ��� �޽��� ���� ��˻��� �ǽ��Ѵ� . 
	return false;
}

bool CActorScript::ResearchNode(Vec3 _MovePos, int _RayDir)
{
	m_Dist = 0.f;
	bool NodeCheck = false;

	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = _MovePos;
	ObjectPos.y += m_fOffSetPos;

	//ray�� �׻� �Ʒ������� ���Ѵ� 
	DirectX::XMVECTOR LayDir = Vec3(0.f, _RayDir, 0.f);
	DirectX::XMVECTOR ObjCenter = ObjectPos;

	//���ҽ� �Ŵ������Լ� ���� �ʿ� �����ϴ� ��� �׺�޽� ���͸� ���´� 
	const vector<CGameObject*>& VecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

	if (VecNavMesh.size() == 0)
		return false;

	//�ϴ� ���� ��ġ�ߴ� �׺�޽��ε����� ������ �˻縦 �����غ��� .
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

			//���� �����Ѵٸ� ���� �޽����� ��ġ�� ����ε����� �������ش� . �޽��ε����� �״�δ� . 
			m_iCurNavNodeIdx = vecNavMesh[idx].NodeIdx;

			return NodeCheck;
		}
	}

	//�� �Լ��� ������� ����Ǿ��ٴ� ���� �ƿ� �ٸ� �׺�޽��� �Ѿ�ų� , ���� ����ٴ� ���̴� . 
	//�ٸ� �׺�޽��� �Ѿ������ ���� �˻縦 �����Ѵ� . 

	UINT NavMeshCount = VecNavMesh.size();

	for (UINT i = 0; i < NavMeshCount; ++i)
	{
		CGameObject* pNavMesh = VecNavMesh[i];

		vector<tNavMeshNode> vecNavMesh = pNavMesh->MeshRender()->GetMesh()->GetMeshNodeVec();

		const Matrix& MapMat = pNavMesh->Transform()->GetWorldMat();

		// XMVECTOR�� XMVector3TransformCoord �Ҷ� y�� z�� ���� �ٲ㼭 �־�� ����ε� ���� ���´�
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

				//���� ���� �ö���ִ� �׺�޽� �ε��� . 
				//���� ���� �����￡�� �浹�� �����ϸ� �Ʒ� Idx�� �ִ� �������� nearIdx �������� Ž���� �浹���θ� �˻��Ѵ� 
				m_iCurNavMeshIdx = i;

				//���� �����ϴ� ��� �׺�޽����� ���� ���Ϳ��� ���° �ε����� ��ġ�ϰ� �ִ��� 
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

	 //���� ������ Ȥ�� ���� ��������� �浹üũ�� �����ϸ� ���� �׺�޽��� ��� �����￡ ���� �浹üũ�ϰ� 
	 //�� üũ�� ���ؼ��� �����ϸ� ���� ��� �׺�޽��� �浹üũ�� �����Ѵ� . 
	 
	 //���� �����Ѵٸ� , GUI�� �׺�޽� ��ü�� Idx�� Near�׺�޽� Idx�� �ֵ��� ���ټ��� �������̴� .  
	 if (!IsGround)
		 IsGround = ResearchNode();

	 if (false == m_bJump)
	 {
		 //���� �浹�ߴµ� �Ÿ��� 100���� ������ (��縦 �ö󰬴ٸ� ��)
		 if (IsGround && m_Dist < 100.f)
		 {
			 float YMove = 100.f - m_Dist;
			 Vec3 Pos = Transform()->GetLocalPos();
			 Pos.y += YMove;
			 Transform()->SetLocalPos(Pos);
		 }

		 //�Ÿ��� 100���� ũ�� 

		 if (m_Dist > 100.f)
		 {
			 float YMove = m_Dist - 100.f;
			 Vec3 Pos = Transform()->GetLocalPos();
			 Pos.y -= YMove;
			 Transform()->SetLocalPos(Pos);
		 }
	 }

}

void CActorScript::OnCollisionEnter(CGameObject* _pOther)
{
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

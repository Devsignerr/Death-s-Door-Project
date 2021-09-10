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


void CActorScript::GetDamage()
{
	if (false == m_bDamaged)
	{
		int vecOriginalMtrlSize = m_vecOriginMtrl.size();

		const vector<CGameObject*>& ChildVec = GetObj()->GetChild();

		for (int i = 0; i < vecOriginalMtrlSize; ++i)
		{
			if (ChildVec[i]->MeshRender() && nullptr == ChildVec[i]->Collider3D())
			{
				int Count = ChildVec[i]->MeshRender()->GetMtrlCount();

				for (UINT j = 0; j < Count; ++j)
				{
					Ptr<CMaterial> CloneMtrl = ChildVec[i]->MeshRender()->GetCloneMaterial(j);
					ChildVec[i]->MeshRender()->SetMaterial(CloneMtrl, j);
				}

			}
		}
	}

	m_bDamaged = true;
	m_fCurDamageTime = 0.f;
}

void CActorScript::DamageEffectupdate()
{
	if (m_bDamaged)
	{
		m_fCurDamageTime += fDT;

		if (m_fCurDamageTime > m_fDamageEffectTime)
		{
			m_bDamaged = false;

			int vecOriginalMtrlSize = m_vecOriginMtrl.size();

			const vector<CGameObject*>& ChildVec = GetObj()->GetChild();


			for (int i = 0; i < vecOriginalMtrlSize; ++i)
			{
				if (ChildVec[i]->MeshRender() && nullptr == ChildVec[i]->Collider3D())
				{
					ChildVec[i]->MeshRender()->SetMaterial(m_vecOriginMtrl[i], 0);
				}
			}

			return;
		}

		//White
		if (m_fCurDamageTime < 0.1f * m_fDamageEffectTime)
		{
			EffectParamSetting(Vec4(100.f, 100.f, 100.f, 1.f), Vec4(10.f, 10.f, 10.f, 1.f), m_WhiteTex);
		}
		//Red
		else if (m_fCurDamageTime >= 0.1f * m_fDamageEffectTime && m_fCurDamageTime < 0.3f * m_fDamageEffectTime)
		{
			EffectParamSetting(Vec4(10.f, 1.f, 1.f, 1.f), Vec4(0.1f, 0.f, 0.f, 1.f), m_RedTex);
		}

		//Black
		else if (m_fCurDamageTime >= 0.3f * m_fDamageEffectTime && m_fCurDamageTime < 0.6f * m_fDamageEffectTime)
		{
			EffectParamSetting(Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), m_WhiteTex);
		}

		//Red
		else if (m_fCurDamageTime >= 0.6f * m_fDamageEffectTime && m_fCurDamageTime < 0.8f * m_fDamageEffectTime)
		{
			EffectParamSetting(Vec4(10.f, 1.f, 1.f, 1.f), Vec4(0.1f, 0.f, 0.f, 1.f), m_RedTex);
		}

		//Black
		else if (m_fCurDamageTime >= 0.8f * m_fDamageEffectTime && m_fCurDamageTime < 1.0f * m_fDamageEffectTime)
		{
			EffectParamSetting(Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), m_WhiteTex);
		}
	}
}


void CActorScript::EffectParamSetting(Vec4 Diff, Vec4 Emis, Ptr<CTexture> _Tex)
{
	int vecOriginalMtrlSize = m_vecOriginMtrl.size();

	const vector<CGameObject*>& ChildVec = GetObj()->GetChild();

	for (int i = 0; i < vecOriginalMtrlSize; ++i)
	{
		if (ChildVec[i]->MeshRender() && nullptr == ChildVec[i]->Collider3D())
		{
			//Ŭ�е� ���׸��� 
			Ptr<CMaterial> Mtrl = ChildVec[i]->MeshRender()->GetSharedMaterial(0);

			if (Mtrl->GetKey() != L"CM")
			{
				Mtrl = ChildVec[i]->MeshRender()->GetCloneMaterial(0);
			}


			Mtrl->SetDiffuse(Diff);
			Mtrl->SetEmissive(Emis);

			Mtrl->SetData(SHADER_PARAM::TEX_0, _Tex.Get());
			Mtrl->SetData(SHADER_PARAM::TEX_3, _Tex.Get());
		}
	}
}

void CActorScript::CreateDeadParticle()
{

}

void CActorScript::CreateCollider(UINT _LayerIdx, Vec3 _Scale, Vec3 OffsetPos)
{
	CGameObject* Obj = new CGameObject;

	Obj->SetName(L"Col");
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

CGameObject* CActorScript::IntanciatePrefab(wstring _wstr, UINT _LayerIdx)
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
	
	if (VecNavMesh.size() == 0 || VecNavMesh.size() >= m_iCurNavMeshIdx)
	{
		m_iCurNavMeshIdx = 0;
		return false;
	}

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
	DamageEffectupdate();

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

void CActorScript::awake()
{
	m_PaperBurnTex = CResMgr::GetInst()->FindRes<CTexture>(L"PaperBurnTexture");

	if (nullptr == m_PaperBurnTex)
		m_PaperBurnTex = CResMgr::GetInst()->Load<CTexture>(L"PaperBurnTexture", L"texture\\PaperBurn\\PaperBurnTexture.jpg");



	m_WhiteTex = CResMgr::GetInst()->FindRes<CTexture>(L"WhiteTex");

	if (nullptr == m_WhiteTex)
		m_WhiteTex = CResMgr::GetInst()->Load<CTexture>(L"WhiteTex", L"texture\\FBXTexture\\WhiteTex.png");


	m_RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"RedTex");

	if (nullptr == m_RedTex)
		m_RedTex = CResMgr::GetInst()->Load<CTexture>(L"RedTex", L"texture\\FBXTexture\\RedTex.png");


	if (m_vecOriginMtrl.size() != 0)
		return;

	//int MtrlCount = 0;
	int ChildCount = 0;

	const vector<CGameObject*>& ChildVec = GetObj()->GetChild();
	ChildCount = ChildVec.size();

	for (int i = 0; i < ChildCount; ++i)
	{
		if (ChildVec[i]->MeshRender() && nullptr == ChildVec[i]->Collider3D())
		{
			int Count = ChildVec[i]->MeshRender()->GetMtrlCount();

			for (UINT j = 0; j < Count; ++j)
			{
				m_vecOriginMtrl.push_back(ChildVec[i]->MeshRender()->GetSharedMaterial(j));
			}

		}
	}
}

void CActorScript::OnCollisionEnter(CGameObject* _pOther)
{
	if(_pOther->GetLayerIndex()==(UINT)LAYER_TYPE::PLAYER_ATTACK_COL)
		GetDamage();
}


void CActorScript::ActivateImpactParticle(Vec4 EmisColor, Vec3 _Pos ,Vec3 _Dir, int ActivateCount, float SpreadRange, Vec2 Scale, Vec2 Speed)
{
	for (int i = 0; i < ActivateCount; ++i)
	{
		CGameObject* pAttackImpact = CMemoryPoolScript::GetAttackImpact();

		if (nullptr == pAttackImpact)
			return;

		Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 vRight = _Dir.Cross(vUp);
		vRight.Normalize();

		float iRand = CRandomMgrScript::GetRandomintNumber(-SpreadRange, SpreadRange);
		iRand /= 10.f;

		if (vRight.Length() == 0.f)
			continue;

		XMMATRIX matR = XMMatrixRotationAxis(vRight, iRand);


		iRand = CRandomMgrScript::GetRandomintNumber(-SpreadRange, SpreadRange);
		iRand /= 10.f;


		if (vUp.Length() == 0.f)
			continue;


		XMMATRIX matU = XMMatrixRotationAxis(vUp, iRand);

		Vec3 Dir = _Dir;

		Dir = XMVector3TransformCoord(Dir, matU);
		Dir = XMVector3TransformCoord(Dir, matR);


		((CAttackImpactScript*)pAttackImpact->GetScript())->SetDir(Dir);


		iRand = CRandomMgrScript::GetRandomintNumber(Scale.x, Scale.y);

		((CAttackImpactScript*)pAttackImpact->GetScript())->SetScale(iRand);

		iRand = CRandomMgrScript::GetRandomintNumber(Speed.x, Speed.y);

		((CAttackImpactScript*)pAttackImpact->GetScript())->SetSpeed(iRand);


		pAttackImpact->Transform()->SetLocalPos(_Pos);

		Vec3 Rot = pAttackImpact->Transform()->GetLocalRot();
		Vec3 vFront = pAttackImpact->Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Vec3 vCross = Dir.Cross(vFront);
		Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
		vRight = GetObj()->Transform()->GetLocalDir(DIR_TYPE::RIGHT);

		                 
		float dot = vCross.Dot(vUP);
		Dir.Normalize();

		float RotAngle = vFront.Dot(Dir);
		RotAngle = acos(RotAngle);

		if (dot > 0.0)
			Rot.y -= RotAngle;

		else if (dot < 0.0)
			Rot.y += RotAngle;


		vCross = Dir.Cross(vRight);
		dot = vCross.Dot(vRight);
		Dir.Normalize();

	    RotAngle = vRight.Dot(Dir);
		RotAngle = acos(RotAngle);

		if (dot > 0.0)
			Rot.x -= RotAngle;

		else if (dot < 0.0)
			Rot.x += RotAngle;

		pAttackImpact->Transform()->SetLocalRot(Rot);

		pAttackImpact->MeshRender()->GetSharedMaterial(0)->SetEmissive(EmisColor);
	}
}

CActorScript::CActorScript()
	: CScript((UINT)SCRIPT_TYPE::ACTORSCRIPT)
	, m_fOffSetPos(10.f)
	, m_bJump(false)
	, RayDir(-1)
	, m_bDamaged(false)
	, m_fDamageEffectTime(0.4f)
	, m_fCurDamageTime(0.f)
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

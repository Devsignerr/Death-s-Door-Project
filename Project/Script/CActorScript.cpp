#include "pch.h"
#include "CActorScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTimeMgr.h>

bool CActorScript::GroundCheck()
{
	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjectPos = Transform()->GetLocalPos();
	ObjectPos.y += m_fOffSetPos;

	DirectX::XMVECTOR ObjCenter = ObjectPos;
	DirectX::XMVECTOR LayDir = -Transform()->GetWorldDir(DIR_TYPE::UP);

	CGameObject* Map = CSceneMgr::GetInst()->FindObjectByName(L"M");

	CGameObject* NavMeshObj = Map->GetChild()[0];
	CMesh* pNavMesh = NavMeshObj->MeshRender()->GetMesh().Get();

	vector<tNavMeshNode> vecNavMesh = pNavMesh->GetMeshNodeVec();

	const Matrix& MapMat = Map->Transform()->GetWorldMat();

	// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
	DirectX::XMVECTOR MapPoint1 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.0f), MapMat);
	DirectX::XMVECTOR MapPoint2 = DirectX::XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.0f), MapMat);
	DirectX::XMVECTOR MapPoint3 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.0f), MapMat);

	//DirectX::XMVECTOR MapPoint1 = m_NaviMesh->VTXPos[0];
	//DirectX::XMVECTOR MapPoint2 = m_NaviMesh->VTXPos[1];
	//DirectX::XMVECTOR MapPoint3 = m_NaviMesh->VTXPos[2];



	if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		if (m_Dist > 100.0f)
		{
			Vec3 Pos = Transform()->GetLocalPos();
			Pos.y -= CTimeMgr::GetInst()->GetfDT() * 200.0f;
			Transform()->SetLocalPos(Pos);
		}
	}
	else
	{
		bool NodeCheck = false;
		
		for (int idx = 0; idx < vecNavMesh.size(); ++idx)
		{
			MapPoint1 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[0], MapMat);
			MapPoint2 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[1], MapMat);
			MapPoint3 = DirectX::XMVector3TransformCoord(vecNavMesh[idx].VertexPosition[2], MapMat);
			
			if (DirectX::TriangleTests::Intersects(ObjCenter, LayDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;

				if (m_Dist > 100.0f)
				{
					Vec3 Pos = Transform()->GetLocalPos();
					Pos.y -= CTimeMgr::GetInst()->GetfDT() * 200.0f;
					Transform()->SetLocalPos(Pos);
				}
			}
		}


		if (false == NodeCheck)
		{
			Vec3 Pos = Transform()->GetLocalPos();
			Pos.y -= CTimeMgr::GetInst()->GetfDT() * 200.0f;
			Transform()->SetLocalPos(Pos);
		}

		//ResearchNode();
	}




	return false;
}

void CActorScript::ResearchNode()
{
}

void CActorScript::update()
{
	GroundCheck();
}

void CActorScript::OnCollisionEnter(CGameObject* _pOther)
{
}

CActorScript::CActorScript()
	: CScript((UINT)SCRIPT_TYPE::ACTORSCRIPT),
	m_fOffSetPos(10.f)
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

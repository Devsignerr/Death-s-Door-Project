#include "pch.h"
#include "CActorScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTimeMgr.h>

bool CActorScript::GroundCheck()
{
	const Matrix& ObjWorld = Transform()->GetWorldMat();

	Vec3 ObjOffsetPos = Transform()->GetLocalPos();
	ObjOffsetPos.y += 10.0f;

	DirectX::XMVECTOR ObjCenter = ObjOffsetPos;
	DirectX::XMVECTOR ObjDir = -Transform()->GetWorldDir(DIR_TYPE::UP);

	CGameObject* Map = CSceneMgr::GetInst()->FindObjectByName(L"Plane");
	const Matrix& MapMat = Map->Transform()->GetWorldMat();

	// XMVECTOR는 XMVector3TransformCoord 할때 y와 z를 서로 바꿔서 넣어야 제대로된 값이 나온다
	DirectX::XMVECTOR MapPoint1 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.0f), MapMat);
	DirectX::XMVECTOR MapPoint2 = DirectX::XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.0f), MapMat);
	DirectX::XMVECTOR MapPoint3 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.0f), MapMat);

	//DirectX::XMVECTOR MapPoint1 = m_NaviMesh->VTXPos[0];
	//DirectX::XMVECTOR MapPoint2 = m_NaviMesh->VTXPos[1];
	//DirectX::XMVECTOR MapPoint3 = m_NaviMesh->VTXPos[2];

	if (DirectX::TriangleTests::Intersects(ObjCenter, ObjDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
	{
		if (m_Dist > 10.0f)
		{
			Vec3 Pos = Transform()->GetLocalPos();
			Pos.y -= CTimeMgr::GetInst()->GetfDT() * 200.0f;
			Transform()->SetLocalPos(Pos);
		}
	}
	else
	{
		bool NodeCheck = false;
		float VYXChange = 0.5f;
		for (size_t i = 0; i < 2; ++i)
		{
			MapPoint1 = DirectX::XMVector3TransformCoord(Vec3(0.5f, VYXChange * -1.0f, 0.0f), MapMat);

			if (DirectX::TriangleTests::Intersects(ObjCenter, ObjDir, MapPoint1, MapPoint2, MapPoint3, m_Dist))
			{
				NodeCheck = true;
				break;
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
	: CScript((UINT)SCRIPT_TYPE::ACTORSCRIPT)
{
}

CActorScript::~CActorScript()
{
}

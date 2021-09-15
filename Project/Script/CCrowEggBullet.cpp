#include "pch.h"
#include "CCrowEggBullet.h"
#include "CPlayerScript.h"

#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CCollider3D.h>


void CCrowEggBullet::Create()
{
	CGameObject* Obj = nullptr;
	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

	Obj=IntanciatePrefab(L"CrowChain", (UINT)LAYER_TYPE::BOSS_EFFECT);
	
	CGameObject* Col = new CGameObject;
	Col->SetName(L"BazookBullet_Col");

	Col->AddComponent(new CTransform);
	Col->AddComponent(new CMeshRender);
	Col->AddComponent(new CCollider3D);

	Col->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Col->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));

	Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CurScene->AddObject(Col, (UINT)LAYER_TYPE::INDETERMINATE);

	Obj->Transform()->SetLocalPos(m_StartPos);
	//Obj->Transform()->SetLocalScale(Vec3(500.0f, 500.0f, 500.0f));

	m_CrowBossRot.x += XM_PI / 2.f;
	Vec3 Rot = m_CrowBossRot;
	Rot.y += XM_PI / 2.f;

	Obj->Transform()->SetLocalRot(Rot);

	//Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	//Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::BOSS_EFFECT);

	Obj->AddChild(Col);

	AddSlidingLine(Obj);
}

void CCrowEggBullet::Move()
{

	// 가장 선두
	list<SLIDINGINFO>::iterator Iter = m_SlidingLine.begin();

	// 멈출 조건
	if ((*Iter).MovementValue > m_Range)
		(*Iter).MoveCheck = false;

	for (; Iter != m_SlidingLine.end(); ++Iter)
	{
		list<SLIDINGINFO>::iterator NextIter = Iter;
		float Scale = (*Iter).Obj->GetChild()[0]->Transform()->GetWorldScale().y/4.f;

		if ((*Iter).MovementValue >= Scale)
		{
			NextIter = ++NextIter;
			if (NextIter != m_SlidingLine.end())
				(*NextIter).MoveCheck = true;
			else
				Create();
		}

		if (true == (*Iter).MoveCheck)
		{
			Vec3 Pos = (*Iter).Obj->Transform()->GetLocalPos();
			float MovementValue = 40.f;
			Pos += m_Dir * MovementValue;
			(*Iter).MovementValue += MovementValue;

			(*Iter).Obj->Transform()->SetLocalPos(Pos);
		}
	}

}

void CCrowEggBullet::Dead()
{
#pragma region Test



	/*if (true != m_SlidingLine.empty())
	{
		list<SLIDINGINFO>::reverse_iterator Iter = m_SlidingLine.rbegin();

		for (; Iter != m_SlidingLine.rend();)
		{
			if (false == (*Iter).Obj->IsDead())
			{
				DeleteObject((*Iter).Obj);
				m_SlidingLine.erase(--(Iter.base()));
				break;
			}
			else
			{
				++Iter;
			}
		}
	}
	else
	{
	}*/
#pragma endregion
	if (true != m_SlidingLine.empty())
	{
		list<SLIDINGINFO>::iterator Iter = m_SlidingLine.begin();

		for (; Iter != m_SlidingLine.end();)
		{
			if (true == (*Iter).DeadCheck)
			{
				DeleteObject((*Iter).Obj);
				m_SlidingLine.erase(Iter);
				break;
			}
			else
			{
				++Iter;
			}
		}
	}
}

void CCrowEggBullet::FindToDeadCheck(CGameObject* _Obj)
{
	if (true != m_SlidingLine.empty())
	{
		list<SLIDINGINFO>::iterator Iter = m_SlidingLine.begin();
		for (; Iter != m_SlidingLine.end(); ++Iter)
		{
			if ((*Iter).Obj == _Obj->GetParent())
			{
				(*Iter).DeadCheck = true;
				break;
			}
		}

	}
}

void CCrowEggBullet::awake()
{
	Create();
	m_SlidingLine.front().MoveCheck = true;
}

void CCrowEggBullet::update()
{
	if (!m_SlidingLine.empty())
	{
		SLIDINGINFO Obj = m_SlidingLine.front();

		if (Obj.MoveCheck)
			Move();
		else
			Dead();
	}
	else
	{
		m_SlidingLine.clear();
		DeleteObject(GetGameObject());
	}
}

void CCrowEggBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCrowEggBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}


CCrowEggBullet::CCrowEggBullet()
	: m_Range(0.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::CROWEGGBULLET;
}

CCrowEggBullet::~CCrowEggBullet()
{
}
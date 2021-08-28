#include "pch.h"
#include "CCrowEggBullet.h"
#include "CPlayerScript.h"

#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CCollider3D.h>


void CCrowEggBullet::Create()
{
	CGameObject* Obj = new CGameObject;

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(m_StartPos);
	Obj->Transform()->SetLocalScale(Vec3(500.0f, 500.0f, 500.0f));
	Obj->Transform()->SetLocalRot(m_CrowBossRot);

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::INDETERMINATE);

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
		float Scale = (*Iter).Obj->Transform()->GetLocalScale().y / 2.0f;

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
			float MovementValue = fDT * 10000.0f;
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
			if ((*Iter).Obj == _Obj)
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
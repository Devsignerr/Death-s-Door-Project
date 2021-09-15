#include "pch.h"
#include "CFence.h"

#include <Engine/CCollider3D.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

bool CFence::m_IsOpen = false;

void CFence::awake()
{
	Vec3 Down = -Transform()->GetLocalDir(DIR_TYPE::UP);
	m_DestPos = Transform()->GetLocalPos() + Down * 700.0f;
	CreateWallCol();
	m_PrevOpenCheck = m_IsOpen;
}

void CFence::update()
{
	if (true == m_IsOpen)
	{
		Vec3 Pos = Transform()->GetLocalPos();
		Vec3 FenceWallPos = m_FenceWall->Transform()->GetLocalPos();

		if (!m_PrevOpenCheck)
		{
			Play_Sound(L"VaultDoor");
			m_PrevOpenCheck = true;
		}

		if (m_DestPos.y < Pos.y)
		{
			Pos.y -= fDT * 150.0f;
			Transform()->SetLocalPos(Pos);

			FenceWallPos.y -= fDT * 150.0f;
			m_FenceWall->Transform()->SetLocalPos(FenceWallPos);
		}
		else
		{
			m_IsOpen = false;
		}
	}

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		m_IsOpen = true;
	}
}

void CFence::CreateWallCol()
{
	static int a = 0;

	if (0 == a)
	{
		++a;
		std::queue<WALLINFO> WallInfo = {};
		Vec3 FencePos = Transform()->GetLocalPos();
		WALLINFO Temp = { };

		Temp.Pos = Vec3(FencePos.x + 3339.0f, FencePos.y + 25.0f, FencePos.z + 1174.0f);
		Temp.Scale = Vec3(300.0f, 300.0f, 300.0f);
		WallInfo.push(Temp);

		Temp.Pos = Vec3(FencePos.x + 3276.0f, FencePos.y + 71.0f, FencePos.z - 21.0f);
		Temp.Scale = Vec3(150.0f, 400.0f, 800.0f);
		WallInfo.push(Temp);

		Temp.Pos = Vec3(FencePos.x + 3276.0f, FencePos.y + 71.0f, FencePos.z + 2386.0f);
		Temp.Scale = Vec3(150.0f, 400.0f, 800.0f);
		WallInfo.push(Temp);

		Temp.Pos = Vec3(FencePos.x + 67.0f, FencePos.y + 81.0f, FencePos.z - 64.0f);
		Temp.Scale = Vec3(150.0f, 400.0f, 2500.0f);
		WallInfo.push(Temp);


		for (int i = 0; i < 4; ++i)
		{
			CGameObject* pObj = new CGameObject;
			wchar_t Name[256] = { 0, };
			swprintf_s(Name, L"Wall%d", i);

			pObj->SetName(Name);

			pObj->AddComponent(new CTransform);
			pObj->AddComponent(new CMeshRender);
			pObj->AddComponent(new CCollider3D);

			pObj->Transform()->SetLocalPos(WallInfo.front().Pos);
			pObj->Transform()->SetLocalScale(WallInfo.front().Scale);
			pObj->Transform()->SetLocalRot(WallInfo.front().Rot);

			pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(pObj, (UINT)LAYER_TYPE::WALL_COL);

			WallInfo.pop();

			if (3 == i)
			{
				m_FenceWall = pObj;
			}
		}

	}
}

void CFence::OnCollisionEnter(CGameObject* _pOther)
{
}

void CFence::OnCollision(CGameObject* _pOther)
{
}

void CFence::OnCollisionExit(CGameObject* _pOther)
{
}
void CFence::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CFence::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

CFence::CFence()
	: m_FenceWall(nullptr)
	, m_PrevOpenCheck(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::FENCE;
	m_GimicType = GIMICTYPE::FENCE;
}

CFence::~CFence()
{
}


#include "pch.h"
#include "CMapGimic.h"

#include <Engine/CCollider3D.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void CMapGimic::CreateCol(Vec3 _Scale, Vec3 _OffsetPos, wstring _Name)
{
	CGameObject* Obj = new CGameObject;
	Obj->SetName(_Name);

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(_OffsetPos);
	Obj->Transform()->SetLocalScale(_Scale);

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MAP_GIMIC_COL);

	GetObj()->AddChild(Obj);
}

void CMapGimic::awake()
{
}

void CMapGimic::update()
{
}

CMapGimic::CMapGimic()
	: CScript((UINT)SCRIPT_TYPE::MAPGIMIC)
	, m_GimicType(GIMICTYPE::END)
{
}

CMapGimic::~CMapGimic()
{
}

void CMapGimic::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CMapGimic::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

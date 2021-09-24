#include "pch.h"
#include "CLeverScript.h"
#include "CCameraScript.h"
#include "CGateScript.h"

#include <Engine/CCollider3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>


CLeverScript::CLeverScript():
	m_fRotateTime(2.f),
	m_fCurTime(0.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::LEVERSCRIPT;
	m_GimicType = GIMICTYPE::LEVER;
	m_eState = LEVER_STATE::WAIT;
}

CLeverScript::~CLeverScript()
{
}

void CLeverScript::awake()
{
	CGameObject* Col = new CGameObject;
	Col->SetName(L"Lever_Col");

	Col->AddComponent(new CTransform);
	Col->AddComponent(new CMeshRender);
	Col->AddComponent(new CCollider3D);

	Col->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	Col->Transform()->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CSceneMgr::GetInst()->GetCurScene()->AddObject(Col, (UINT)LAYER_TYPE::MAP_GIMIC_COL);

	Col->MeshRender()->Activate(false);

	Col->SetDynamicShadow(false);
	Col->SetFrustumCheck(false);


	GetObj()->AddChild(Col);



	CGameObject* Obj = new CGameObject;
	Obj->SetName(L"AnChor_Col1");

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(Vec3(-15054, -640, -5666));
	Obj->Transform()->SetLocalScale(Vec3(100.f,1000.f,100.f));

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	Obj->MeshRender()->Activate(false);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MAP_GIMIC_COL);

	Obj = new CGameObject;
	Obj->SetName(L"AnChor_Col2");

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(Vec3(-19118, -640, -8287));
	Obj->Transform()->SetLocalScale(Vec3(100.f, 1000.f, 100.f));

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	Obj->MeshRender()->Activate(false);

	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MAP_GIMIC_COL);
}

void CLeverScript::update()
{
	if (m_eState == LEVER_STATE::OPEN)
	{
		m_fCurTime += fDT;

		if (m_fRotateTime > m_fCurTime)
		{
			Rotate();
		}
	}
}

void CLeverScript::OpenGate()
{
	vector<CGameObject*>& Temp = (vector<CGameObject*>&)CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::MAP_GIMIC)->GetObjects();

	vector<CGameObject*>::iterator iter = Temp.begin();

	for (; iter != Temp.end(); ++iter)
	{
		if ((*iter)->GetScript())
		{
			if (((CMapGimic*)(*iter)->GetScript())->GetGimicType() == GIMICTYPE::GATE)
			{
				((CGateScript*)(*iter)->GetScript())->Open();
			}
		}
	}
}

void CLeverScript::Rotate()
{
	Vec3 LocalRot = Transform()->GetLocalRot();
	LocalRot.x += fDT * 0.5f;
	Transform()->SetLocalRot(LocalRot);
}

void CLeverScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIndex() == (UINT)LAYER_TYPE::PLAYER_ATTACK_COL)
	{
		if (m_eState == LEVER_STATE::WAIT)
		{
			OpenGate();
			m_eState = LEVER_STATE::OPEN;
		}
	}

	CCameraScript::SetCameraShake(0.5f, 100.f, 4.f);
}

void CLeverScript::SaveToScene(FILE* _pFile)
{
	CMapGimic::SaveToScene(_pFile);
}

void CLeverScript::LoadFromScene(FILE* _pFile)
{
	CMapGimic::LoadFromScene(_pFile);
}


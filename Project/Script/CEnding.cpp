#include "pch.h"
#include "CEnding.h"
#include "CFadeScript.h"

#include <Engine/CUI.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CKeyMgr.h>


void CEnding::awake()
{
	static int a = 0;
	if (a == 0)
	{
		Ptr<CTexture> UITex = CResMgr::GetInst()->FindRes<CTexture>(L"DD_Logo_Smooth_Dropshadow");
		if (nullptr == UITex)
			UITex = CResMgr::GetInst()->Load<CTexture>(L"DD_Logo_Smooth_Dropshadow", L"texture\\UI\\DD_Logo_Smooth_Dropshadow.png");

		Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIEndingMtrl");
		{
			CGameObject* Obj = new CGameObject;

			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);


			Obj->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 2.f));
			Obj->Transform()->SetLocalScale(Vec3(1600.f, 900.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);

			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), Obj);
		}
	}
	CFadeScript::SetFadeTime(XM_PI / 8.0f);
	CFadeScript::Fade_In();
}

void CEnding::update()
{
}

CEnding::CEnding()
	: CScript((UINT)SCRIPT_TYPE::ENDING)
{

}

CEnding::~CEnding()
{
}

void CEnding::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CEnding::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

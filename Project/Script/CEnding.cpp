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
			m_Ending = new CGameObject;

			m_Ending->AddComponent(new CTransform);
			m_Ending->AddComponent(new CMeshRender);
			m_Ending->AddComponent(new CUI);

			m_Ending->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 2.f));
			m_Ending->Transform()->SetLocalScale(Vec3(1600.f, 900.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			m_Ending->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			m_Ending->MeshRender()->SetMaterial(Mtrl, 0);

			m_Ending->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_Ending, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), m_Ending);
		}
	}
	//CFadeScript::SetFadeTime(XM_PI / 8.0f);
	//CFadeScript::Fade_In();
}

void CEnding::update()
{
	m_Time += fDT * 1.0f;

	if (m_Time > XM_PI / 2.f)
	{
		m_Time = XM_PI / 2.f;
	}


	if (m_Time > 0.f)
	{
		int a = 1;
		m_Ending->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &a);
		m_Ending->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_Time);
	}
	else
	{
		int a = 1;
		float Time = 0.0f;
		m_Ending->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &a);
		m_Ending->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &Time);
	}
}

CEnding::CEnding()
	: CScript((UINT)SCRIPT_TYPE::ENDING)
	, m_Ending(nullptr)
	, m_Time(-6.0f)
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

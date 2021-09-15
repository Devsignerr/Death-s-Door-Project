#include "pch.h"
#include "CTitle.h"
#include "CFadeScript.h"

#include <Engine/CUI.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CDevice.h>


void CTitle::awake()
{
	static int a = 0;
	if (a == 0)
	{
		Ptr<CTexture> UITex = CResMgr::GetInst()->FindRes<CTexture>(L"title");
		if (nullptr == UITex)
			UITex = CResMgr::GetInst()->Load<CTexture>(L"title", L"texture\\UI\\title.png");

		Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UITitleMtrl");
		{
			m_Title = new CGameObject;

			m_Title->AddComponent(new CTransform);
			m_Title->AddComponent(new CMeshRender);
			m_Title->AddComponent(new CUI);
			m_Title->SetName(L"TitleScene");

			m_Title->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 2.f));
			m_Title->Transform()->SetLocalScale(Vec3(1600.f * 3.2f, 900.f * 3.2f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			m_Title->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			m_Title->MeshRender()->SetMaterial(Mtrl, 0);

			m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_Title, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), m_Title);
		}

		{
			Ptr<CTexture> GameStart_S = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_S");
			if (nullptr == GameStart_S)
				GameStart_S = CResMgr::GetInst()->Load<CTexture>(L"GameStart_S", L"texture\\UI\\GameStart_S.png");

			Ptr<CTexture> GameStart_L = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_L");
			if (nullptr == GameStart_L)
				GameStart_L = CResMgr::GetInst()->Load<CTexture>(L"GameStart_L", L"texture\\UI\\GameStart_L.png");


			Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIFontMtrl");

			m_TitleFont = new CGameObject;

			m_TitleFont->AddComponent(new CTransform);
			m_TitleFont->AddComponent(new CMeshRender);
			m_TitleFont->AddComponent(new CUI);

			m_TitleFont->SetName(L"TitleFont");

			m_TitleFont->Transform()->SetLocalPos(Vec3(0.0f, -700.0f, 0.f));
			m_TitleFont->Transform()->SetLocalScale(Vec3(300.f * 3.2f, 130.f * 3.2f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			m_TitleFont->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			m_TitleFont->MeshRender()->SetMaterial(Mtrl, 0);

			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_S.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_TitleFont, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), m_TitleFont);
		}
	}

	//CFadeScript::SetFadeTime(XM_PI / 8.0f);
	//CFadeScript::Fade_In();
}

void CTitle::update()
{


	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();

	if (666.0f <= MousePos.x && 622.0f <= MousePos.y &&
		925.0f >= MousePos.x && 708.0f >= MousePos.y)
	{
		Ptr<CTexture> GameStart_L = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_L");
		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_L.Get());

		if (KEY_TAP(KEY_TYPE::LBTN))
		{
			m_FadeType = 0;
			m_Time = 0.f;
		}
	}
	else
	{
		Ptr<CTexture> GameStart_S = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_S");
		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_S.Get());
	}

	m_Time += fDT * 1.0f;

	if (m_Time > XM_PI / 2.f)
	{
		m_Time = XM_PI / 2.f;

		if (0 == m_FadeType)
		{
			CSceneMgr::GetInst()->SceneChange(L"Puzzlet");
		}
	}
		

	if (m_Time > 0.f)
	{
		if (0 == m_FadeType)
		{
			m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
			m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_Time);

			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_Time);
		}
		else
		{
			m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
			m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_Time);

			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_Time);

		}
	}
	else 
	{
		float Time = 0.f;

		m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
		m_Title->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &Time);

		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_FadeType);
		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &Time);
	}
}

CTitle::CTitle()
	: CScript((UINT)SCRIPT_TYPE::TITLE)
	, m_TitleFont(nullptr)
	, m_Title(nullptr)
	, m_Fade(true)
	, m_FadeType(1)
	, m_Time(-6.0f)
{

}

CTitle::~CTitle()
{
}

void CTitle::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CTitle::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

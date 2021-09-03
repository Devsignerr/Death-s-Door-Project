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

			m_TitleFont->SetName(L"TitleFont");

			m_TitleFont->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 0.f));
			m_TitleFont->Transform()->SetLocalScale(Vec3(182.f, 181.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			m_TitleFont->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			m_TitleFont->MeshRender()->SetMaterial(Mtrl, 0);

			m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_S.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_TitleFont, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), m_TitleFont);
		}
	}

	m_FontPos = m_TitleFont->Transform()->GetLocalPos();
	m_FontHalfScale = m_TitleFont->Transform()->GetLocalScale() / 2.0f;

	m_FontMinRange = m_FontPos - m_FontHalfScale;
	m_FontMaxRange = m_FontPos + m_FontHalfScale;

	CFadeScript::SetFadeTime(XM_PI / 8.0f);
	CFadeScript::Fade_In();
}

void CTitle::update()
{
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	MousePos.x -= (float)CDevice::GetInst()->GetBufferResolution().x;
	MousePos.y -= (float)CDevice::GetInst()->GetBufferResolution().y;

	if (m_FontMinRange.x <= MousePos.x && m_FontMinRange.y <= MousePos.y &&
		m_FontMaxRange.x >= MousePos.x && m_FontMaxRange.y >= MousePos.y)
	{
		Ptr<CTexture> GameStart_L = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_L");
		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_L.Get());

		if (KEY_TAP(KEY_TYPE::LBTN))
		{
			CFadeScript::Fade_Out();
		}
	}
	else
	{
		Ptr<CTexture> GameStart_S = CResMgr::GetInst()->FindRes<CTexture>(L"GameStart_S");
		m_TitleFont->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, GameStart_S.Get());
	}


}

CTitle::CTitle()
	: CScript((UINT)SCRIPT_TYPE::TITLE)
	, m_TitleFont(nullptr)
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

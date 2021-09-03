#include "pch.h"
#include "CHpBar.h"

#include <Engine/CUI.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void CHpBar::awake()
{
	static int a = 0;
	if (a == 0)
	{
		Ptr<CTexture> UITex = CResMgr::GetInst()->FindRes<CTexture>(L"hud_hp_empty 1");
		if (nullptr == UITex)
			UITex = CResMgr::GetInst()->Load<CTexture>(L"hud_hp_empty 1", L"texture\\UI\\hud_hp_empty 1.png");

		Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIHPMtrl");


		for (UINT i = 0; i < 4; ++i)
		{
			CGameObject* Obj = new CGameObject;

			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);
			Obj->AddComponent(new CUI);

			Obj->Transform()->SetLocalPos(Vec3(150.0f * (float)i, 0.0f, 0.f));
			Obj->Transform()->SetLocalScale(Vec3(150.0f, 100.0f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);

			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());
			Vec4 Temp = Vec4(0.5f, 0.9f, 0.5f, 1.0f);
			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Temp);

			//Obj->UI()->SetUIOffsetPos(Vec3(150.0f * (float)i, 0.0f, 0.f));

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), Obj);
			m_HpBar.push_back(Obj);
		}
	}
	++a;
}

void CHpBar::update()
{
	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		if (0 < m_Hp && true != m_HpBar.empty())
		{
			m_HpBar[m_Hp]->UI()->Activate(false);
			--m_Hp;
		}
	}

	static float Time = 0.0f;

	if (m_Hp != 3)
		Time += fDT;


	if (1.0f < Time && 3 > m_Hp)
	{
		Time = 0.0f;
		++m_Hp;
		m_HpBar[m_Hp]->UI()->Activate(true);
	}

	if (1 == m_Hp)
	{
		Vec4 Temp = Vec4(0.5f, 0.5f, 0.0f, 1.0f);

		for (size_t i = 0; i < m_HpBar.size(); ++i)
			m_HpBar[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Temp);

	
	}
	else if (0 == m_Hp)
	{
		Vec4 Temp = Vec4(0.5f, 0.0f, 0.0f, 1.0f);
		for (size_t i = 0; i < m_HpBar.size(); ++i)
			m_HpBar[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Temp);
	}
	else
	{
		Vec4 Temp = Vec4(0.5f, 0.9f, 0.5f, 1.0f);
		for (size_t i = 0; i < m_HpBar.size(); ++i)
			m_HpBar[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Temp);
	}
	
}


CHpBar::CHpBar()
	: CScript((UINT)SCRIPT_TYPE::HPBAR)
	, m_Hp(3)
{

}

CHpBar::~CHpBar()
{
}

void CHpBar::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CHpBar::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

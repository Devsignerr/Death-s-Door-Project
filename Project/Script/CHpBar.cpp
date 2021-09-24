#include "pch.h"
#include "CHpBar.h"
#include "CPlayerScript.h"

#include <Engine/CUI.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

bool CHpBar::m_HitCheck = false;

void CHpBar::awake()
{
    static int a = 0;
    //if (a == 0)
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
            Obj->Transform()->SetLocalScale(Vec3(150.0f, 70.0f, 1.f));
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

        UITex = CResMgr::GetInst()->FindRes<CTexture>(L"HpCase");
        if (nullptr == UITex)
            UITex = CResMgr::GetInst()->Load<CTexture>(L"HpCase", L"texture\\UI\\HP_Bound_UI.png");

        Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIHPCASEMtrl");


        {
            CGameObject* Obj = new CGameObject;

            Obj->AddComponent(new CTransform);
            Obj->AddComponent(new CMeshRender);
            Obj->AddComponent(new CUI);

            Obj->Transform()->SetLocalPos(Vec3(225.f, 0.f, 0.f));
            Obj->Transform()->SetLocalScale(Vec3(645.0f, 105.0f, 1.f));

            Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
            Obj->MeshRender()->SetMaterial(Mtrl, 0);

            Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());

            int a = 1;
            Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_1, &a);

            CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
            CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);

            AddChild(GetObj(), Obj);

        }
    }
    ++a;

    Transform()->SetLocalPos(Vec3(-2000.0f, 1250.0f, 50.f));
}

void CHpBar::update()
{
    if (true == m_HitCheck)
    {
        m_HitCheck = false;

        if (0 < m_Hp && true != m_HpBar.empty())
        {
            m_HpBar[m_Hp]->UI()->Activate(false);
            --m_Hp;
        }
    }

    static float Time = 0.0f;

    if (m_Hp != 3)
        Time += fDT;


    if (5.0f < Time && 3 > m_Hp)
    {
        Time = 0.0f;
        ++m_Hp;
        m_HpBar[m_Hp]->UI()->Activate(true);
    }

    if (1 == m_Hp)
    {
        Vec4 Temp = Vec4(0.9f, 0.9f, 0.0f, 1.0f);

        for (size_t i = 0; i < m_HpBar.size(); ++i)
            m_HpBar[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Temp);


    }
    else if (0 == m_Hp)
    {
        Vec4 Temp = Vec4(0.8f, 0.0f, 0.2f, 1.0f);
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
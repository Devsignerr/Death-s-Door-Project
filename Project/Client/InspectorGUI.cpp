#include "pch.h"
#include "InspectorGUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CSceneMgr.h>
#include <Engine\CGameObject.h>

#include <Engine\CRes.h>
#include <Engine/CResMgr.h>

#include <Script\CScriptMgr.h>

#include "ComponentGUI.h"
#include "TransformGUI.h"
#include "MeshRenderGUI.h"
#include "Collider2DGUI.h"
#include "Collider3DGUI.h"
#include "ScriptGUI.h"
#include "Animator3DGUI.h"
#include "Light3DGUI.h"
#include "Particle3DGUI.h"
#include "FrustumGUI.h"
#include "CameraGUI.h"
#include "UIGUI.h"


#include "ResInfoGUI.h"
#include "MaterialGUI.h"
#include "PrefabGUI.h"

InspectorGUI::InspectorGUI()
    : m_arrComGUI{}
    , m_arrResInfoGUI{}
    , m_pTargetObj(nullptr)
    , m_pTargetRes(nullptr)
    , m_bItemChanged(false)
{
}

InspectorGUI::~InspectorGUI()
{
    Safe_Delete_Array(m_arrComGUI);
    Safe_Delete_Array(m_arrResInfoGUI);
    Safe_Delete_Vector(m_vecScriptGUI);
}

void InspectorGUI::init()
{
    ComponentGUI* pNew = new TransformGUI;
  
    pNew->SetName(L"Transform");
    pNew->SetSize(Vec2(0.f, 130.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::TRANSFORM] = pNew;

    pNew = new MeshRenderGUI;
  
    pNew->SetName(L"MeshRender");
    pNew->SetSize(Vec2(0.f, 80.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::MESHRENDER] = pNew;

    pNew = new Collider2DGUI;
   
    pNew->SetName(L"Collider2D");
    pNew->SetSize(Vec2(0.f, 100.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = pNew;

    pNew = new Collider3DGUI;

    pNew->SetName(L"Collider3D");
    pNew->SetSize(Vec2(0.f, 100.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::COLLIDER3D] = pNew;

    pNew = new Light3DGUI;

    pNew->SetName(L"Light3DGUI");
    pNew->SetSize(Vec2(0.f, 200.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::LIGHT3D] = pNew;

    pNew = new Particle3DGUI;

    pNew->SetName(L"Particle3DGUI");
    pNew->SetSize(Vec2(0.f, 200.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::PARTICLE] = pNew;

    pNew = new Animator3DGUI;

    pNew->SetName(L"Animator3D");
    pNew->SetSize(Vec2(0.f, 350.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::ANIMATOR3D] = pNew;

    pNew = new CameraGUI;

    pNew->SetName(L"Camera");
    pNew->SetSize(Vec2(0.f, 300.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::CAMERA] = pNew;

    pNew = new FrustumGUI;

    pNew->SetName(L"Frustum Sphere");
    pNew->SetSize(Vec2(0.f, 100.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::FRUSTUMSPHERE] = pNew;

    pNew = new UIGUI;

    pNew->SetName(L"UI");
    pNew->SetSize(Vec2(0.f, 100.f));
    m_arrComGUI[(UINT)COMPONENT_TYPE::UI] = pNew;


    for (int i = 0; i < 10; ++i)
    {
        m_vecScriptGUI.push_back(new ScriptGUI);
    }    

    ResInfoGUI* pResInfoGUI = new MaterialGUI;
    pResInfoGUI->SetName(L"Material");
    m_arrResInfoGUI[(UINT)RES_TYPE::MATERIAL] = pResInfoGUI;

    PrefabGUI* pPrefabGUI = new PrefabGUI;
    pPrefabGUI->SetName(L"PrefabGUI");
    m_arrResInfoGUI[(UINT)RES_TYPE::PREFAB] = pPrefabGUI;
}

void InspectorGUI::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->SetTargetObj(m_pTargetObj);
		m_arrComGUI[i]->update();
	}

    if (m_pTargetObj)
    {
        const vector<CScript*> vecScript = m_pTargetObj->GetScripts();
        for (size_t i = 0; i < vecScript.size(); ++i)
        {
            if (m_vecScriptGUI.size() <= i)
            {
                m_vecScriptGUI.push_back(new ScriptGUI);
            }

            m_vecScriptGUI[i]->SetTargetObj(m_pTargetObj);
            m_vecScriptGUI[i]->SetSize(Vec2(0.f, 100.f));
            m_vecScriptGUI[i]->update();
        }
    }
    else
    {
        for (size_t i = 0; i < m_vecScriptGUI.size(); ++i)
        {
            m_vecScriptGUI[i]->SetTargetObj(nullptr);
            m_vecScriptGUI[i]->SetSize(Vec2(0.f, 100.f));
            m_vecScriptGUI[i]->update();
        }        
    }
	
   
    if (m_pTargetRes)
    {
        if (nullptr != m_arrResInfoGUI[(UINT)m_eResType])
        {
            m_arrResInfoGUI[(UINT)m_eResType]->SetTargetRes(m_pTargetRes);
            m_arrResInfoGUI[(UINT)m_eResType]->update();
        }
    }
}

void InspectorGUI::render()
{
    ImGui::Begin("Inspector");
  
    if (m_pTargetObj)
    {
        if (m_pTargetObj->IsDead() == true)
            m_pTargetObj = nullptr;
    }

    if (m_pTargetObj)
    {
        std::wstring ObjName = L"Object Name : ";
        ObjName += m_pTargetObj->GetName();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.5f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.5f, 0.5f, 0.5f));
        ImGui::Button(GetString(ObjName));
        ImGui::PopStyleColor(3);

        ImGui::NewLine();

        static bool DynamicShadow = false;
        static bool FrustumCulling = false;

        if (m_bItemChanged)
        {
            DynamicShadow = m_pTargetObj->IsDynamicShdow();
            FrustumCulling = m_pTargetObj->IsFrustum();

            m_bItemChanged = false;
        }

        ImGui::Checkbox("DynamicShadow", &DynamicShadow);
        if (true == DynamicShadow)
        {
            m_pTargetObj->SetDynamicShadow(true);
        }
        else 
        {
            m_pTargetObj->SetDynamicShadow(false);
        }

        ImGui::SameLine();

        ImGui::Checkbox("FrustumCulling", &FrustumCulling);
        if (true == FrustumCulling)
        {
            m_pTargetObj->SetFrustumCheck(true);
        }
        else
        {
            m_pTargetObj->SetFrustumCheck(false);
        }


        for(UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
        {
            if (nullptr == m_arrComGUI[i])
                continue;
                        
            if (m_arrComGUI[i]->IsActive())
            {
                wstring ComponentName = m_arrComGUI[i]->GetName();
                string strName = GetString(ComponentName);

                //ImGui::SetNextTreeNodeOpen(true);

                if (ImGui::CollapsingHeader(strName.c_str()))
                {
                    m_arrComGUI[i]->render();
                }
                ImGui::Separator();
            }             
        }

        // Object Script 에 대응하는 ScriptGUI render
        const vector<CScript*> vecScript = m_pTargetObj->GetScripts();
        for (size_t i = 0; i < vecScript.size(); ++i)
        {                                
            m_vecScriptGUI[i]->SetName(CScriptMgr::GetScriptName(vecScript[i]));                   
            m_vecScriptGUI[i]->SetScript(vecScript[i]);
            m_vecScriptGUI[i]->render();
        }

        static char szEmpty[100] = {};
        ImGui::InputText("PrefabName", szEmpty, IM_ARRAYSIZE(szEmpty));

        if (ImGui::Button("Object Name"))
        {
            int length = strlen(szEmpty);
            wstring ObjectName(szEmpty, &szEmpty[length]);
            m_pTargetObj->SetName(ObjectName);
        }

        if (ImGui::Button("make Prefab"))
        {
            Ptr<CPrefab> pPrefab = nullptr;

            int Layer = m_pTargetObj->GetLayerIndex();         

            CGameObject* ProtoObject = m_pTargetObj->Clone();
            ProtoObject->SetName(m_pTargetObj->GetName());

            UINT ChildCount = m_pTargetObj->GetChild().size();

            for (UINT i = 0; i < ChildCount; ++i)
            {
                wstring ChildName = m_pTargetObj->GetChild()[i]->GetName();
                UINT ChildLayer = m_pTargetObj->GetChild()[i]->GetLayerIndex();

                ProtoObject->GetChild()[i]->SetName(ChildName);
                ProtoObject->GetChild()[i]->SetLayerIndex(ChildLayer);
            }

            ProtoObject->SetLayerIndex(Layer);

            pPrefab = new CPrefab(ProtoObject);
            wstring Name = m_pTargetObj->GetName();
            Name = L"prefab\\" + Name + L".pref";
            pPrefab->Save(Name);
            CResMgr::GetInst()->AddRes<CPrefab>(m_pTargetObj->GetName(), pPrefab);
            
        }

        int LayerIdx = m_pTargetObj->GetLayerIndex();
        char strIdx[10] = {};
        _itoa_s(LayerIdx, strIdx, 10);

        static int CLayerIdx = m_pTargetObj->GetLayerIndex();

        ImGui::Text("Current Layer");
        ImGui::SameLine();
        ImGui::Text(strIdx);

        ImGui::Text("Change LayerIdx");
        ImGui::InputInt("##changeLabel", &CLayerIdx);
        ImGui::SameLine();

        if (ImGui::Button("Confirm Change"))
        {
            m_pTargetObj->ChangeLayerIdx(CLayerIdx);
        }




    }
    else if (m_pTargetRes)
    {
        if (nullptr != m_arrResInfoGUI[(UINT)m_eResType])
        {
            m_arrResInfoGUI[(UINT)m_eResType]->render();
        }
    }

    ImGui::End();
}

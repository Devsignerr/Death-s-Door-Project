#include "pch.h"
#include "MenuGUI.h"

#include "CSaveLoadMgr.h"
#include "InspectorGUI.h"
#include "CImGUIMgr.h"

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CResMgr.h>

#include <Engine/CCore.h>
#include <tchar.h>

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider3D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCamera.h>
#include <Engine/CLight3D.h>
#include <Engine/CFrustumSphere.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CEventMgr.h>
#include <Engine/CUI.h>
#include <Engine/CCollisionMgr.h>
#include "CSaveLoadMgr.h"

#include <Engine/CScript.h>
#include <Script/CScriptMgr.h>
#include <Script/CMapChange.h>

#include "MRTGUI.h"

MenuGUI::MenuGUI()
{
}

MenuGUI::~MenuGUI()
{
}


void MenuGUI::update()
{
}

void MenuGUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {       
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Save Scene"))
            {
                static char szAnimName[30] = {};

                ImGui::Text("Scene Name");
                ImGui::SameLine();
                ImGui::InputText("##Input SceneName", szAnimName, 30);

                if (ImGui::Button("Save##Scaene Save"))
                {
                    int strLength = strlen(szAnimName);
                    wstring wstr(szAnimName, &szAnimName[strLength]);

                    wstr = L"scene\\" + wstr + L".scene";

                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    CSaveLoadMgr::SaveScene(CurScene, wstr);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Load Scene"))
            {
                if (ImGui::Button("Load Scene"))
                {
                    OPENFILENAME ofn = {};       // common dialog box structure
                    wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                    wstring strMapFilePath = CPathMgr::GetResPath();
                    strMapFilePath += L"scene\\";

                    // Initialize OPENFILENAME   
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = _T("All\0*.*\0??????\0*.bmp\0");
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = strMapFilePath.c_str(); // ?????? ???? ????
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    GetOpenFileName(&ofn);
                    wstring strOutput = ofn.lpstrFile;

                    wchar_t* wstr = (wchar_t*)strOutput.c_str();
                    wstring FileName;
                    int Length = wcslen(wstr);
                    static int count = 0;

                    for (int i = Length - 1; i >= 0; --i)
                    {
                        if (L'\\' == wstr[i])
                        {
                            break;
                        }
                        FileName.push_back(wstr[i]);
                    }

                    reverse(FileName.begin(), FileName.end());

                    FileName;

                    FileName = L"scene\\" + FileName;

                    CScene* pScene = new CScene;

                    CResMgr::GetInst()->GetNavMeshVec().clear();

                    CSaveLoadMgr::LoadScene(pScene, FileName);

                    CSceneMgr::GetInst()->ChangeScene(pScene);

                    // InspeactorGUI ?? ???? ???????? ??????
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    pInspector->SetTargetObject(nullptr);
                }

                ImGui::Separator();

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("LoadFBX")) 
        {
            static char cFBXName[64];
            Ptr<CMeshData> pMeshData = nullptr;
            CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
            int idx = 0;
            static int LayerIdx = 0;

            ImGui::TextColored(ImVec4(0.6f, 0.2f, 0.2f, 1.0f), "Create New Anim"); ImGui::NewLine();

            ImGui::Text("FBXName          \t");  ImGui::SameLine(); ImGui::PushItemWidth(100); ImGui::InputText("##LoadFBXName", cFBXName, 64);


            ImGui::Text("Instanciate Layer\t");  ImGui::SameLine(); ImGui::PushItemWidth(100); ImGui::InputInt("##Layer", &LayerIdx);


            string StrFbxName = cFBXName;
            wstring wFBXName = wstring(StrFbxName.begin(), StrFbxName.end());
            wstring wFBXPath = wFBXName + L".fbx";

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();

            const char* items[] = {"NAVMESH_LOAD","OBJECT_LOAD","MAP_LOAD","ANIMATION_LOAD"};
            static int item_current_idx = 0;
            const char* combo_label = items[item_current_idx];  

            ImGui::PushItemWidth(200);

            if (ImGui::BeginCombo("Select Load Type", combo_label))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            //item_current_idx;

            ImGui::NewLine();

            FBXLOAD_TYPE Type = (FBXLOAD_TYPE)item_current_idx;

            if (ImGui::MenuItem("Load"))
            {                           
                CResMgr::GetInst()->LoadFBX(wFBXPath, Type);
            }

            if (ImGui::MenuItem("Instanciate"))
            {

                //???? ?????? ???????? ?????? ?????? ?? ?????? ???? ?????? ???? ???????? 
                //?????? ???? = FBX ?????? ???? 
                //?????? ???? = ???? ???? 
                //?????? ???? = ???????? ?????? ?????? 
                CGameObject* pParent = CreateParentObj(wFBXName, Type, LayerIdx);

                     
                //FBX ?????? ???????? meshdata?? ???????? ?????? ???????????? ???? ?????????? .???????? ???????? ???????? . 
                while (true) {

                    if (nullptr == CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat"))
                        pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat", L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat");
                    else
                    {
                        pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat");
                    }

                    if (pMeshData->IsLoadFail())
                        break;


                    CGameObject* pObj = nullptr;

                    //?????? ?????? ???? + child + ???????????? ?????? ?????? 
                    //(???? ?????????????? ????????)
                    wstring str = { wFBXName + L" Child" + std::to_wstring(idx) };
                    pObj = pMeshData->Instantiate((FBXLOAD_TYPE)item_current_idx);
                    pObj->SetName(str);

                    
                    //?????????? ???? ???????? ?????? ???????? ???? 
                    if ((FBXLOAD_TYPE)item_current_idx != FBXLOAD_TYPE::NAVMESH_LOAD)
                    {
                        pObj->SetDynamicShadow(true);
                    }
                    else 
                    {
                        pObj->Transform()->SetLocalScale(Vec3(20.f, 20.f, 20.f));
                        pObj->Transform()->SetLocalRot(Vec3(-90.f * XM_PI / 180.f, 180.f * XM_PI / 180.f, 0.f));
                        //???? ???????? ???? ???????? ????
                        vector<CGameObject*>& pVecNavMesh = CResMgr::GetInst()->GetNavMeshVec();

                        //?????? ResMgr?? ???????????? ?????? ?????? Nav???? ???????? ?????? ???? 
                        pVecNavMesh.push_back(pObj);
                    
                    }

                    //???? ?????? ?????? ?????? ?????????? 0?? ???????? ?????? 
                    //(???? ???????? ???? ?????? ?????? ???? ?????? ?????????? ???????? ???? ???????????? )
                    pCurScene->AddObject(pObj, LayerIdx);
                    pParent->AddChild(pObj);

                    idx++;
                }

                //???? ?????????? MeshData?? Instantiate ?????? ???? ?????????? ?????? ?????? ???? (?????? tVecClip?? ????)
                //??????  ?????? ?????? ?????? ?????? ???? AnimClip?? ???????? 
                if ((FBXLOAD_TYPE)item_current_idx == FBXLOAD_TYPE::ANIMATION_LOAD)
                {
                    Ptr<CMesh> mesh = pParent->GetChild()[0]->MeshRender()->GetMesh();

                    pParent->Animator3D()->CopyAnimClip((vector<tMTAnimClip>*)mesh->GetAnimClip());
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObject"))
        {
            if (ImGui::BeginMenu("Create Empty Object"))
            {
                static int LayerIdx = 0;
                static char szEmpty[100] = {};

                ImGui::Text("Setting Layer \t");
                ImGui::SameLine();
                ImGui::InputInt("##LayerSetting", &LayerIdx);

                ImGui::Text("Setting ObjectName");
                ImGui::SameLine();
                ImGui::InputText("##NameSetting", szEmpty, IM_ARRAYSIZE(szEmpty));


                if (ImGui::Button("Confirm"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

                    CGameObject* pGameObject = new CGameObject;

                    int length = strlen(szEmpty);
                    wstring ObjectName(szEmpty, &szEmpty[length]);
                    pGameObject->SetName(ObjectName);

                    tEvent even = {};

                    even.eEvent = EVENT_TYPE::CREATE_OBJECT;
                    even.lParam = (DWORD_PTR)pGameObject;
                    even.wParam = LayerIdx;

                    CEventMgr::GetInst()->AddEvent(even);
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Delete Object"))
            {
                InspectorGUI* pInspectorGUI = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                CGameObject* pObject = pInspectorGUI->GetTargetObject();


                if (nullptr != pObject)
                {
                    tEvent even = {};

                    even.eEvent = EVENT_TYPE::DELETE_OBJECT;
                    even.lParam = (DWORD_PTR)pObject;

                    CEventMgr::GetInst()->AddEvent(even);
                }

                pInspectorGUI->SetTargetObject(nullptr);
            }

            if (ImGui::BeginMenu("Add Component"))
            {              
                if (ImGui::MenuItem("Transform"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->Transform())
                    {
                        pTempObject->AddComponent(new CTransform);

                        pTempObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
                        pTempObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
                        pTempObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
                    }
                }


                if (ImGui::MenuItem("Collider3D"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->Collider3D())
                    {
                        pTempObject->AddComponent(new CCollider3D);
                        pTempObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
                    }
                }

                if (ImGui::MenuItem("MeshRender"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->MeshRender())
                    {
                        pTempObject->AddComponent(new CMeshRender);
                        pTempObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
                        pTempObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
                  
                    }                
                }

                if (ImGui::MenuItem("Light3D"))
                {

                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->Light3D())
                    {
                        pTempObject->AddComponent(new CLight3D);
                        pTempObject->Light3D()->SetLightType(LIGHT_TYPE::POINT);
                    }
                }

                if (ImGui::MenuItem("UI"))
                {

                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->UI())
                    {
                        pTempObject->AddComponent(new CUI);
                    }
                }

                if (ImGui::MenuItem("ParticleSystem"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->ParticleSystem())
                    {
                        pTempObject->AddComponent(new CParticleSystem);
                    }
               
                }

                if (ImGui::MenuItem("Frustum Sphere"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
                    InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                    CGameObject* pTempObject = pInspector->GetTargetObject();

                    if (nullptr == pTempObject->FrustumSphere())
                    {
                        pTempObject->AddComponent(new CFrustumSphere);
                    }
                }

                //???????? ???????? ???? 
                if (ImGui::BeginMenu("Script"))
                {
                    vector<wstring> vecScriptInfo;
                    CScriptMgr::GetScriptInfo(vecScriptInfo);

                    for (int i = 0; i < vecScriptInfo.size(); ++i)
                    {
                        char* strScriptName = GetString(vecScriptInfo[i]);

                        if (ImGui::MenuItem(strScriptName))
                        {
                            CScript* pScript = CScriptMgr::GetScript(vecScriptInfo[i]);

                            InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
                            CGameObject* pObject = pInspector->GetTargetObject();

                            pObject->AddComponent(pScript);
                        }
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }            

            ImGui::Separator();

            ImGui::EndMenu();
        }

        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

        if (ImGui::BeginMenu("Scene Mode"))
        {
            if (pCurScene->GetState() != SCENE_STATE::PLAY)
            {
                // Play ?????? ??????.
                if (ImGui::MenuItem("Play"))
                {
                    ChangeSceneState(pCurScene, SCENE_STATE::PLAY);                   
                }
            }
            else
            {
                // Pause ?????? ??????.
                if (ImGui::MenuItem("Pause"))
                {
                    ChangeSceneState(pCurScene, SCENE_STATE::PAUSE);                   
                }
            }

            // ?????????? ????????	
            if (ImGui::MenuItem("Stop"))
            {
                ChangeSceneState(pCurScene, SCENE_STATE::STOP);              
            }

            ImGui::EndMenu();
        }
       
        if (ImGui::BeginMenu("Tool Window"))
        {
            if (ImGui::MenuItem("MRT Window"))
            {
                MRTGUI* pMRTGUI = (MRTGUI*)CImGUIMgr::GetInst()->FindGUI(L"MRT");
                pMRTGUI->Activate();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Map Collider"))
        {
            if (ImGui::BeginMenu("Map Change Collider Create"))
            {
                static int ColliderCount = 0;
                static int LayerIdx = (UINT)LAYER_TYPE::SCENE_CHANGE_COL;

                //ImGui::Text("Setting Layer \t");
                //ImGui::SameLine(200);
                //ImGui::InputInt("##Layer Setting", &LayerIdx);

                ImGui::Text("Setting Collider Count \t");
                ImGui::SameLine(200);
                ImGui::InputInt("##ColliderCount", &ColliderCount);

                if (ImGui::Button("Confirm"))
                {
                    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

                    for (int i = 0; i < ColliderCount; ++i)
                    {
                        CGameObject* pGameObject = new CGameObject;
                        wchar_t Name[256] = { 0, };
                        swprintf_s(Name, L"MapCol_%02d", i);

                        wstring wstr = Name;

                        pGameObject->SetName(wstr);
                        pGameObject->AddComponent(new CTransform);
                        pGameObject->AddComponent(new CMeshRender);
                        pGameObject->AddComponent(new CCollider3D);
                        pGameObject->AddComponent(new CMapChange);

                        pGameObject->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
                        pGameObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
                        pGameObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

                        tEvent even = {};

                        even.eEvent = EVENT_TYPE::CREATE_OBJECT;
                        even.lParam = (DWORD_PTR)pGameObject;
                        even.wParam = LayerIdx;

                        CEventMgr::GetInst()->AddEvent(even);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }
}

CGameObject* MenuGUI::CreateParentObj(wstring _wName, FBXLOAD_TYPE _Type ,int _LayerIdx)
{
    CGameObject* pParentObj = nullptr;

    // MeshData ?? ?????? ???????? ???? ???????? ???? ?????????? ???????? 

    //?????????? ?????? ?? ???? ???? ???????????? ???????? ???????????? ?????? ?????? 
    //???????????? ???????? ???????? ?????? ???????? 
    if (FBXLOAD_TYPE::ANIMATION_LOAD == _Type)
    {      
        pParentObj = new CGameObject;
        pParentObj->SetName(_wName);
        pParentObj->AddComponent(new CTransform);
        pParentObj->AddComponent(new CAnimator3D);
        pParentObj->Animator3D()->SetParent();

    }

    //?????????? ?????? ???????? ???? ?????????? ???????? 
    else
    {  
        pParentObj = new CGameObject;
        pParentObj->SetName(_wName);
        pParentObj->AddComponent(new CTransform);   
    }

    tEvent even = {};

    even.eEvent = EVENT_TYPE::CREATE_OBJECT;
    even.lParam = (DWORD_PTR)pParentObj;
    even.wParam = _LayerIdx;

    CEventMgr::GetInst()->AddEvent(even);

        return pParentObj;  
}

void MenuGUI::ChangeSceneState(CScene* _pScene, SCENE_STATE _eState)
{
    // ???? -> ??????
    if (SCENE_STATE::STOP == _pScene->GetState() && SCENE_STATE::PLAY == _eState)
    {
        CSaveLoadMgr::SaveScene(_pScene, L"scene\\temp.scene");

        _pScene->awake();
        _pScene->start();
    }

    else if (_pScene->GetState() != SCENE_STATE::STOP && _eState == SCENE_STATE::STOP)
    {
        _pScene = new CScene;

        //???? ???? ???? ?????????????? ???????????? . 
        //?????????? ???????? ???????? ?????? ???????? ???????? . 
        CResMgr::GetInst()->GetNavMeshVec().clear();

        CSaveLoadMgr::LoadScene(_pScene, L"scene\\temp.scene");
        CSceneMgr::GetInst()->ChangeScene(_pScene);

        // InspeactorGUI ?? ???? ???????? ??????
        InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
        pInspector->SetTargetObject(nullptr);
    }
    _pScene->SetState(_eState);
}

#include "pch.h"
#include "TransformGUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CSceneMgr.h>
#include <Engine\CTransform.h>

#include <Engine\CKeyMgr.h>
#include <Engine/CCore.h>
#include <Engine/CScene.h>


#define GIZMO_TANSLATE 0
#define GIZMO_SCALE 1
#define GIZMO_ROTATE 2

TransformGUI::TransformGUI()
    : ComponentGUI(COMPONENT_TYPE::TRANSFORM)
    , m_arrData{}
{
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::init()
{

}

void TransformGUI::lateupdate()
{
    if (IsActive() && KEY_TAP(KEY_TYPE::ENTER))
    {
        ImGui::SetWindowFocus(nullptr); // 모든 ImGui window focus 해제
    }
}

void TransformGUI::render()
{
#pragma region Origin



   // CTransform* pTransform = GetTargetObj()->Transform();
   //
   // float arrData[3][3] = {};
   // Vec3 vData[3] = { pTransform->GetLocalPos(), pTransform->GetLocalScale(), pTransform->GetLocalRot() };
   //
   // for (int i = 0; i < 3; ++i)
   // {
   //     for (int j = 0; j < 3; ++j)
   //     {
   //         arrData[i][j] = vData[i][j];
   //     }
   // }
   //
   // arrData[2][0] *= 180.f / XM_PI;
   // arrData[2][1] *= 180.f / XM_PI;
   // arrData[2][2] *= 180.f / XM_PI;
   //
   // Start();
   // ImGui::Text("Position\t");  ImGui::SameLine(); ImGui::DragFloat3("##Position", arrData[0]);
   // ImGui::Text("Scale   \t");  ImGui::SameLine(); ImGui::DragFloat3("##Scale", arrData[1]);
   // ImGui::Text("Rotation\t");  ImGui::SameLine(); ImGui::DragFloat3("##Rotation", arrData[2]);
   // End();
   //
   // arrData[2][0] /= 180.f / XM_PI;
   // arrData[2][1] /= 180.f / XM_PI;
   // arrData[2][2] /= 180.f / XM_PI;
   //
   // pTransform->SetLocalPos(Vec3(arrData[0]));
   // pTransform->SetLocalScale(Vec3(arrData[1]));
   // pTransform->SetLocalRot(Vec3(arrData[2]));
#pragma endregion

    CTransform* Transform = GetTargetObj()->Transform();

    float ArrData[3][3] = {};
    Vec3 Data[3] = { Transform->GetLocalPos(),Transform->GetLocalScale(),Transform->GetLocalRot() };

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ArrData[i][j] = Data[i][j];
        }
    }

    Start();
    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
    static bool InputModeOnOff = false;
    //   if (SCENE_STATE::PLAY != CurScene->GetState())
    {

        static int Mode = GIZMO_TANSLATE;
        ImGui::RadioButton("Translate", &Mode, GIZMO_TANSLATE);
        ImGui::SameLine();
        ImGui::RadioButton("Scale", &Mode, GIZMO_SCALE);
        ImGui::SameLine();
        ImGui::RadioButton("Rotate", &Mode, GIZMO_ROTATE);
        ImGui::SameLine();
        ImGui::Checkbox("Input", &InputModeOnOff);

        DrawGizmo(Mode);
    }

    ImGui::Text("Position"); ImGui::SameLine(100);	ImGui::InputFloat3("##Position", ArrData[0], "%.2f", ImGuiSliderFlags_None);
    ImGui::Text("Scale"); ImGui::SameLine(100); ImGui::InputFloat3("##Scale", ArrData[1], "%.2f", ImGuiSliderFlags_None);
    ImGui::Text("Rotation"); ImGui::SameLine(100); ImGui::InputFloat3("##Rotation", ArrData[2], "%.2f", ImGuiSliderFlags_None);

    if (true == InputModeOnOff)
    {
        Transform->SetLocalPos(Vec3(ArrData[0]));
        Transform->SetLocalScale(Vec3(ArrData[1]));
        Transform->SetLocalRot(Vec3(ArrData[2]));
    }

    End();

}

void TransformGUI::DrawGizmo(int _Mode)
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;



    ImGui::GetWindowPos();
    RECT rt = {};
    GetWindowRect(CCore::GetInst()->GetWndHwnd(), &rt);
    ImVec2 Temp = ImVec2{ (float)rt.left + 8.f,(float)rt.top + 49.0f };
    ImVec2 Size = ImVec2{ (float)rt.right - (float)rt.left - 16.0f,(float)rt.bottom - (float)rt.top - 57.0f };
    ImGui::SetNextWindowPos(Temp);
    ImGui::SetNextWindowSize(Size);
    ImGui::Begin("Gizmo", 0, window_flags);
    ImDrawList* List = ImGui::GetOverlayDrawList();
    ImGuizmo::SetDrawlist(List);
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    float ViewMat[16] = {
    g_transform.matView._11, g_transform.matView._12, g_transform.matView._13, g_transform.matView._14,
    g_transform.matView._21, g_transform.matView._22, g_transform.matView._23, g_transform.matView._24,
    g_transform.matView._31, g_transform.matView._32, g_transform.matView._33, g_transform.matView._34,
    g_transform.matView._41, g_transform.matView._42, g_transform.matView._43, g_transform.matView._44 };

    float ViewProj[16] = {
    g_transform.matProj._11, g_transform.matProj._12, g_transform.matProj._13,g_transform.matProj._14,
    g_transform.matProj._21, g_transform.matProj._22, g_transform.matProj._23,g_transform.matProj._24,
    g_transform.matProj._31, g_transform.matProj._32, g_transform.matProj._33,g_transform.matProj._34,
    g_transform.matProj._41, g_transform.matProj._42, g_transform.matProj._43,g_transform.matProj._44 };

    Matrix ObjMat = GetTargetObj()->Transform()->GetWorldMat();

    float objectMatrix[16] = {
   ObjMat._11,ObjMat._12,ObjMat._13, ObjMat._14,
   ObjMat._21,ObjMat._22,ObjMat._23, ObjMat._24,
   ObjMat._31,ObjMat._32,ObjMat._33, ObjMat._34,
   ObjMat._41,ObjMat._42,ObjMat._43, ObjMat._44 };



    static ImGuizmo::OPERATION OP = {};

    if (GIZMO_TANSLATE == _Mode)
        OP = ImGuizmo::OPERATION::TRANSLATE;
    else if (GIZMO_SCALE == _Mode)
        OP = ImGuizmo::OPERATION::SCALE;
    else if (GIZMO_ROTATE == _Mode)
        OP = ImGuizmo::OPERATION::ROTATE;

    ImGuizmo::Manipulate(ViewMat, ViewProj, OP, ImGuizmo::MODE::WORLD, objectMatrix);
    ImGuizmo::ViewManipulate(ViewMat, 100000.0f, ImVec2((float)rt.right - 144.0f, (float)rt.bottom - 185.0f), ImVec2(128, 128), 0x10101010);

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(objectMatrix, matrixTranslation, matrixRotation, matrixScale);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, objectMatrix);

    ImGui::End();


    CTransform* Transform = GetTargetObj()->Transform();


    if (GIZMO_TANSLATE == _Mode)
        Transform->SetLocalPos(Vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
    else if (GIZMO_SCALE == _Mode)
        Transform->SetLocalScale(Vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
    else if (GIZMO_ROTATE == _Mode)
        Transform->SetLocalRot(Vec3(matrixRotation[0] / 180.f * XM_PI, matrixRotation[1] / 180.f * XM_PI, matrixRotation[2] / 180.f * XM_PI));
}

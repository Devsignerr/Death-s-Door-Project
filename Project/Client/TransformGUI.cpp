#include "pch.h"
#include "TransformGUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CSceneMgr.h>
#include <Engine\CTransform.h>

#include <Engine\CKeyMgr.h>

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
    CTransform* pTransform = GetTargetObj()->Transform();

    float arrData[3][3] = {};
    Vec3 vData[3] = { pTransform->GetLocalPos(), pTransform->GetLocalScale(), pTransform->GetLocalRot() };

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            arrData[i][j] = vData[i][j];
        }
    }

    arrData[2][0] *= 180.f / XM_PI;
    arrData[2][1] *= 180.f / XM_PI;
    arrData[2][2] *= 180.f / XM_PI;

    Start();
    ImGui::Text("Position\t");  ImGui::SameLine(); ImGui::DragFloat3("##Position", arrData[0]);
    ImGui::Text("Scale   \t");  ImGui::SameLine(); ImGui::DragFloat3("##Scale", arrData[1]);
    ImGui::Text("Rotation\t");  ImGui::SameLine(); ImGui::DragFloat3("##Rotation", arrData[2]);
    End();

    arrData[2][0] /= 180.f / XM_PI;
    arrData[2][1] /= 180.f / XM_PI;
    arrData[2][2] /= 180.f / XM_PI;

    pTransform->SetLocalPos(Vec3(arrData[0]));
    pTransform->SetLocalScale(Vec3(arrData[1]));
    pTransform->SetLocalRot(Vec3(arrData[2]));
}
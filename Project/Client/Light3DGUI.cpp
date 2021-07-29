#include "pch.h"
#include "Light3DGUI.h"
#include <Engine/CLight3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTransform.h>

Light3DGUI::Light3DGUI() :
    ComponentGUI(COMPONENT_TYPE::LIGHT3D),
    m_iLight3DIdx(0)
{
}

Light3DGUI::~Light3DGUI()
{
}

void Light3DGUI::finalupdate()
{

}

void Light3DGUI::render()
{
    const tLightInfo& info = GetTargetObj()->Light3D()->GetInfo();

    CTransform* pTransform = GetTargetObj()->Transform();

    tLightInfo temp = {};

    memcpy(&temp, &info, sizeof(tLightInfo));


    const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
    int item_current = int(temp.eType);

    ImGuiColorEditFlags misc_flags = 0;


    float vecDir[3] = { temp.vDir.x,temp.vDir.y, temp.vDir.z};

    temp.fAngle *= 180.f/XM_PI ;

    Start();

    ImGui::Text("colorAmb   \t");  ImGui::SameLine(); ImGui::ColorEdit3("##vAmb", temp.color.vAmb, misc_flags);
    ImGui::Text("colorDiff  \t");  ImGui::SameLine(); ImGui::ColorEdit3("##vDiff", temp.color.vDiff, misc_flags);
    ImGui::Text("colorSpec  \t");  ImGui::SameLine(); ImGui::ColorEdit3("##vSpec", temp.color.vSpec, misc_flags);

    ImGui::Text("fRange     \t");  ImGui::SameLine(); ImGui::DragFloat("##fRange", &temp.fRange);
    ImGui::Text("fAngle     \t");  ImGui::SameLine(); ImGui::DragFloat("##fAngle", &temp.fAngle,0.1f,0.f,360.f);

    ImGui::Text("eType      \t");  ImGui::SameLine(); ImGui::Combo("##eType", &item_current, items, IM_ARRAYSIZE(items));

    End();

    GetTargetObj()->Light3D()->SetAmbPow(temp.color.vAmb);
    GetTargetObj()->Light3D()->SetDiffusePow(temp.color.vDiff);
    GetTargetObj()->Light3D()->SetSpecPow(temp.color.vSpec);

    if (m_iLight3DIdx != item_current)
    {
        m_iLight3DIdx = item_current;
        GetTargetObj()->Light3D()->SetLightType((LIGHT_TYPE)m_iLight3DIdx);
        ImGui::SetWindowFocus(nullptr); // 모든 ImGui window focus 해제
    }

    temp.fAngle /= 180.f / XM_PI;

    GetTargetObj()->Light3D()->SetRange(temp.fRange);
    GetTargetObj()->Light3D()->SetAngle(temp.fAngle);
}

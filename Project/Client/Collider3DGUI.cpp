#include "pch.h"
#include "Collider3DGUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCollider3D.h>

void Collider3DGUI::lateupdate()
{
}

void Collider3DGUI::render()
{
    CCollider3D* pCollider = GetTargetObj()->Collider3D();

    Start();
    const char* items[] = { "CUBE", "SPHERE","POINT" };

    COLLIDER3D_TYPE eType = pCollider->GetCollider3DType();
    int item_current = int(eType);

    ImGui::Text("Collider Type  "); ImGui::SameLine();
    ImGui::Combo("##Collider3D Type", &item_current, items, IM_ARRAYSIZE(items));

    End();

    if (m_iColliderTypeIdx != item_current)
    {
        m_iColliderTypeIdx = item_current;
        pCollider->SetCollider3DType((COLLIDER3D_TYPE)m_iColliderTypeIdx);
        ImGui::SetWindowFocus(nullptr); // 모든 ImGui window focus 해제
    }

   static bool Enable = false;
  
   if (m_bItemChanged)
   {
       Enable = GetTargetObj()->Collider3D()->IsEnable();
       m_bItemChanged = false;
   }
  
   ImGui::Checkbox("Enable", &Enable);
  
   if (nullptr != GetTargetObj())
        GetTargetObj()->Collider3D()->Activate(Enable);
}

Collider3DGUI::Collider3DGUI()
    : ComponentGUI(COMPONENT_TYPE::COLLIDER3D)
{
}

Collider3DGUI::~Collider3DGUI()
{
}

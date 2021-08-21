#include "pch.h"
#include "FrustumGUI.h"
#include <Engine/CFrustumSphere.h>

FrustumGUI::FrustumGUI()
	:ComponentGUI(COMPONENT_TYPE::FRUSTUMSPHERE)
{
}

FrustumGUI::~FrustumGUI()
{
}

void FrustumGUI::update()
{
	ComponentGUI::update();
}

void FrustumGUI::render()
{
	Start();

	Vec3 OffsetPos =GetTargetObj()->FrustumSphere()->GetOffSetPos();
	float Radius =GetTargetObj()->FrustumSphere()->GetRadius();

	float fOffsetPos[3] = { OffsetPos.x,OffsetPos.y,OffsetPos.z };

	ImGui::Text("OffsetPos"); ImGui::SameLine(100);	ImGui::DragFloat3("##wPosition", fOffsetPos);
	ImGui::Text("Radius"); ImGui::SameLine(100);	ImGui::DragFloat("##wPosition", &Radius);

	OffsetPos = Vec3(fOffsetPos[0], fOffsetPos[1], fOffsetPos[2]);

	GetTargetObj()->FrustumSphere()->SetOffsetPos(OffsetPos);
	GetTargetObj()->FrustumSphere()->SetRadius(Radius);

	End();
}

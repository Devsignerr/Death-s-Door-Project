#include "pch.h"
#include "CameraGUI.h"

#include <Engine/CCamera.h>
#include <Script/CCameraScript.h>
#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>



CameraGUI::CameraGUI() :
	ComponentGUI(COMPONENT_TYPE::CAMERA)
{
}

CameraGUI::~CameraGUI()
{
}

void CameraGUI::update()
{
	ComponentGUI::update();

	if (!IsActive())
		return;
}

void CameraGUI::render()
{
	Start();

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pCameraObj = pScene->FindParentObj(L"Camera Object", 0);
	
	CCameraScript* pCameraScript = (CCameraScript*)pCameraObj->GetScript();
	CCamera* pCamera = pCameraScript->Camera();
	
	if (nullptr == pCamera)
		assert(0);
	
	ImGui::Text("Camera Scale");
	
	static Vec2 CameraScale = pCamera->GetScale();
	
	ImGui::InputFloat("##Setting CameraXScale", &CameraScale.x);
	ImGui::SameLine();
	ImGui::InputFloat("##Setting CameraYScale", &CameraScale.y);
	
	pCamera->SetScale(CameraScale);
	
	ImGui::Text("View Type");
	
	
	if (ImGui::Button("Orthographic"))
	{
		pCamera->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
	
	if (ImGui::Button("Perspective"))
	{
		pCamera->SetProjType(PROJ_TYPE::PERSPECTIVE);
	}
	
	End();
}


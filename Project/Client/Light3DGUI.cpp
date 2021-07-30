#include "pch.h"
#include "Light3DGUI.h"
#include <Engine/CLight3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CTransform.h>

#include <Engine/CPathMgr.h>
#include <Engine/CCore.h>
#include <Engine/CResMgr.h>

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



    GetTargetObj()->Light3D()->SetAmbPow(temp.color.vAmb);
    GetTargetObj()->Light3D()->SetDiffusePow(temp.color.vDiff);
    GetTargetObj()->Light3D()->SetSpecPow(temp.color.vSpec);

    if (m_iLight3DIdx != item_current)
    {
        m_iLight3DIdx = item_current;
        GetTargetObj()->Light3D()->SetLightType((LIGHT_TYPE)m_iLight3DIdx);
        ImGui::SetWindowFocus(nullptr); // ��� ImGui window focus ����
    }

    temp.fAngle /= 180.f / XM_PI;

    GetTargetObj()->Light3D()->SetRange(temp.fRange);
    GetTargetObj()->Light3D()->SetAngle(temp.fAngle);


	CLight3D* Light3D = GetTargetObj()->Light3D();

	Ptr<CTexture> ShadowTex = Light3D->GetStaticShadowTex();
	std::wstring TempShadowTexName = {};
	if (nullptr == ShadowTex)
		TempShadowTexName = L"Empty";
	else
		TempShadowTexName = ShadowTex->GetRelativePath();

	std::string ShadowTexName = GetString(TempShadowTexName);

	char szBuffer[255] = "";
	strcpy_s(szBuffer, 255, ShadowTexName.c_str());
	std::string LabelName = "##";
	LabelName += ShadowTexName;
	ImGui::Text("Cur ShadowMapTex");
	ImGui::PushItemWidth(150);
	ImGui::SameLine();
	ImGui::InputText(LabelName.c_str(), szBuffer, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("Select##SelShadowMapTex"))
	{
		LoadShadowMap();
	}

	BakingShadowMap();

	End();

}

void Light3DGUI::BakingShadowMap()
{
	Ptr<CTexture> ShadowMapTex = CResMgr::GetInst()->FindDataTexture(L"ShadowMapTargetTex");

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec2 teszie = ImVec2(200.f, 200.f);
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
	ImGui::Image(ShadowMapTex->GetSRV().Get(), teszie, uv_min, uv_max, tint_col, border_col);

	ImGui::NewLine();
	ImGui::Text("Create ShadowMap");

	ImGui::PushItemWidth(140);
	static char ShadowMapName[256] = {};
	ImGui::InputText("##ShadowMapName", ShadowMapName, IM_ARRAYSIZE(ShadowMapName));
	ImGui::SameLine();

	if (ImGui::Button("Baking"))
	{
		if ('\0' != ShadowMapName[0])
		{
			std::string TempPath = "texture\\ShadowMap\\";
			TempPath += ShadowMapName;

			std::wstring Path = std::wstring(TempPath.begin(), TempPath.end());
			ShadowMapTex->SetUsageShadowMap(true);
			ShadowMapTex->Save(L"texture\\ShadowMap\\ShadowMap");
		}
	}
}

void Light3DGUI::LoadShadowMap()
{
	std::wstring Path = CPathMgr::GetResPath();
	Path += L"TEXTURE\\SHADOWMAP\\"; //

	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
	ofn.lpstrFilter = L"(*.*)";
	ofn.lpstrFile = fileName;
	ofn.lpstrInitialDir = Path.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = nullptr;

	std::wstring fileNameStr = {};

	std::wstring SaveFileName = {};
	if (GetOpenFileName(&ofn))
		SaveFileName = ofn.lpstrFile;

	fileNameStr = L"TEXTURE\\SHADOWMAP\\";
	SaveFileName = CPathMgr::GetFileName(SaveFileName);
	fileNameStr += SaveFileName;
	fileNameStr += L".dds";
	Ptr<CTexture> ShadowMapTex = CResMgr::GetInst()->Load<CTexture>(SaveFileName, fileNameStr);


	GetTargetObj()->Light3D()->SetStaticShadowTex(ShadowMapTex);
}


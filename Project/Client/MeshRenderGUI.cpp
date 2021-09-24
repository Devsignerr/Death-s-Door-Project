#include "pch.h"
#include "MeshRenderGUI.h"

#include "ListGUI.h"
#include "CImGUIMgr.h"

#include <Engine\CMeshRender.h>

#include <Engine\CResMgr.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>
#include <Engine\CCore.h>

MeshRenderGUI::MeshRenderGUI()
	: ComponentGUI(COMPONENT_TYPE::MESHRENDER),
	m_iMaterialIdx(0)
{

}

MeshRenderGUI::~MeshRenderGUI()
{
}

void MeshRenderGUI::update()
{
	ComponentGUI::update();

	if (!IsActive())
		return;
}

void MeshRenderGUI::render()
{
	CMeshRender* pMeshRender = GetTargetObj()->MeshRender();
	
	Start();
	
	wstring wstrName = pMeshRender->GetMesh()->GetKey();
	string strName = string(wstrName.begin(), wstrName.end());
		
	char szBuffer[100] = "";
	strcpy_s(szBuffer, 100, strName.c_str());

	ImGui::PushItemWidth(100);
	ImGui::Text("Mesh\t");
	ImGui::SameLine();  
	ImGui::PushItemWidth(200);
	ImGui::InputText("##MeshName", szBuffer, 100, ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BeginDrag"))
		{
			// �����
			int a = 0;
		}
		ImGui::EndDragDropTarget();
	}
	 
	ImGui::SameLine();

	if (ImGui::Button("Select##1"))
	{
		ListGUI* pGUI = (ListGUI*)PopupListGUI();
		
		pGUI->SetCaption("Mesh");
		
		const map<wstring, CRes*>& mapMesh = CResMgr::GetInst()->GetRes(RES_TYPE::MESH);
		map<wstring, CRes*>::const_iterator iter = mapMesh.begin();
		for (; iter != mapMesh.end(); ++iter)
		{
			pGUI->AddItem(string(iter->first.begin(), iter->first.end()));
		}		
		pGUI->AddItem("empty");
	}	


	UINT MaterialCount = pMeshRender->GetMesh()->GetSubsetCount();
	int iMtrlCount = pMeshRender->GetMtrlCount();
	int item_current = m_iMaterialIdx;
	vector<string> vecMtrl;
	const char* strMtrl[20];

	vecMtrl.resize(20);

	for (int i = 0; i < 20; ++i)
	{
		if (i < iMtrlCount)
		{
			if (nullptr != pMeshRender->GetSharedMaterial(i))
			{
				wstrName = pMeshRender->GetSharedMaterial(i)->GetKey();
				vecMtrl[i] = string(wstrName.begin(), wstrName.end());
				strMtrl[i] = vecMtrl[i].c_str();
			}
		}
		else
		{
			vecMtrl[i] = string("Empty");
			strMtrl[i] = vecMtrl[i].c_str();
		}
	}

	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::PushItemWidth(200);

	if (nullptr == pMeshRender->GetSharedMaterial(0))
	{
		strMtrl[0] = "Empty";
	}

	ImGui::Combo("##MtrlName", &item_current, strMtrl, ARRAYSIZE(strMtrl));
	ImGui::SameLine();


	if (ImGui::Button("Select##2"))
	{
		ListGUI* pGUI = (ListGUI*)PopupListGUI();

		pGUI->SetCaption("Material");
		
		const map<wstring, CRes*>& mapMaterial = CResMgr::GetInst()->GetRes(RES_TYPE::MATERIAL);
		map<wstring, CRes*>::const_iterator iter = mapMaterial.begin();
		for (; iter != mapMaterial.end(); ++iter)
		{
			pGUI->AddItem(string(iter->first.begin(), iter->first.end()));
		}
		pGUI->AddItem("empty");
	}	
	
	m_iMaterialIdx = item_current;


	//=======================MeshRender On Off =============

	static bool Enable = false;
	
	if (m_bItemChanged)
	{
		Enable = GetTargetObj()->MeshRender()->IsEnable();
		m_bItemChanged = false;
	}
	
	ImGui::Checkbox("Enable", &Enable);
	
	if(nullptr!=GetTargetObj())
		GetTargetObj()->MeshRender()->Activate(Enable);
		
	
	End();
}

GUI* MeshRenderGUI::PopupListGUI()
{
	ListGUI* pListGUI = (ListGUI*)CImGUIMgr::GetInst()->FindGUI(L"ListGUI");
	pListGUI->SetSelectCallBack(this, (LIST_SELECT_ITEM)&MeshRenderGUI::SelectListItem);
	pListGUI->Activate();
	
	return pListGUI;
}

void MeshRenderGUI::SelectListItem(ListGUI* _pListGUI, const char* _pSelectName)
{
	string strSel = _pListGUI->GetSelect();

	CMeshRender* pMeshRender = GetTargetObj()->MeshRender();

	Ptr<CMaterial> Temp = pMeshRender->GetSharedMaterial(m_iMaterialIdx);

	if (_pListGUI->GetCaption() == "Mesh")
	{
		Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strSel.begin(), strSel.end()));
		pMeshRender->SetMesh(pMesh);
		pMeshRender->SetMaterial(Temp, m_iMaterialIdx);
		ImGui::SetWindowFocus(nullptr); // ��� ImGui window focus ����
	}
	else if (_pListGUI->GetCaption() == "Material")
	{
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strSel.begin(), strSel.end()));

		pMeshRender->SetMaterial(pMtrl, m_iMaterialIdx);
		ImGui::SetWindowFocus(nullptr); // ��� ImGui window focus ����
	}
}
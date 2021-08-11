#include "pch.h"
#include "PrefabGUI.h"

#include <Engine\Ptr.h>
#include <Engine\CResMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CGameObject.h>


#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>


PrefabGUI::PrefabGUI()
{
}

PrefabGUI::~PrefabGUI()
{

}

void PrefabGUI::render()
{
	Ptr<CPrefab> pPrefab = (CPrefab*)GetTargetRes();

	Start();

	ImGui::Text("Please Click Button to Instanciate");

	if (ImGui::Button("Instanciate Prefab"))
	{
		wstring PrefabName = pPrefab->GetKey();

		Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(PrefabName);
		if (nullptr == Prefab)
		{
			wstring PrefabPath = L"prefab\\" + PrefabName + L".pref";
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->Load<CPrefab>(PrefabName, PrefabPath);
		}

		CGameObject* pGameObject = Prefab->Instantiate();

		int PrefabCount = 0;
		wstring PrefabNumber = L"0";

		while (nullptr != CSceneMgr::GetInst()->FindObjectByName(PrefabName + PrefabNumber))
		{
			PrefabCount++;
			wchar_t Str[10] = {};
			_itow_s(PrefabCount, Str, 10);
			PrefabNumber = wstring(Str);
		}

		pGameObject->SetName(PrefabName + PrefabNumber);
		pGameObject->awake();
		pGameObject->start();
		tEvent even = {};

		even.eEvent = EVENT_TYPE::CREATE_OBJECT;
		even.lParam = (DWORD_PTR)pGameObject;
		even.wParam = Prefab->GetProtoObj()->GetLayerIndex();

		CEventMgr::GetInst()->AddEvent(even);
	}

	End();

}

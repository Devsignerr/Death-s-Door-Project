#include "pch.h"
#include "CMemoryPoolScript.h"
#include "CPlayerChain.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>


int CMemoryPoolScript::m_iChainCount = 30;

std::queue<CGameObject*> CMemoryPoolScript::m_queueChain = {};

CMemoryPoolScript::CMemoryPoolScript()
    :CScript((UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT;
}

CMemoryPoolScript::~CMemoryPoolScript()
{
	m_queueChain = std::queue<CGameObject*>();
}


void CMemoryPoolScript::CreateChain()
{
	if (m_queueChain.size() >= m_iChainCount)
		return;

	for (int i = 0; i < m_iChainCount; ++i)
	{
		wstring PrefabName = L"PlayerChain2";

		Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(PrefabName);
		if (nullptr == Prefab)
		{
			wstring PrefabPath = L"prefab\\" + PrefabName + L".pref";
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->Load<CPrefab>(PrefabName, PrefabPath);
		}

		CGameObject* pGameObject = Prefab->Instantiate();

		int PrefabCount = i;

		wchar_t Str[10] = {};
		_itow_s(PrefabCount, Str, 10);
		wstring PrefabNumber = wstring(Str);

		pGameObject->SetName(PrefabName + PrefabNumber);

		CPlayerChain* pScript = (CPlayerChain*)pGameObject->GetScript();
		pScript->SetName(L"CPlayerChain");
		m_queueChain.push(pGameObject);
		
		pScript->SetActive(false);
		pGameObject->SetAllMeshrenderActive(false);
		
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT_DONSAVE);

	}
}


CGameObject* CMemoryPoolScript::GetChain()
{
	if (!m_queueChain.empty())
	{
		CGameObject* pObj = m_queueChain.front();
		pObj->SetAllMeshrenderActive(true);

		CPlayerChain* pScript = (CPlayerChain*)pObj->GetScript();
		pScript->SetActive(true);

		m_queueChain.pop();
		return pObj;
	}
	return nullptr;
}

void CMemoryPoolScript::awake()
{
	//CreateChain();
}

void CMemoryPoolScript::update()
{
}


void CMemoryPoolScript::ReturnObj(CGameObject* _Obj)
{
	wstring ScriptName = _Obj->GetScript()->GetName();

	if (ScriptName == L"CPlayerChain")
	{
		//¹à±â ÃÊ±âÈ­ 
		((CPlayerChain*)_Obj->GetScript())->SetEmissive(1.f);
		//¸Þ½¬·»´õ Off
		((CPlayerChain*)_Obj->GetScript())->SetActive(false);

		_Obj->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		_Obj->SetAllMeshrenderActive(false);
		m_queueChain.push(_Obj);
	}
}


void CMemoryPoolScript::SaveToScene(FILE* _pFile)
{
    CScript::SaveToScene(_pFile);
}

void CMemoryPoolScript::LoadFromScene(FILE* _pFile)
{
    CScript::LoadFromScene(_pFile);
}

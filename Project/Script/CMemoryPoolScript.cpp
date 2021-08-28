#include "pch.h"
#include "CMemoryPoolScript.h"
#include "CPlayerChain.h"
#include "CExplosionParticle.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>


int CMemoryPoolScript::m_iChainCount = 30;
int CMemoryPoolScript::m_iExplosionPTC =10;

std::queue<CGameObject*> CMemoryPoolScript::m_queueChain = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueExplosionPTC = {};

CMemoryPoolScript::CMemoryPoolScript()
    :CScript((UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT;
}

CMemoryPoolScript::~CMemoryPoolScript()
{
	m_queueChain = std::queue<CGameObject*>();
	m_queueExplosionPTC = std::queue<CGameObject*>();
}


void CMemoryPoolScript::CreateExplosionPTC()
{
	if (m_queueExplosionPTC.size() >= m_iExplosionPTC)
		return;

	for (int i = 0; i < m_iExplosionPTC; ++i)
	{
		wstring PrefabName = L"ExplosionPTC";

		Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(PrefabName);
		if (nullptr == Prefab)
		{
			wstring PrefabPath = L"prefab\\" + PrefabName + L".pref";
			Ptr<CPrefab> Prefab = CResMgr::GetInst()->Load<CPrefab>(PrefabName, PrefabPath);
		}

		CGameObject* pGameObject = Prefab->Instantiate();
		pGameObject->awake();

		int PrefabCount = i;

		wchar_t Str[10] = {};
		_itow_s(PrefabCount, Str, 10);
		wstring PrefabNumber = wstring(Str);

		pGameObject->SetName(PrefabName + PrefabNumber);

		CExplosionParticle* pScript = (CExplosionParticle*)pGameObject->GetScript();
		pScript->SetName(L"CExplosionParticle");
		m_queueExplosionPTC.push(pGameObject);

		pScript->SetActive(false);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT_DONSAVE);

	}
}

void CMemoryPoolScript::CreateChain()
{
	if (m_queueChain.size() >= m_iChainCount)
		return;

	for (int i = 0; i < m_iChainCount; ++i)
	{
		wstring PrefabName = L"PlayerChain";

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


CGameObject* CMemoryPoolScript::GetExplosionPTC()
{
	if (!m_queueExplosionPTC.empty())
	{
		CGameObject* pObj = m_queueExplosionPTC.front();

		CExplosionParticle* pScript = (CExplosionParticle*)pObj->GetScript();
		pScript->SetActive(true);

		m_queueExplosionPTC.pop();
		return pObj;
	}
	return nullptr;
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
	CreateChain();
	//CreateExplosionPTC();
}

void CMemoryPoolScript::update()
{
}


void CMemoryPoolScript::ReturnObj(CGameObject* _Obj)
{
	wstring ScriptName = _Obj->GetScript()->GetName();

	if (ScriptName == L"CPlayerChain")
	{
		//��� �ʱ�ȭ 
		((CPlayerChain*)_Obj->GetScript())->SetEmissive(1.f);
		//�޽����� Off
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

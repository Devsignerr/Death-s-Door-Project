#include "pch.h"
#include "CMemoryPoolScript.h"
#include "CPlayerChain.h"
#include "CExplosionParticle.h"
#include "CFireDamageParticle.h"
#include "CAttackImpactScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CScene.h>


int CMemoryPoolScript::m_iChainCount = 30;
int CMemoryPoolScript::m_iExplosionPTC =10;
int CMemoryPoolScript::m_iFireDamagePTC = 10;
int CMemoryPoolScript::m_iAttackImpact = 60;


std::queue<CGameObject*> CMemoryPoolScript::m_queueChain = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueExplosionPTC = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueFireDamagePTC = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueAttackImpact = {};


CMemoryPoolScript::CMemoryPoolScript()
    :CScript((UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::MEMORYPOOLSCRIPT;
}

CMemoryPoolScript::~CMemoryPoolScript()
{
	m_queueChain = std::queue<CGameObject*>();
	m_queueExplosionPTC = std::queue<CGameObject*>();
	m_queueFireDamagePTC = std::queue<CGameObject*>();
	m_queueAttackImpact = std::queue<CGameObject*>();
}


void CMemoryPoolScript::CreateAttackImpact()
{
	if (m_queueAttackImpact.size() >= m_iAttackImpact)
		return;

	for (int i = 0; i < m_iAttackImpact; ++i)
	{
		wstring PrefabName = L"AttackImpact";

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

		pGameObject->SetAllMeshrenderActive(false);

		CAttackImpactScript* pScript = (CAttackImpactScript*)pGameObject->GetScript();
		pScript->SetName(L"CAttackImpactScript");
		m_queueAttackImpact.push(pGameObject);

		pScript->SetActive(false);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT);

	}
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

		pGameObject->ParticleSystem()->SetRepeat(false);
		pGameObject->ParticleSystem()->Activate(false);
		CExplosionParticle* pScript = (CExplosionParticle*)pGameObject->GetScript();
		pScript->SetName(L"CExplosionParticle");
		m_queueExplosionPTC.push(pGameObject);

		pScript->SetActive(false);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT);

	}
}

void CMemoryPoolScript::CreateFireDamagePTC()
{
	if (m_queueFireDamagePTC.size() >= m_iFireDamagePTC)
		return;

	for (int i = 0; i < m_iFireDamagePTC; ++i)
	{
		wstring PrefabName = L"FireDamagePTC";

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
		pGameObject->ParticleSystem()->Activate(false);

		CFireDamageParticle* pScript = (CFireDamageParticle*)pGameObject->GetScript();
		pScript->SetName(L"CFireDamageParticleScript");
		m_queueFireDamagePTC.push(pGameObject);

		pScript->SetActive(false);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT);

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
		
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT);

	}
}


CGameObject* CMemoryPoolScript::GetAttackImpact()
{
	if (!m_queueAttackImpact.empty())
	{
		CGameObject* pObj = m_queueAttackImpact.front();

		CAttackImpactScript* pScript = (CAttackImpactScript*)pObj->GetScript();
		pScript->SetActive(true);

		m_queueAttackImpact.pop();
		return pObj;
	}
	return nullptr;
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


CGameObject* CMemoryPoolScript::GetFireDamagePTC()
{
	if (!m_queueFireDamagePTC.empty())
	{
		CGameObject* pObj = m_queueFireDamagePTC.front();

		CFireDamageParticle* pScript = (CFireDamageParticle*)pObj->GetScript();
		pScript->SetActive(true);

		m_queueFireDamagePTC.pop();
		return pObj;
	}
	return nullptr;
}


void CMemoryPoolScript::awake()
{
	CreateChain();
	CreateExplosionPTC();
	CreateFireDamagePTC();
	CreateAttackImpact();
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

	else if (ScriptName == L"CExplosionParticle")
	{
		_Obj->Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		((CExplosionParticle*)_Obj->GetScript())->SetActive(false);
		m_queueExplosionPTC.push(_Obj);
	}

	else if (ScriptName == L"CFireDamageParticleScript")
	{
		_Obj->Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		((CFireDamageParticle*)_Obj->GetScript())->SetActive(false);
		m_queueFireDamagePTC.push(_Obj);
	}

	else if (ScriptName == L"CAttackImpactScript")
	{
		_Obj->Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		((CAttackImpactScript*)_Obj->GetScript())->SetActive(false);
		m_queueAttackImpact.push(_Obj);
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

#include "pch.h"
#include "CMemoryPoolScript.h"
#include "CPlayerChain.h"
#include "CExplosionParticle.h"
#include "CFireDamageParticle.h"
#include "CAttackImpactScript.h"
#include "CCrowBatBullet.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CCollider3D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CScene.h>


int CMemoryPoolScript::m_iChainCount = 30;
int CMemoryPoolScript::m_iExplosionPTC =10;
int CMemoryPoolScript::m_iFireDamagePTC = 10;
int CMemoryPoolScript::m_iAttackImpact = 100;
int CMemoryPoolScript::m_iCrowBullet = 50;


std::queue<CGameObject*> CMemoryPoolScript::m_queueChain = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueExplosionPTC = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueFireDamagePTC = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueAttackImpact = {};
std::queue<CGameObject*> CMemoryPoolScript::m_queueCrowBullet = {};

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
	m_queueCrowBullet = std::queue<CGameObject*>();
}


void CMemoryPoolScript::CreateAttackImpact()
{
	if (m_queueAttackImpact.size() >= m_iAttackImpact)
		return;

	Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"AttackImpactMtrl");
	Ptr<CTexture> Tex = CResMgr::GetInst()->FindRes<CTexture>(L"iridecent");

	mtrl->SetDiffuse(Vec4(10.f, 10.f, 10.f, 10.f));
	mtrl->SetData(SHADER_PARAM::TEX_3, Tex.Get());

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

		pGameObject->MeshRender()->SetMaterial(mtrl, 0);

		pGameObject->SetAllMeshrenderActive(false);
		pGameObject->SetDynamicShadow(false);
		pGameObject->SetFrustumCheck(false);
		
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

		pGameObject->SetDynamicShadow(false);
		pGameObject->SetFrustumCheck(false);

		CExplosionParticle* pScript = (CExplosionParticle*)pGameObject->GetScript();

		pScript->SetExploPTCType(EXPLOSION_PTC_TYPE::PLAYER_BOMB);
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

		pGameObject->SetDynamicShadow(false);
		pGameObject->SetFrustumCheck(false);

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
		pGameObject->SetDynamicShadow(false);
		pGameObject->SetFrustumCheck(false);

		
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::PLAYER_EFFECT);

	}
}

void CMemoryPoolScript::CreateCrowBullet()
{
	if (m_queueCrowBullet.size() >= m_iCrowBullet)
		return;

	for (int i = 0; i < m_iCrowBullet; ++i)
	{
		wstring PrefabName = L"CrowBullet";

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

		pGameObject->SetDynamicShadow(false);
		pGameObject->SetFrustumCheck(false);


		CCrowBatBullet* pScript = (CCrowBatBullet*)pGameObject->GetScript();

		pScript->SetName(L"CCrowBatBullet");
		m_queueCrowBullet.push(pGameObject);

		pScript->SetActive(false);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::BOSS_EFFECT);


		CGameObject* Col = new CGameObject;
		Col->SetName(L"CrowBatBullet_Col");

		Col->AddComponent(new CTransform);
		Col->AddComponent(new CMeshRender);
		Col->AddComponent(new CCollider3D);

		Col->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		Col->Transform()->SetLocalScale(Vec3(80.f, 80.f, 200.f));

		Col->Collider3D()->SetParentOffsetPos(Vec3(0.f, 0.f, 100.f));

		Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		CSceneMgr::GetInst()->GetCurScene()->AddObject(Col, (UINT)LAYER_TYPE::CROWBULLET_COL);	

		Col->SetDynamicShadow(false);
		Col->SetFrustumCheck(false);


		pGameObject->AddChild(Col);

		pGameObject->awake();

		pGameObject->SetAllMeshrenderActive(false);
		pGameObject->SetAllColliderActive(false);

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

CGameObject* CMemoryPoolScript::GetCrowBullet()
{
	if (!m_queueCrowBullet.empty())
	{
		CGameObject* pObj = m_queueCrowBullet.front();

		pObj->SetAllMeshrenderActive(true);
		pObj->SetAllColliderActive(true);

		pObj->SetDynamicShadow(true);

		CCrowBatBullet* pScript = (CCrowBatBullet*)pObj->GetScript();
		pScript->SetActive(true);

		const vector<CGameObject*>& vecChild = pObj->GetChild();

		UINT ChildCount = vecChild.size();

		for (UINT i = 0; i < ChildCount; ++i)
		{
			if (vecChild[i]->MeshRender() && vecChild[i]->Collider3D())
				vecChild[i]->MeshRender()->Activate(false);
		}

		m_queueCrowBullet.pop();

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
	CreateCrowBullet();
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

	else if (ScriptName == L"CCrowBatBullet")
	{
		_Obj->Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		_Obj->SetAllMeshrenderActive(false);
		_Obj->SetAllColliderActive(false);
		((CCrowBatBullet*)_Obj->GetScript())->SetActive(false);
		_Obj->SetDynamicShadow(false);

		m_queueCrowBullet.push(_Obj);
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

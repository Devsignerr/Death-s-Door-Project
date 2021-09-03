#include "pch.h"
#include "CWeaponType.h"

#include <Engine/CUI.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>


void CWeaponType::awake()
{

	static int a = 0;
	if (a == 0)
	{
		Ptr<CTexture> UITex = CResMgr::GetInst()->FindRes<CTexture>(L"Arrow");
		if (nullptr == UITex)
			UITex = CResMgr::GetInst()->Load<CTexture>(L"Arrow", L"texture\\UI\\Arrow.png");

		Ptr<CMaterial> Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIArrowMtrl");


		{
			CGameObject* Obj = new CGameObject;

			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);
			Obj->AddComponent(new CUI);

			Obj->Transform()->SetLocalPos(Vec3(0.0f, 110.0f, 0.f));
			Obj->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));

			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);

			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);

			AddChild(GetObj(), Obj);
			m_WeaponIcon.insert(map<WEAPONTYPE, CGameObject*>::value_type(WEAPONTYPE::ARROW, Obj));

		}	

		{
			UITex = CResMgr::GetInst()->FindRes<CTexture>(L"Icon_Fireball");
			if (nullptr == UITex)
				UITex = CResMgr::GetInst()->Load<CTexture>(L"Icon_Fireball", L"texture\\UI\\Icon_Fireball.png");


			Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIFireMtrl");

			CGameObject* Obj = new CGameObject;
	
			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);
			Obj->AddComponent(new CUI);

			Obj->Transform()->SetLocalPos(Vec3(-110.0f, 0.0f, 0.f));
			Obj->Transform()->SetLocalScale(Vec3(182.f, 181.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));
	
			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);
	
			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());
	
			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);
	
			AddChild(GetObj(), Obj);
			m_WeaponIcon.insert(map<WEAPONTYPE, CGameObject*>::value_type(WEAPONTYPE::FIRE, Obj));
		}
	
	
		{

			UITex = CResMgr::GetInst()->FindRes<CTexture>(L"Icon_Bomb");
			if (nullptr == UITex)
				UITex = CResMgr::GetInst()->Load<CTexture>(L"Icon_Bomb", L"texture\\UI\\Icon_Bomb.png");

			Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIBombMtrl");

			CGameObject* Obj = new CGameObject;
	
			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);
			Obj->AddComponent(new CUI);
	
			Obj->Transform()->SetLocalPos(Vec3(110.0f, 0.0f, 0.f));
			Obj->Transform()->SetLocalScale(Vec3(182.f, 181.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));
	
			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);
	
			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());
	
			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);
	
			AddChild(GetObj(), Obj);
			m_WeaponIcon.insert(map<WEAPONTYPE, CGameObject*>::value_type(WEAPONTYPE::BOMB, Obj));
		}
	
		{
			Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIHookMtrl");

			UITex = CResMgr::GetInst()->FindRes<CTexture>(L"Icon_Hook");
			if (nullptr == UITex)
				UITex = CResMgr::GetInst()->Load<CTexture>(L"Icon_Hook", L"texture\\UI\\Icon_Hook.png");

			CGameObject* Obj = new CGameObject;
	
			Obj->AddComponent(new CTransform);
			Obj->AddComponent(new CMeshRender);
			Obj->AddComponent(new CUI);
	
			Obj->Transform()->SetLocalPos(Vec3(0.0f, -110.0f, 0.f));
			Obj->Transform()->SetLocalScale(Vec3(182.f, 181.f, 1.f));
			//Obj->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, XM_PI / 4.0f));
	
			Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			Obj->MeshRender()->SetMaterial(Mtrl, 0);

			Obj->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, UITex.Get());
	
	
			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::UI);
			AddChild(GetObj(), Obj);
			m_WeaponIcon.insert(map<WEAPONTYPE, CGameObject*>::value_type(WEAPONTYPE::HOOK, Obj));
		}
	}

	++a;
}

void CWeaponType::update()
{
	if (KEY_TAP(KEY_TYPE::NUM_1))
		TypeChange(WEAPONTYPE::ARROW);
	if (KEY_TAP(KEY_TYPE::NUM_2))
		TypeChange(WEAPONTYPE::FIRE);
	if (KEY_TAP(KEY_TYPE::NUM_3))
		TypeChange(WEAPONTYPE::BOMB);
	if (KEY_TAP(KEY_TYPE::NUM_4))
		TypeChange(WEAPONTYPE::HOOK);

	if (true == m_IsChange)
		FucusOnOff(PrevIcon, CurIcon);
}

void CWeaponType::FucusOnOff(CGameObject* _PrevIcon, CGameObject* _CurIcon)
{

	Vec3 PrevIconScale = _PrevIcon->Transform()->GetLocalScale();
	Vec3 PrevIconDestScale = Vec3(182.f, 181.f, 1.0f);

	Vec3 PrevIconScaleDiff = DirectX::XMVectorSubtract(PrevIconScale, PrevIconDestScale);

	Vec3 PrevIconSmoothScale = Vector3::Lerp(PrevIconScale, PrevIconDestScale, 5.0f * fDT);
	PrevIconDestScale.x = PrevIconSmoothScale.x;
	PrevIconDestScale.y = PrevIconSmoothScale.y;

	Vec3 CurIconScale = _CurIcon->Transform()->GetLocalScale();
	Vec3 CurIconDestScale = Vec3(220.0f, 220.0f, 1.0f);

	Vec3 CurIconScaleDiff = DirectX::XMVectorSubtract(CurIconScale, CurIconDestScale);

	Vec3 CurIconSmoothScale = Vector3::Lerp(CurIconScale, CurIconDestScale, 5.0f * fDT);
	CurIconDestScale.x = CurIconSmoothScale.x;
	CurIconDestScale.y = CurIconSmoothScale.y;


	if (PrevIconScaleDiff.x <= 0.1f && PrevIconScaleDiff.y <= 0.1f &&
		CurIconScaleDiff.x <= 0.1f && CurIconScaleDiff.y <= 0.1f)
	{
		m_IsChange = false;
	}
	else
	{
		_PrevIcon->Transform()->SetLocalScale(PrevIconDestScale);
		_CurIcon->Transform()->SetLocalScale(CurIconDestScale);
	}

}

void CWeaponType::TypeChange(WEAPONTYPE _Type)
{
	WEAPONTYPE PrevType = m_CurType;
	m_CurType = _Type;

	if (PrevType != m_CurType)
	{
		map<WEAPONTYPE, CGameObject*>::iterator PrevIconIter = m_WeaponIcon.find(PrevType);
		map<WEAPONTYPE, CGameObject*>::iterator CurIconIter = m_WeaponIcon.find(m_CurType);

		if (m_WeaponIcon.end() != PrevIconIter)
			PrevIcon = PrevIconIter->second;

		if (m_WeaponIcon.end() != CurIconIter)
			CurIcon = CurIconIter->second;

		m_IsChange = true;
	}

}

CWeaponType::CWeaponType()
	: CScript((UINT)SCRIPT_TYPE::WEAPONTYPE)
	, m_CurType(WEAPONTYPE::ARROW)
	, PrevIcon(nullptr)
	, CurIcon(nullptr)
	, m_IsChange(false)
{
}

CWeaponType::~CWeaponType()
{
}

void CWeaponType::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CWeaponType::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}


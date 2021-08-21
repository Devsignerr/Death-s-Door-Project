#include "pch.h"
#include "TestScene.h"

#include <Engine\CScene.h>
#include <Engine\CResMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CMeshRender.h>
#include <Engine\CCamera.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CCollider2D.h>
#include <Engine\CParticleSystem.h>
#include <Engine\CLight2D.h>
#include <Engine\CLight3D.h>
#include <Engine\CTerrain.h>

#include <Engine\CTexture.h>
#include <Engine\cmeshdata.h>
#include <Engine\CPathMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CCopyShaderCS.h>
#include <Engine\CSceneMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CCollider3D.h>
#include <Engine/CAnimator3D.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CSkyBoxScript.h>
#include <Script/CCameraScript.h>

#include<Engine/CLayer.h>

#include "CSaveLoadMgr.h"

void CreateSamplePrefab()
{
	/*CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetLocalScale(Vec3(50.f, 50.f, 1.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));*/

	//Ptr<CPrefab> pPrefab = new CPrefab(pObject);
	//pPrefab->Save(L"prefab\\missile.pref");
	//CResMgr::GetInst()->AddRes<CPrefab>(L"Missile", pPrefab);

	//CResMgr::GetInst()->Load<CPrefab>(L"Missile", L"prefab\\missile.pref");
}


void CreateTestScene()
{
	CreateSamplePrefab();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// Scene Load
	//CSaveLoadMgr::LoadScene(pCurScene, L"scene\\TestScene.scene");
	//return;

	CGameObject* pObj = nullptr;

	// Texture ·Îµù 
	Ptr<CTexture> pSkyBoxTex = CResMgr::GetInst()->Load<CTexture>(L"SkyBox_01", L"texture\\skybox\\Sky02.jpg");
	Ptr<CTexture> pSkyBoxTexArr = CResMgr::GetInst()->Load<CTexture>(L"SkyBox_Cube_Water", L"texture\\skybox\\SkyWater.dds");
	CResMgr::GetInst()->Load<CTexture>(L"SkyBox_Cube_Dawn", L"texture\\skybox\\SkyDawn.dds");
	CResMgr::GetInst()->Load<CTexture>(L"Room_MainHall-mat3-_Emissive", L"texture\\FBXTexture\\Room_MainHall-mat3-_Emissive.png");
	CResMgr::GetInst()->Load<CTexture>(L"Room_Right_Emis", L"texture\\FBXTexture\\Room_Right_Emis.png");

	if(nullptr==CResMgr::GetInst()->FindRes<CTexture>(L"Lantern_Emis"))
		CResMgr::GetInst()->Load<CTexture>(L"Lantern_Emis", L"texture\\FBXTexture\\Lantern_Emis.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"weapon_emis"))
		CResMgr::GetInst()->Load<CTexture>(L"weapon_emis", L"texture\\FBXTexture\\weapon_emis.png");

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl");
	pMtrl->SetData(SHADER_PARAM::TEX_0, pSkyBoxTex.Get());
	pMtrl->SetData(SHADER_PARAM::TEXCUBE_0, pSkyBoxTexArr.Get());

	// =============
	// Camera Object
	// =============
	pObj = new CGameObject;
	pObj->SetName(L"Camera Object");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCamera);
	pObj->AddComponent(new CCameraScript);

	pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pObj->Camera()->SetFar(1000000.f);
	pObj->Camera()->SetLayerAllCheck();

	pCurScene->AddObject(pObj, 0);

	// =============
	// PostEffect
	// =============
	CGameObject* pPostEffect = new CGameObject;
	pPostEffect->SetName(L"PostEffect Object");
	
	pPostEffect->AddComponent(new CTransform);
	pPostEffect->AddComponent(new CMeshRender);
	
	pPostEffect->Transform()->SetLocalPos(Vec3(0.f, 0.f, 100.f));
	pPostEffect->Transform()->SetLocalScale(Vec3(5500.f, 3300.f, 1.f));
	
	pPostEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostEffectMtrl"),0);
	
	pCurScene->AddObject(pPostEffect, 1);


	Ptr<CTexture> ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow50px");
	if(nullptr ==ParticleTex )
		ParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Snow50px", L"texture\\particle\\Snow50px.png");

	// ==================
	// Particle System
	// ==================
	CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle System");

	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem);

	pParticle->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));

	pParticle->ParticleSystem()->SetTexture(ParticleTex);
	pCurScene->AddObject(pParticle, 1);


	//
	// ===============
	// 3D Light Object
	// ===============
	pObj = new CGameObject;
	pObj->SetName(L"Directional Light");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CLight3D);

	pObj->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pObj->Light3D()->SetDiffusePow(Vec3(1.f, 1.f, 1.f));
	pObj->Light3D()->SetSpecPow(Vec3(0.4f, 0.4f, 0.4f));
	pObj->Light3D()->SetAmbPow(Vec3(0.5f, 0.5f, 0.5f));
	pObj->Light3D()->SetRange(1000.f);

	pObj->Transform()->SetLocalPos(Vec3(-500.f, 1000.f, -500.f));
	pObj->Transform()->SetLocalRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));

	pCurScene->AddObject(pObj, 0);


	// =============
	// SkyBox Object
	// =============
	pObj = new CGameObject;
	pObj->SetName(L"SkyBox");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CSkyBoxScript);
	
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"), 0);
	
	((CSkyBoxScript*)pObj->GetScript())->SetBoxType();
	
	pCurScene->AddObject(pObj, 0);

	
	pObj = new CGameObject;
	pObj->SetName(L"Collider Object2");
	
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CActorScript);
	//pObj->AddComponent(new PLAYERSCRIPT);
	//
	pObj->Transform()->SetLocalPos(Vec3(800.f, 200.0f, 0.f));
	pObj->Transform()->SetLocalScale(Vec3(200.0f, 200.0f, 200.0f));
	
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
	pCurScene->AddObject(pObj, 10);
	

	// Collision Check
	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(10, 11);

	// Scene Save
	CSaveLoadMgr::SaveScene(pCurScene, L"scene\\TestScene.scene");
}

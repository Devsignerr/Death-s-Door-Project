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
#include <Engine/CRenderMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CSkyBoxScript.h>
#include <Script/CCameraScript.h>
#include <Script/CMemoryPoolScript.h>
#include <Script/CFadeScript.h>

#include<Engine/CLayer.h>

#include "CSaveLoadMgr.h"
void CreateTestScene()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//Scene Load
	//CSaveLoadMgr::LoadScene(pCurScene, L"scene\\test1234.scene");


	CGameObject* pObj = nullptr;
	// =================
	// UI Camera Object
	// ==================
	//pObj = new CGameObject;
	//pObj->SetName(L"UI Camera Object");
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CCamera);
	//
	//pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pObj->Camera()->SetCameraType(CAMERA_TYPE::UI_CAM);
	//pObj->Camera()->SetFar(1000.f);
	//CRenderMgr::GetInst()->RegisterUICamera(pObj->Camera());
	//
	////playercol - monsterattackcol
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MONSTER_ATTACK_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MONSTER_BULLET_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::BOSS_ATTACK_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::BOSS_BULLET_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MAP_GIMIC_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::WALL_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::WALL_COL);
	//
	//// playerattackcol - monstercol
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::MONSTER_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::BOSS_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::MAP_GIMIC_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::BOSS_BULLET_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::CROWBULLET_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::CROWBULLET_COL);
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::SCENE_CHANGE_COL);
	//
	//CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::BOSS_COL, (UINT)LAYER_TYPE::INDETERMINATE);
	//
	//
	//pCurScene->awake();
	//pCurScene->SetState(SCENE_STATE::PLAY);
	//return;

	//CGameObject* pObj = nullptr;

	// Texture ·Îµù 
	Ptr<CTexture> pSkyBoxTex = CResMgr::GetInst()->Load<CTexture>(L"SkyBox_01", L"texture\\skybox\\Sky02.jpg");
	Ptr<CTexture> pSkyBoxTexArr = CResMgr::GetInst()->Load<CTexture>(L"SkyBox_Cube_Water", L"texture\\skybox\\SkyWater.dds");
	CResMgr::GetInst()->Load<CTexture>(L"SkyBox_Cube_Dawn", L"texture\\skybox\\SkyDawn.dds");
	CResMgr::GetInst()->Load<CTexture>(L"Room_MainHall-mat3-_Emissive", L"texture\\FBXTexture\\Room_MainHall-mat3-_Emissive.png");
	CResMgr::GetInst()->Load<CTexture>(L"Room_Right_Emis", L"texture\\FBXTexture\\Room_Right_Emis.png");

	if(nullptr==CResMgr::GetInst()->FindRes<CTexture>(L"Lantern_Emis"))
		CResMgr::GetInst()->Load<CTexture>(L"Lantern_Emis", L"texture\\FBXTexture\\Lantern_Emis.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"Lantern_Emis2"))
		CResMgr::GetInst()->Load<CTexture>(L"Lantern_Emis2", L"texture\\FBXTexture\\Lantern_Emis2.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"weapon_emis"))
		CResMgr::GetInst()->Load<CTexture>(L"weapon_emis", L"texture\\FBXTexture\\weapon_emis.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"GrayScale2"))
		CResMgr::GetInst()->Load<CTexture>(L"GrayScale2", L"texture\\PaperBurn\\GrayScale2.png");
	
	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"noise"))
		CResMgr::GetInst()->Load<CTexture>(L"noise", L"texture\\PaperBurn\\noise.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"sword_slash_texture 1"))
		CResMgr::GetInst()->Load<CTexture>(L"sword_slash_texture 1", L"texture\\FBXTexture\\sword_slash_texture 1.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"iridecent"))
		CResMgr::GetInst()->Load<CTexture>(L"iridecent", L"texture\\FBXTexture\\iridecent.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"plagueKnightTexture"))
		CResMgr::GetInst()->Load<CTexture>(L"plagueKnightTexture", L"texture\\FBXTexture\\plagueKnightTexture.png");
	
	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"T_WaterParticles"))
		CResMgr::GetInst()->Load<CTexture>(L"T_WaterParticles", L"texture\\FBXTexture\\T_WaterParticles.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"LightningBoltTexture"))
		CResMgr::GetInst()->Load<CTexture>(L"LightningBoltTexture", L"texture\\FBXTexture\\LightningBoltTexture.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"Perlin"))
		CResMgr::GetInst()->Load<CTexture>(L"Perlin", L"texture\\FBXTexture\\Perlin.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"Fiber"))
		CResMgr::GetInst()->Load<CTexture>(L"Fiber", L"texture\\FBXTexture\\Fiber.png");

	if (nullptr == CResMgr::GetInst()->FindRes<CTexture>(L"HardCircle"))
		CResMgr::GetInst()->Load<CTexture>(L"HardCircle", L"texture\\particle\\HardCircle.png");
	
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

	pCurScene->AddObject(pObj, LAYER_TYPE::CAMERA);


	// =================
	// UI Camera Object
	// ==================
	pObj = new CGameObject;
	pObj->SetName(L"UI Camera Object");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCamera);

	pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pObj->Camera()->SetCameraType(CAMERA_TYPE::UI_CAM);
	pObj->Camera()->SetFar(1000.f);
	CRenderMgr::GetInst()->RegisterUICamera(pObj->Camera());



	// =============
	// PostEffect
	// =============
	CGameObject* pPostEffect = new CGameObject;
	pPostEffect->SetName(L"PostEffect Object");
	
	pPostEffect->AddComponent(new CTransform);
	pPostEffect->AddComponent(new CMeshRender);
	pPostEffect->AddComponent(new CFadeScript);
	
	pPostEffect->Transform()->SetLocalPos(Vec3(0.f, 0.f, 100.f));
	pPostEffect->Transform()->SetLocalScale(Vec3(5500.f, 3300.f, 1.f));
	
	pPostEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostEffectMtrl"),0);
	
	pCurScene->AddObject(pPostEffect, LAYER_TYPE::POSTEFFECT);


	Ptr<CTexture> ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(L"fireparticle");
	if(nullptr ==ParticleTex )
		ParticleTex = CResMgr::GetInst()->Load<CTexture>(L"fireparticle", L"texture\\FBXTexture\\fireparticle.png");

	// ==================
	// Particle System
	// ==================
	CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle System");

	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem);

	pParticle->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));

	pParticle->ParticleSystem()->SetTexture(ParticleTex);
	pCurScene->AddObject(pParticle, LAYER_TYPE::PARTICLE);


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

	pCurScene->AddObject(pObj, LAYER_TYPE::LIGHT);


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
	
	pCurScene->AddObject(pObj, LAYER_TYPE::SKYBOX);

	// =================
	// MemoryPool Object
	// =================
	pObj = new CGameObject;
	pObj->SetName(L"MemoryPool");
	
	pObj->AddComponent(new CMemoryPoolScript);
	
	pCurScene->AddObject(pObj, (UINT)LAYER_TYPE::MEMORYPOOL);


	// =============
	// Collider Object
	// =============
	pObj = new CGameObject;
	pObj->SetName(L"Collider17");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider3D);

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	pObj->Transform()->SetLocalScale(Vec3(400.f, 400.f, 400.f));
	pObj->Transform()->SetLocalPos(Vec3(400.f, 0.f, 400.f));

	pCurScene->AddObject(pObj, (UINT)LAYER_TYPE::MONSTER_COL);


	// playercol - monsterattackcol
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MONSTER_ATTACK_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MONSTER_BULLET_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::BOSS_ATTACK_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::BOSS_BULLET_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::MAP_GIMIC_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::WALL_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::WALL_COL);

	// playerattackcol - monstercol
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::MONSTER_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::BOSS_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::MAP_GIMIC_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::BOSS_BULLET_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_ATTACK_COL, (UINT)LAYER_TYPE::CROWBULLET_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::CROWBULLET_COL);
	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_COL, (UINT)LAYER_TYPE::SCENE_CHANGE_COL);

	CCollisionMgr::GetInst()->CollisionCheck((UINT)LAYER_TYPE::BOSS_COL, (UINT)LAYER_TYPE::INDETERMINATE);
	pCurScene->awake();

	// Scene Save
	CSaveLoadMgr::SaveScene(pCurScene, L"scene\\TestScene.scene");
}

void CreatePrefabs()
{



}

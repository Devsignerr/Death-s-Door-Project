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

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CMissileScript.h>
#include <Script\CSkyBoxScript.h>
#include <Script/CCameraScript.h>
#include <Script/CHsh.h>

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
	pObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pObj->Camera()->SetFar(1000000.f);
	pObj->Camera()->SetLayerAllCheck();

	pCurScene->AddObject(pObj, 0);

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

	// Clone
	/*pObj = pObj->Clone();
	pObj->Light3D()->SetDiffusePow(Vec3(1.f, 1.f, 1.f));
	pObj->Transform()->SetLocalPos(Vec3(400.f, 200.f, 500.f));
	pCurScene->AddObject(pObj, 0);*/

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


	// ==============
	// Player Object
	// ==============
	//pObj = new CGameObject;
	//pObj->SetName(L"Plane");
	//pObj->SetDynamicShadow(true);

	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	////pObj->AddComponent(new CPlayerScript);

	//pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pObj->Transform()->SetLocalScale(Vec3(4000.f, 4000.f, 100.f));
	//pObj->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	//pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	////CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl")->SetData(SHADER_PARAM::TEX_0, pTileTex.Get());
	////CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl")->SetData(SHADER_PARAM::TEXCUBE_0, pSkyBoxTexArr.Get());

	//pCurScene->AddObject(pObj, 0);


	// ============
	// FBX Loading
	// ============
	// ========
	// FBX Load
	// ========

	//D3DXIntersectTri();

	//CResMgr::GetInst()->Load<CTexture>(L"bruteTex2", L"texture\\FBXTexture\\bruteGruntTexture_Multi2.png");

	//wstring FileName = { L"Monster" };
	//
	//Ptr<CMeshData> pMeshData = nullptr;
	//
	////CResMgr::GetInst()->LoadFBX(FileName + L".fbx");
	////vector<CMeshData*>& pVecMeshData = CResMgr::GetInst()->GetMeshDataVec();
	//  
	//int idx = 0;	
	//
	//while (true) {
	//
	//	if (nullptr == CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\"+ FileName + std::to_wstring(idx) + L".mdat"))
	//		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\" + FileName + std::to_wstring(idx) + L".mdat", L"meshdata\\" + FileName + std::to_wstring(idx) + L".mdat");
	//	else
	//	{
	//		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\" + FileName + std::to_wstring(idx) + L".mdat");
	//	}
	//
	//	if (pMeshData->IsLoadFail())
	//		break;
	//
	//	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\Monster.mdat", L"meshdata\\Monster.mdat");
	//	CGameObject* pObj = nullptr;
	//
	//	
	//	for (int i = 0; i < 1; i++) {
	//		wstring str = { L"Player Object" };
	//		pObj = pMeshData->Instantiate();
	//		pObj->SetName(str);
	//		pObj->SetDynamicShadow(true);
	//
	//		pCurScene->AddObject(pObj,0);
	//	}
	//	idx++;	
	//}

	//CGameObject* pColObj = nullptr;
	//pColObj = new CGameObject;
	//pColObj->SetName(L"Coll1");
	//pColObj->AddComponent(new CTransform);
	//pColObj->AddComponent(new CCollider2D);
	//pColObj->AddComponent(new CMeshRender);
	//pColObj->Transform()->SetLocalScale(Vec3(10, 10, 10));
	//pCurScene->AddObject(pColObj, 1);
	//
	//
	//pColObj = new CGameObject;
	//pColObj->SetName(L"Coll2");	
	//pColObj->AddComponent(new CTransform);
	//pColObj->AddComponent(new CCollider2D);
	//pColObj->AddComponent(new CMeshRender);
	//pColObj->Transform()->SetLocalScale(Vec3(20, 20, 20));
	//pColObj->Transform()->SetLocalRot(Vec3(1.57, 0, 0));
	//pCurScene->AddObject(pColObj, 2);
	//
	//CCollisionMgr::GetInst()->CollisionCheck(1,2);

	// ===============
	// Terrrain Object
	// ===============
	//pObj = new CGameObject;
	//pObj->SetName(L"Terrain");

	//pObj->AddComponent(new CTransform);	
	//pObj->AddComponent(new CTerrain);
	//
	////Ptr<CTexture> pHeightMap = CResMgr::GetInst()->Load<CTexture>(L"HeightMap_01", L"texture\\HeightMap\\HeightMap_01.jpg");
	////pObj->Terrain()->SetHeightMap(pHeightMap);
	//pObj->Terrain()->CreateHeightMap(2048, 2048);
	//pObj->Transform()->SetLocalPos(Vec3(-9600.f, 10.f, -9600.f));
	//pObj->Transform()->SetLocalScale(Vec3(300.f, 1000.f, 300.f));	
	//pObj->Terrain()->SetFace(64, 64);	

	//pCurScene->AddObject(pObj, 0);



	pObj = new CGameObject;
	pObj->SetName(L"Plane");
	pObj->SetDynamicShadow(true);
	
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	
	pObj->Transform()->SetLocalPos(Vec3(0.f, 0.0f, 0.f));
	pObj->Transform()->SetLocalScale(Vec3(4000.f, 4000.f, 4000.f));
	pObj->Transform()->SetLocalRot(Vec3(DirectX::XM_PI / 2.f, 0.f, 0.f));
	
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	//JRESMGR::GetInst()->FindRes<JMATERIAL>(L"Std3DMaterial")->SetData(SHADER_PARAM::TEX_0, TileTex.Get());
	//JRESMGR::GetInst()->FindRes<JMATERIAL>(L"Std3DMaterial")->SetData(SHADER_PARAM::TEXCUBE_0, SkyBoxTexArr.Get());
	
	pCurScene->AddObject(pObj, 0);

	
	pObj = new CGameObject;
	pObj->SetName(L"Collider Object2");
	
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider3D);
	//pObj->AddComponent(new PLAYERSCRIPT);
	//
	pObj->Transform()->SetLocalPos(Vec3(800.f, 200.0f, 0.f));
	pObj->Transform()->SetLocalScale(Vec3(200.0f, 200.0f, 200.0f));
	
	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
	pCurScene->AddObject(pObj, 10);

	pObj = new CGameObject;
	pObj->SetName(L"Collider Object");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider3D);
	//pObj->AddComponent(new PLAYERSCRIPT);
	//
	pObj->Transform()->SetLocalPos(Vec3(0.f, 200.0f, 0.f));
	pObj->Transform()->SetLocalScale(Vec3(200.0f, 200.0f, 200.0f));

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
	pCurScene->AddObject(pObj, 11);
	
	//
	//pObj = new JGAMEOBJECT;
	//pObj->SetName(L"Collider Object");
	//
	//pObj ->AddComponent(new JTRANSFORM);
	//pObj ->AddComponent(new JMESHRENDER);
	//pObj ->AddComponent(new JCOLLIDER3D);
	//pObj ->AddComponent(new TESTMONSTERSCRIPT);
	//
	//pObj->Transform()->SetLocalPos(Vec3(0.f, 0.0f, 0.f));
	//pObj->Transform()->SetLocalScale(Vec3(200.0f, 200.0f, 200.0f));
	//
	//pObj ->MeshRender()->SetMesh(JRESMGR::GetInst()->FindRes<JMESH>(L"Cube_C3D_Mesh"));
	//pObj ->MeshRender()->SetMaterial(JRESMGR::GetInst()->FindRes<JMATERIAL>(L"Collider3DMaterial"), 0);
	//CurScene->AddObject(pObj, 10);


	// Collision Check
	CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(10, 11);

	// Scene Save
	CSaveLoadMgr::SaveScene(pCurScene, L"scene\\TestScene.scene");
}

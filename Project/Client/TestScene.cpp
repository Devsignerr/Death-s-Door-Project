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
#include <Script/CMemoryPoolScript.h>

#include<Engine/CLayer.h>

#include "CSaveLoadMgr.h"
//
//void CreateMonsterPrefab()
//{
//}
//
//void CreateBossPrefab()
//{
//}
//
//void CreateParticlePrefab()
//{
//}
//
//void CreatePlayerPrefab()
//{
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//
//	Instanciate(L"crow_player", FBXLOAD_TYPE::ANIMATION_LOAD, (UINT)LAYER_TYPE::PLAYER);
//
//	CGameObject* pSwingEffect = new CGameObject;
//	pSwingEffect->SetName(L"Player Prefab");
//	pSwingEffect->AddComponent(new CTransform);
//	pSwingEffect->AddComponent(new CMeshRender);
//	pSwingEffect->AddComponent(new CAnimator2D);
//	pSwingEffect->AddComponent(new CSwingEffectScript);
//
//	pSwingEffect->Transform()->SetLocalScale(Vec3(240.f, 188.f, 1.f));
//
//	pSwingEffect->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
//
//	pSwingEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
//	pSwingEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
//
//	Ptr<CTexture> pSwingTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalSwing");
//
//	if (nullptr == pSwingTex)
//		pSwingTex = CResMgr::GetInst()->Load<CTexture>(L"NormalSwing", L"Texture\\Effect\\Swing.png");
//
//
//	if (nullptr == pSwingEffect->Animator2D()->FindAnimation(L"NORMALSWING"))
//		pSwingEffect->Animator2D()->CreateAnimation(L"NORMALSWING", pSwingTex, Vec2(0.f, 0.f), Vec2(60.f, 47.f), 3, 0.08f);
//
//	pSwingEffect->Animator2D()->PlayAnimation(L"NORMALSWING", false);
//
//
//	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(pSwingEffect->GetName());
//
//	if (nullptr == pPrefab)
//	{
//		int Layer = pSwingEffect->GetLayerIndex();
//
//		CGameObject* ProtoObject = pSwingEffect->Clone();
//		ProtoObject->SetName(pSwingEffect->GetName());
//		ProtoObject->SetLayerIndex((UINT)LAYER_TYPE::PLAYER_EFFECT);
//
//		pPrefab = new CPrefab(ProtoObject);
//		wstring Name = ProtoObject->GetName();
//		Name = L"prefab\\" + Name + L".pref";
//		pPrefab->Save(Name);
//		CResMgr::GetInst()->AddRes<CPrefab>(ProtoObject->GetName(), pPrefab);
//	}
//
//	tEvent Event = {};
//
//	Event.eEvent = EVENT_TYPE::DELETE_OBJECT;
//	Event.lParam = (DWORD_PTR)pSwingEffect;
//
//	CEventMgr::GetInst()->AddEvent(Event);
//
//
//
//}
//
//void CreateProjectilePrefab()
//{
//}
//
//void CreatestructurePrefab()
//{
//}
//
//void Instanciate(wstring _FBXName, FBXLOAD_TYPE _Type, int _LayerIdx)
//{
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//
//	wstring wFBXName = _FBXName;
//	FBXLOAD_TYPE Type = _Type;
//	int LayerIdx = _LayerIdx;
//
//
//	Ptr<CMeshData> pMeshData = nullptr;
//	CGameObject* pParent = CreateParentObj(wFBXName, Type, LayerIdx);
//	int idx = 0;
//
//	//FBX ������ �̸����� meshdata�� ����ؼ� �˻��� �����Ҷ����� ��� �ε��Ų�� .�����ϸ� �ݺ����� Ż���Ѵ� . 
//	while (true) {
//
//		if (nullptr == CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat"))
//			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat", L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat");
//		else
//		{
//			pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\" + wFBXName + std::to_wstring(idx) + L".mdat");
//		}
//
//		if (pMeshData->IsLoadFail())
//			break;
//
//
//		CGameObject* pObj = nullptr;
//
//		//�ڽ��� �θ��� �̸� + child + �ε��������� �̸��� �ٴ´� 
//		//(�޽� �̸����󰡵��� ���濹��)
//		wstring str = { wFBXName + L" Child" + std::to_wstring(idx) };
//		pObj = pMeshData->Instantiate(Type);
//		pObj->SetName(str);
//
//
//		//�׺�޽��� ���� �׸����� ������ �����Ƿ� ���� 
//		if (Type != FBXLOAD_TYPE::NAVMESH_LOAD)
//		{
//			pObj->SetDynamicShadow(true);
//		}
//		else
//		{
//			//���� �����ϴ� ��� �׺�޽� ����
//			vector<CGameObject*>& pVecNavMesh = CResMgr::GetInst()->GetNavMeshVec();
//
//			//�޽��� ResMgr�� �޽�Ÿ�Կ��� ����� ������ Nav�޽� ���Ϳ��� ����� �ȴ� 
//			pVecNavMesh.push_back(pObj);
//
//		}
//
//		//�ڽ� ��ü�� ������ �θ� ���ԵɰŶ� 0�� ���̾ �ִ´� 
//		//(��� �浹ü�� ��Ÿ ������ �ֻ��� �θ� ���ַ� �������Ƿ� ���̾ ũ�� �ǹ̾����ϴ� )
//		pCurScene->AddObject(pObj, 0);
//		pParent->AddChild(pObj);
//
//		idx++;
//	}
//
//	//�θ� ������Ʈ�� MeshData�� Instantiate �Լ��� ���� ��������� �ƴ϶� �޽��� ���� (���� tVecClip�� ����)
//	//����  ù��° �ڽ��� �޽��� ������ �ִ� AnimClip�� �����ؿ� 
//	if (Type == FBXLOAD_TYPE::ANIMATION_LOAD)
//	{
//		Ptr<CMesh> mesh = pParent->GetChild()[0]->MeshRender()->GetMesh();
//
//		pParent->Animator3D()->CopyAnimClip((vector<tMTAnimClip>*)mesh->GetAnimClip());
//	}
//}
//
//CGameObject* CreateParentObj(wstring _wName, FBXLOAD_TYPE _Type, int _LayerIdx)
//{
//	CGameObject* pParentObj = nullptr;
//
//	// MeshData �� ������ �ε��Ҷ� �̸� �����ϴ� �θ� ������Ʈ�� �ٿ��ش� 
//
//	//�ִϸ��̼� �ε带 �� ��� �ڽ� ������Ʈ�鿡 �޷��ִ� �ִϸ����Ϳ� ����� ������ 
//	//�ִϸ����͸� �θ𿡰� �޾��ְ� �θ�� �����Ѵ� 
//	if (FBXLOAD_TYPE::ANIMATION_LOAD == _Type)
//	{
//		pParentObj = new CGameObject;
//		pParentObj->SetName(_wName);
//		pParentObj->AddComponent(new CTransform);
//		pParentObj->AddComponent(new CAnimator3D);
//		pParentObj->Animator3D()->SetParent();
//
//	}
//
//	//�ִϸ��̼� �ε尡 �ƴҰ�� �׳� Ʈ�������� �޾��ش� 
//	else
//	{
//		pParentObj = new CGameObject;
//		pParentObj->SetName(_wName);
//		pParentObj->AddComponent(new CTransform);
//	}
//
//	tEvent even = {};
//
//	even.eEvent = EVENT_TYPE::CREATE_OBJECT;
//	even.lParam = (DWORD_PTR)pParentObj;
//	even.wParam = _LayerIdx;
//
//	CEventMgr::GetInst()->AddEvent(even);
//
//	return pParentObj;
//}

void CreateTestScene()
{
	//CreateMonsterPrefab(); 
	//CreateBossPrefab(); 
	//CreateParticlePrefab();
	//CreatePlayerPrefab();
	//CreateProjectilePrefab();
	//CreatestructurePrefab();


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// Scene Load
	//CSaveLoadMgr::LoadScene(pCurScene, L"scene\\TestScene.scene");
	//return;

	CGameObject* pObj = nullptr;

	// Texture �ε� 
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

	// =================
	// MemoryPool Object
	// =================
	pObj = new CGameObject;
	pObj->SetName(L"MemoryPool");
	
	pObj->AddComponent(new CMemoryPoolScript);
	
	pCurScene->AddObject(pObj, 0);


	// Collision Check
	//CCollisionMgr::GetInst()->CollisionCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionCheck(10, 26);
	CCollisionMgr::GetInst()->CollisionCheck(10, 8);
	CCollisionMgr::GetInst()->CollisionCheck(13, 22);
	CCollisionMgr::GetInst()->CollisionCheck(0, 22);

	CCollisionMgr::GetInst()->CollisionCheck(2, 3);
	CCollisionMgr::GetInst()->CollisionCheck(1, 3);

	pCurScene->awake();

	// Scene Save
	CSaveLoadMgr::SaveScene(pCurScene, L"scene\\TestScene.scene");
}

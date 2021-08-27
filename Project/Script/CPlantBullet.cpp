#include "pch.h"
#include "CPlantBullet.h"
#include <Engine/CLight3D.h>
#include <Engine/CParticleSystem.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void CPlantBullet::update()
{
	Vec3 Pos = Transform()->GetLocalPos();
	
	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.x * 1000.0f;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_BulletDir.z * 1000.0f;
	
	Transform()->SetLocalPos(Pos);
}

void CPlantBullet::awake()
{
	CGameObject* Child = new CGameObject;

	Child->AddComponent(new CTransform);
	Child->AddComponent(new CLight3D);
	Child->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	Child->Light3D()->SetRange(200.f);
	Child->Light3D()->SetAmbPow(Vec3(1.f, 0.f, 0.f));

	Child->AddComponent(new CParticleSystem);

	Ptr<CTexture> ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(L"fireparticle");
	if (nullptr == ParticleTex)
		ParticleTex = CResMgr::GetInst()->Load<CTexture>(L"fireparticle", L"texture\\FBXTexture\\fireparticle.png");

	Child->ParticleSystem()->SetTexture(ParticleTex);
	Child->ParticleSystem()->awake();
	Child->ParticleSystem()->SetStartScale(Vec3(200.f, 200.f, 200.f));
	CSceneMgr::GetInst()->GetCurScene()->AddObject(Child, 0);


	GetObj()->AddChild(Child);
}

CPlantBullet::CPlantBullet()
	: m_BulletDir{}
{
	m_iScriptType = (int)SCRIPT_TYPE::PLANTBULLET;
}

CPlantBullet::~CPlantBullet()
{
}

void CPlantBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlantBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

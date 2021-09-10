#include "pch.h"
#include "TCastleLaser_Ready.h"
#include "CCastleScript.h"
#include "CPlayerScript.h"
#include "CCastleLaser.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCollider3D.h>
#include <Engine/CSceneMgr.h>

void TCastleLaser_Ready::CreateLaser()
{
	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

	if (nullptr == m_Laser)
	{
		m_Laser = new CGameObject;

		m_Laser->SetName(L"Castle Laser");

		m_Laser->AddComponent(new CTransform);
		m_Laser->AddComponent(new CMeshRender);
		m_Laser->AddComponent(new CCastleLaser);

		((CCastleLaser*)m_Laser->GetScript())->SetActive(false);

		m_Laser->Transform()->SetLocalPos(Vec3(-99999.f, -99999.f, -99999.f));
		m_Laser->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		m_Laser->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Low_SphereMesh"));
		m_Laser->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LaserMtrl"), 0);

		Ptr<CTexture> WhiteTex = CResMgr::GetInst()->FindRes<CTexture>(L"WhiteTex");

		if (nullptr == WhiteTex)
			WhiteTex = CResMgr::GetInst()->Load<CTexture>(L"WhiteTex", L"texture\\FBXTexture\\WhiteTex.png");

		m_Laser->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_0, WhiteTex.Get());
		m_Laser->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::TEX_3, WhiteTex.Get());

		m_Laser->MeshRender()->GetSharedMaterial(0)->SetDiffuse(Vec4(1, 1, 1, 1));
		m_Laser->MeshRender()->GetSharedMaterial(0)->SetEmissive(Vec4(0.4, 0.1, 0.1, 1));

		CurScene->AddObject(m_Laser, (UINT)LAYER_TYPE::BOSS_EFFECT);

		m_LaserOrigin->AddChild(m_Laser);

		m_Laser->awake();
	}
}

void TCastleLaser_Ready::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (360 <= CurAni->GetFrameIdx())
	{
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 LaserPointPos = m_LaserPoint->Transform()->GetLocalPos();
		Vec3 Diff = PlayerPos - LaserPointPos;

		Diff.y = 0.f;
		Diff.Normalize();

		LaserPointPos += Diff * fDT * 800.0f;
		m_LaserPoint->Transform()->SetLocalPos(LaserPointPos);

		// 플레이어가 일정 범위 안에 있는 경우 밀어 낸다
		if (m_Script->RangeSearch(1200.0f))
		{
			Vec3 PlayerPos2 = CPlayerScript::GetPlayerPos();
			Vec3 Pos = GetObj()->Transform()->GetLocalPos();

			PlayerPos2.y = 0.0f;
			Pos.y = 0.0f;

			float Distance = Vec3::Distance(PlayerPos2, Pos);
			Distance *= 0.04f;

			CPlayerScript::SetOtherPower(GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT) * fDT * (300.0f - Distance));
		}
		else
		{
			CPlayerScript::SetOtherPower(Vec3(0.0f, 0.0f, 0.0f));
		}

		m_LaserAimTime += fDT;

		if (false == ((CCastleLaser*)m_Laser->GetScript())->IsActive())
		{
			((CCastleLaser*)m_Laser->GetScript())->SetActive(true);
			m_Laser->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		}
			
		// 레이저 조준
		if (m_LaserAimTime > 0.5f)
		{
			m_LaserPoint->ParticleSystem()->SetMaxCount(100);

			Vec3 OriginRot = m_LaserOrigin->Transform()->GetLocalRot();
			Vec3 OriginPos = m_LaserOrigin->Transform()->GetLocalPos();

			Vec3 LaserPointPos = m_LaserPoint->Transform()->GetLocalPos();

			Vec3 vOriginFront = m_LaserOrigin->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			Vec3 vOriginUP = m_LaserOrigin->Transform()->GetLocalDir(DIR_TYPE::UP);
			Vec3 vOriginRight = m_LaserOrigin->Transform()->GetLocalDir(DIR_TYPE::RIGHT);

			((CCastleLaser*)m_Laser->GetScript())->SetLaserOriginPos(OriginPos);
			((CCastleLaser*)m_Laser->GetScript())->SetLaserPointPos(LaserPointPos);

			Vec3 relativePos = LaserPointPos - OriginPos;

			relativePos.y = 0.f;
			relativePos.Normalize();

			vOriginFront.y = 0.f;
			vOriginFront.Normalize();

			Vec3 vCross = relativePos.Cross(vOriginFront);

			float dot = vCross.Dot(vOriginUP);

			float RotAngle = relativePos.Dot(vOriginFront);

			if (RotAngle > 1.f)
				RotAngle = 1.f;
			else if (RotAngle < 0.f)
				RotAngle = 0.f;

			RotAngle = acosf(RotAngle);

			//플레이어는 내 왼쪽에 있다 
			if (dot > 0.0)
				OriginRot.y -= RotAngle;

			//플레이어는 내 오른쪽에 있다 
			else if (dot < 0.0)
				OriginRot.y += RotAngle;



			relativePos = LaserPointPos - OriginPos;
			vOriginFront = m_LaserOrigin->Transform()->GetLocalDir(DIR_TYPE::FRONT);

			relativePos.x = 0.f;
			relativePos.Normalize();

			vOriginFront.x = 0.f;
			vOriginFront.Normalize();

			vCross = relativePos.Cross(vOriginFront);

		    dot = vCross.Dot(vOriginRight);

			RotAngle = relativePos.Dot(vOriginFront);

			if (RotAngle > 1.f)
				RotAngle = 1.f;
			else if (RotAngle < 0.f)
				RotAngle = 0.f;

			RotAngle = acosf(RotAngle);

			//플레이어는 내 왼쪽에 있다 
			if (dot > 0.0)
				OriginRot.x -= RotAngle;

			//플레이어는 내 오른쪽에 있다 
			else if (dot < 0.0)
				OriginRot.x += RotAngle;

			m_LaserOrigin->Transform()->SetLocalRot(OriginRot);


			// 레이저 발사
			m_LaserTime += fDT;

			if (m_LaserTime > 10.0f)
			{
				GetFSM()->ChangeState(L"Laser_Finish", 0.1f, L"Laser_Finish", false);
			}
		}
	}

}

void TCastleLaser_Ready::Enter()
{

	m_Script = (CCastleScript*)GetScript();

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

	if (nullptr == m_LaserPoint)
	{
		m_LaserPoint = nullptr;
	
		m_LaserPoint = m_Script->IntanciatePrefab(L"LaserPoint", (UINT)LAYER_TYPE::BOSS_EFFECT);
		m_LaserPoint->SetName(L"Laser Aim Spot");

		m_LaserPoint->ParticleSystem()->SetPaperburnPTC(true);

		CurScene->AddObject(m_LaserPoint, LAYER_TYPE::BOSS_EFFECT);
	}
	
	m_LaserPoint->ParticleSystem()->SetMaxCount(0);
	m_LaserPoint->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());


	if(nullptr==m_LaserOrigin)
	{
		m_LaserOrigin = new CGameObject;

		m_LaserOrigin->SetName(L"Laser Origin");
		m_LaserOrigin->AddComponent(new CTransform);
		m_LaserOrigin->AddComponent(new CMeshRender);

		m_LaserOrigin->Transform()->SetLocalPos(Vec3(1.0f, 1.0f, 1.0f));
		m_LaserOrigin->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));

		m_LaserOrigin->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		m_LaserOrigin->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		m_LaserOrigin->MeshRender()->Activate(false);


		CurScene->AddObject(m_LaserOrigin, LAYER_TYPE::BOSS_EFFECT);
	}

	CreateLaser();

	Vec3 ReemPos = GetObj()->Transform()->GetLocalPos();

	ReemPos += Vec3(-11.f, 1563.f, -210.f);

	m_LaserOrigin->Transform()->SetLocalPos(ReemPos);

	Vec3 RedeemRot = GetObj()->Transform()->GetLocalRot();

	//m_LaserOrigin->Transform()->SetLocalRot()
}

void TCastleLaser_Ready::Exit()
{
	m_LaserTime = 0.0f;
	m_LaserAimTime = 0.0f;
	m_LaserPoint->Transform()->SetLocalPos(Vec3(-9999.f, -9999.f, -9999.f));
	m_Laser->Transform()->SetLocalPos(Vec3(-9999.f, -9999.f, -9999.f));
	m_LaserPoint->ParticleSystem()->SetMaxCount(0);

	((CCastleLaser*)m_Laser->GetScript())->SetActive(false);
}

TCastleLaser_Ready::TCastleLaser_Ready()
	: m_LaserTime(0.0f)
	, m_LaserAimTime(0.0f)
	, m_LaserPoint(nullptr)
	, m_Script(nullptr)
{
}

TCastleLaser_Ready::~TCastleLaser_Ready()
{
}

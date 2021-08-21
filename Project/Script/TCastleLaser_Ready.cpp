#include "pch.h"
#include "TCastleLaser_Ready.h"
#include "CCastleScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TCastleLaser_Ready::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (360 <= CurAni->GetFrameIdx())
	{
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 LaserPointPos = m_LaserPoint->Transform()->GetLocalPos();
		Vec3 Diff = PlayerPos - LaserPointPos;
		Diff.y = 0.0f;
		Diff.Normalize();

		LaserPointPos += Diff * fDT * 800.0f;
		m_LaserPoint->Transform()->SetLocalPos(LaserPointPos);

		// 플레이어가 일정 범위 안에 있는 경우 밀어 낸다
		if (m_Script->RangeSearch(1000.0f))
		{
			PlayerPos += GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT) * fDT * 200.0f;
			CPlayerScript::GetPlayer()->Transform()->SetLocalPos(PlayerPos);
		}

		m_LaserAimTime += fDT;
		// 레이저 조준
		if (m_LaserAimTime > 3.0f)
		{
			// 레이저 발사
			m_LaserTime += fDT;

			if (m_LaserTime > 4.0f)
			{
				GetFSM()->ChangeState(L"Laser_Finish", 0.1f, L"Laser_Finish", false);
			}
		}
	}

}

void TCastleLaser_Ready::Enter()
{
	m_Script = (CCastleScript*)GetScript();

	m_LaserPoint = new CGameObject;
	m_LaserPoint->AddComponent(new CTransform);
	m_LaserPoint->AddComponent(new CMeshRender);

	m_LaserPoint->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
	m_LaserPoint->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

	m_LaserPoint->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	m_LaserPoint->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(m_LaserPoint, 9);
}

void TCastleLaser_Ready::Exit()
{
	m_LaserTime = 0.0f;
	m_LaserAimTime = 0.0f;
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

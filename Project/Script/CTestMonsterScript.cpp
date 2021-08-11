#include "pch.h"
#include "CTestMonsterScript.h"
#include "CPlayerScript.h"
#include "CTestMonsterAttackEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <random>

void CTestMonsterScript::awake()
{
	RevolutionPos = Transform()->GetLocalPos();
	RevolutionPos.x -= 100.0f;
}

void CTestMonsterScript::update()
{
	CMonsterScript::update();

	switch (m_CurState)
	{
	case CMonsterScript::MONSTERSTATE::IDLE: Idle(); break;
	case CMonsterScript::MONSTERSTATE::MOVE: Move(); break;
	case CMonsterScript::MONSTERSTATE::CHASE: Chase(); break;
	case CMonsterScript::MONSTERSTATE::READY_ACTION: ReadyAction(); break;
	case CMonsterScript::MONSTERSTATE::ATTACK: Attack(); break;
	case CMonsterScript::MONSTERSTATE::FINISH_ACTION: FinishAction(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}
}



void CTestMonsterScript::Idle()
{
	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		// 원거리 공격
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CTestMonsterAttackEffect);	

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f,100.0f,100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 10);

	}

	// 플레이어 따라가기
	//Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	//Vec3 Pos = Transform()->GetLocalPos();
	//
	//Vec3 Distance = PlayerPos - Pos;
	//
	//Pos.x += CTimeMgr::GetInst()->GetfDT() * Distance.x;
	//Pos.z += CTimeMgr::GetInst()->GetfDT() * Distance.z;
	//
	//Transform()->SetLocalPos(Pos);


	// 주변 공전
	//RevolutionPos;
	//
	//{
	//
	//	Vec3 Pos = Transform()->GetLocalPos();
	//	Pos = Vec3(fRadius * cosf(fTheta), 0.0f, fRadius * sinf(fTheta));
	//	fTheta += CTimeMgr::GetInst()->GetfDT() * (XM_2PI / iSliceCount) * 10.0f;
	//	Transform()->SetLocalPos(Pos);
	//}


	if (RangeSearch(700.0f))
	{
		ChangeState(MONSTERSTATE::CHASE);
	}
}

void CTestMonsterScript::Move()
{
}

void CTestMonsterScript::Chase()
{
	if (RangeSearch(300.0f))
	{
		ChangeState(MONSTERSTATE::IDLE);
	}
}

void CTestMonsterScript::ReadyAction()
{
}

void CTestMonsterScript::Attack()
{
}

void CTestMonsterScript::FinishAction()
{
}

void CTestMonsterScript::Death()
{
}

CTestMonsterScript::CTestMonsterScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::TESTMONSTERSCRIPT;
	Test = 2.0f;
	fRadius = 100.0f;
	iSliceCount = 40;
	fTheta = 0.f;


}

CTestMonsterScript::~CTestMonsterScript()
{
}


void CTestMonsterScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CTestMonsterScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
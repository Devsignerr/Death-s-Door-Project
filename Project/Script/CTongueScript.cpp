#include "pch.h"
#include "CTongueScript.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"
#include "CTongueBullet.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void CTongueScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vRot);

	m_RotObj = new CGameObject;
	m_RotObj->AddComponent(new CTransform);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(m_RotObj, 9);
}

void CTongueScript::update()
{
	CMonsterScript::update();
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	switch (m_CurState)
	{
	case CMonsterScript::MONSTERSTATE::IDLE: Idle(); break;
	case CMonsterScript::MONSTERSTATE::MOVE: Move(); break;
	case CMonsterScript::MONSTERSTATE::CHASE: Chase(); break;
	case CMonsterScript::MONSTERSTATE::READY_ACTION: ReadyAction(); break;
	case CMonsterScript::MONSTERSTATE::ATTACK: Attack(); break;
	case CMonsterScript::MONSTERSTATE::FINISH_ACTION: FinishAction(); break;
	case CMonsterScript::MONSTERSTATE::SPECIAL_ACTION: SpecialAction(); break;
	case CMonsterScript::MONSTERSTATE::JUMP: Jump(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}
}



void CTongueScript::Idle()
{
	// 방어중에  플레이어의 위치에따라 같이 회전 
	// 방어 깨지면  공격

	if (false == RangeSearch(1000.0f))
	{
		ChangeState(MONSTERSTATE::MOVE,0.2f,L"Chase");
	}
}

void CTongueScript::Move()
{
	// 바로 플레이어 앞까지 간후 방어태세
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 650.0f;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 650.0f;

	MonsterRotateSystem(5.0f);
	Transform()->SetLocalPos(vPos);

	if (RangeSearch(400.0f))
	{
		ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"Guard");

		//ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
	}
}

void CTongueScript::Chase()
{
}

void CTongueScript::ReadyAction()
{
	// 유도탄 발사 후 깨물기
	// 플레이어가 가까이 있으면 한바퀴 돈 후 땅 찍기
	// 그 후에도 가까이 있으면 백스텝으로 거리 벌린후 다시 Move 후 패턴 반복
}

void CTongueScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true && CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance")
	{
		if (false == RangeSearch(1000.0f))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"SpinDown")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			ChangeState(MONSTERSTATE::FINISH_ACTION, 0.2f, L"SpinUp");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LongDistance")
	{
		if (273 <= CurAni->GetFrameIdx() && 320 >= CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(2.0f);
		}
		if (338 == CurAni->GetFrameIdx())
		{
			LongDistanceAttack();
		}
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_BulletLimit = false;
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");

		}
		else if (false == RangeSearch(1500.0f))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (1012 > CurAni->GetFrameIdx())
		{
			m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
			MonsterRotateSystem(2.0f);
		}
		if (1008 <= CurAni->GetFrameIdx() && 1016 >= CurAni->GetFrameIdx())
		{
			CalAttackDistance();
		}

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			MeleeAttack();

			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);

			if (0 == Pattern)
			{
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep2");
			}
			else if (1 == Pattern)
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
			}
			else if (2 == Pattern)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"Guard");
			}

		}

	}
}

void CTongueScript::FinishAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"SpinUp")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(400.0f))
			{
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep2");
			}
			else if (false == RangeSearch(400.0f) && true == RangeSearch(1000.0f))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
			else if (false == RangeSearch(1000.0f))
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
			}
		}
	}

}

void CTongueScript::SpecialAction()
{
	// 가드
	// 가드 유지
	// 가드 회전
	// 이동

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardBreak");
	}

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	float dot = vCross.Dot(vUP);
	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Guard")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_Test = 0;
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay")
	{
		if (dist > 1.9f)
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");

		//플레이어는 내 왼쪽에 있다 
		else if (dot > 200.0)
		{
			++m_Test;

			if (1 == m_Test)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"LeftSpin");
			}
			
		}

		//플레이어는 내 오른쪽에 있다 
		else if (dot < -200.0)
		{
			++m_Test;

			if (1 == m_Test)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"RightSpin");
			}
		}

	}
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin")
	{
		
		m_Test = 0;
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * 1.0f;
		Transform()->SetLocalRot(Rot);

		if (dot > -5.0 && dot < 5.0 && dist < 1.f)
		{
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
	{
		m_Test = 0;
		Rot.y += CTimeMgr::GetInst()->GetfDT() * 1.0f;
		Transform()->SetLocalRot(Rot);
		if (dot > -5.0 && dot < 5.0 && dist < 1.f)
		{
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardBreak")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(400.0f))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
			}
			else
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
		}
	}

}

void CTongueScript::Jump()
{
	Vec3 BackDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 Pos = Transform()->GetLocalPos();
	Pos += fDT * BackDir * 1800.0f;

	Transform()->SetLocalPos(Pos);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"BackStep2")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (0 == Pattern)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
			}
			else if (1 == Pattern)
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
		}
	}
}

void CTongueScript::Death()
{
}

void CTongueScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CTongueBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CTongueBullet* Script = (CTongueBullet*)Obj->GetScript();

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}

void CTongueScript::MeleeAttack()
{
}

void CTongueScript::CalAttackDistance()
{
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	m_AttackDir.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * 2400.0f;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * 2400.0f;

	Transform()->SetLocalPos(vPos);

}

CTongueScript::CTongueScript()
	: m_Test(0)
	, m_fTheta(0.0f)
	, m_AttackDir{}
	, m_BulletLimit(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::TONGUESCRIPT;
}

CTongueScript::~CTongueScript()
{
}

void CTongueScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CTongueScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
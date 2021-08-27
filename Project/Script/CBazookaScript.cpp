#include "pch.h"
#include "CBazookaScript.h"

#include "CPlayerScript.h"
#include "CBazookaBullet.h"

#include <Engine/CLight3D.h>
#include <Engine/CParticleSystem.h>

void CBazookaScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CBazookaScript::update()
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
	case CMonsterScript::MONSTERSTATE::JUMP: Jump(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}

}

void CBazookaScript::Idle()
{
	if (RangeSearch(m_LongDistanceAttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}
void CBazookaScript::Chase()
{

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();
	Vec3 vMovePos = {};

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_Speed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_Speed;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

	MonsterRotateSystem(m_ChaseRotSpeed);
	// 플레이어 따라오게 
	if (RangeSearch(m_LongDistanceAttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}

void CBazookaScript::ReadyAction()
{

	bool Focus = false;

	if (MonsterRotateSystem(m_AimRotSpeed))
		Focus = true;


	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (false == Focus && CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim",true);
	}

	// 만약 플레이어가 근접해 있다면 근접공격

	if (RangeSearch(m_MeleeAttackRange))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		MeleeAttack();
	}

	m_AimTime += fDT;

	if (true == Focus && m_AimTime > 2.0f)
	{
		m_AimTime = 0.0f;

		if (true == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			LongDistanceAttack();
		}
		else if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Move");
		}
	}


	// 일정시간동안 각도 계산
	// 플레이어가 멀어지면 사정거리 닿을때까지 이동

}




void CBazookaScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (201 > CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_AttackRotSpeed);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		
		if (RangeSearch(m_MeleeAttackRange))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		}
		else if (RangeSearch(m_LongDistanceAttackRange) && false == RangeSearch(m_MeleeAttackRange))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
		}
		else if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE,0.2f, L"Move");
		}
	}

}

void CBazookaScript::Death()
{
}

void CBazookaScript::MeleeAttack()
{
}

void CBazookaScript::LongDistanceAttack()
{
	CGameObject* Obj = new CGameObject;
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CBazookaBullet);

	Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
	Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
	CBazookaBullet* Script = (CBazookaBullet*)Obj->GetScript();

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, 9);
	Obj->awake();
}

void CBazookaScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}
}

void CBazookaScript::OnCollision(CGameObject* _pOther)
{
}

void CBazookaScript::OnCollisionExit(CGameObject* _pOther)
{
}

CBazookaScript::CBazookaScript()
	: m_AimTime(0.0f)
	, m_LongDistanceAttackRange(2000.0f)
	, m_Speed(900.0f)
	, m_MeleeAttackRange(500.0f)
	, m_ChaseRotSpeed(5.0f)
	, m_AimRotSpeed(2.0f)
	, m_AttackRotSpeed(3.4f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BAZOOKASCRIPT;
	m_MonsterInfo.Hp = 6;
}

CBazookaScript::~CBazookaScript()
{
}


void CBazookaScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CBazookaScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}

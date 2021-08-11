#include "pch.h"
#include "CBazookaScript.h"

#include "CPlayerScript.h"
#include "CBazookaBullet.h"

void CBazookaScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vRot);
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
	if (RangeSearch(2000.0f))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}

void CBazookaScript::Move()
{
}

void CBazookaScript::Chase()
{

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 900.0f;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 900.0f;

	MonsterRotateSystem(5.0f);
	Transform()->SetLocalPos(vPos);
	// 플레이어 따라오게 
	if (RangeSearch(2000.0f))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}

void CBazookaScript::ReadyAction()
{

	bool Focus = false;

	if (MonsterRotateSystem(2.0f))
		Focus = true;


	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (false == Focus && CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim",true);
	}

	// 만약 플레이어가 근접해 있다면 근접공격

	if (RangeSearch(500.0f))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		MeleeAttack();
	}

	m_AimTime += fDT;

	if (true == Focus && m_AimTime > 2.0f)
	{
		m_AimTime = 0.0f;

		if (true == RangeSearch(2000.0f))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			LongDistanceAttack();
		}
		else if (false == RangeSearch(2000.0f))
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
			MonsterRotateSystem(3.4f);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		
		if (RangeSearch(500.0f))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		}
		else if (RangeSearch(2000.0f) && false == RangeSearch(500.0f))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
		}
		else if (false == RangeSearch(2000.0f))
		{
			ChangeState(MONSTERSTATE::CHASE,0.2f, L"Move");
		}
	}

}

void CBazookaScript::FinishAction()
{
}

void CBazookaScript::Jump()
{
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

CBazookaScript::CBazookaScript()
	: m_AimTime(0.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BAZOOKASCRIPT;
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

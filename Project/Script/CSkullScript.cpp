#include "pch.h"
#include "CSkullScript.h"
#include "CPlayerScript.h"
#include "CSkullBullet.h"


void CSkullScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CSkullScript::update()
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

void CSkullScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}

}

void CSkullScript::OnCollision(CGameObject* _pOther)
{
}

void CSkullScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CSkullScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CSkullBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CSkullBullet* Script = (CSkullBullet*)Obj->GetScript();

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}

void CSkullScript::MeleeAttack()
{
}

void CSkullScript::Idle()
{
	if (RangeSearch(2000.0f))
	{
		ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
	}
}

void CSkullScript::Chase()
{
	// 초기 상태
	// 플레이어가 공격 범위내에 들어온경우

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	MonsterRotateSystem(m_RotSpeed);
	Transform()->SetLocalPos(vPos);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (RangeSearch(m_MeleeAttackRange))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
		}
	}
}

void CSkullScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (264 > CurAni->GetFrameIdx() || 274 < CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_RotSpeed);
		}

		if (264 <= CurAni->GetFrameIdx() && 266 >= CurAni->GetFrameIdx())
		{
			Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vDiff = vPlayerPos - vPos;
			vDiff.Normalize();

			vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_AttackMoveSpeed;
			vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_AttackMoveSpeed;

			Transform()->SetLocalPos(vPos);
		}

	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance" )
	{
		// LongDistance 아닐때
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee" && RangeSearch(m_MeleeAttack2Range))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee2");
			}
			else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee2" && RangeSearch(m_MeleeAttackRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
			}
			else if (false == RangeSearch(m_MeleeAttackRange) && RangeSearch(m_LongDistanceAttackRange))
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
		
			}
			else
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
		
		}
	}
	else
	{
		// LongDistance 일때
		// 한번 던지고 따라가면서 점점 거리를 좁힌다
		
		if (378 == CurAni->GetFrameIdx())
		{
			LongDistanceAttack();
		}


		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_BulletLimit = false;
			if (RangeSearch(m_MeleeAttackRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
			}
			else
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
			}
		}
	}

}

void CSkullScript::Death()
{
}

CSkullScript::CSkullScript()
	: m_BulletLimit(false)
	, m_ChaseSpeed(450.0f)
	, m_MeleeAttackRange(800.0f)
	, m_MeleeAttack2Range(1000.0f)
	, m_LongDistanceAttackRange(1800.0f)
	, m_AttackMoveSpeed(900.0f)
	, m_RotSpeed(5.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SKULLSCRIPT;
	m_MonsterInfo.Hp = 10;
}

CSkullScript::~CSkullScript()
{
}


void CSkullScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CSkullScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
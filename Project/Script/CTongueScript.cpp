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
	// ����߿�  �÷��̾��� ��ġ������ ���� ȸ�� 
	// ��� ������  ����

	if (false == RangeSearch(1000.0f))
	{
		ChangeState(MONSTERSTATE::MOVE,0.2f,L"Chase");
	}
}

void CTongueScript::Move()
{
	// �ٷ� �÷��̾� �ձ��� ���� ����¼�
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	Vec3 vMovePos = {};
	vDiff.Normalize();

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

	MonsterRotateSystem(m_ChaseRotSpeed);

	if (RangeSearch(m_GuardRange))
	{
		ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"Guard");

		//ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
	}
}

void CTongueScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true && CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance")
	{
		if (false == RangeSearch(m_ChaseRange))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"SpinDown")
	{
		MonsterRotateSystem(3.0f);
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			ChangeState(MONSTERSTATE::FINISH_ACTION, 0.2f, L"SpinUp");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LongDistance")
	{
		if (273 <= CurAni->GetFrameIdx() && 320 >= CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_AttackRotSpeed);
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
		else if (false == RangeSearch(m_ChaseRange))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (1012 > CurAni->GetFrameIdx())
		{
			m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
			MonsterRotateSystem(m_AttackRotSpeed);
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
			if (RangeSearch(m_BackStepRange))
			{
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep2");
			}
			else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_ChaseRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
			else if (false == RangeSearch(m_ChaseRange))
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
			}
		}
	}

}

void CTongueScript::SpecialAction()
{
	// ����
	// ���� ����
	// ���� ȸ��
	// �̵�

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
			m_SpinCount = 0;
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay")
	{
		if (dist > 1.9f)
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");

		//�÷��̾�� �� ���ʿ� �ִ� 
		else if (dot > 200.0)
		{
			++m_SpinCount;

			if (1 == m_SpinCount)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"LeftSpin");
			}
			
		}

		//�÷��̾�� �� �����ʿ� �ִ� 
		else if (dot < -200.0)
		{
			++m_SpinCount;

			if (1 == m_SpinCount)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"RightSpin");
			}
		}

	}
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin")
	{
		
		m_SpinCount = 0;
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * 1.0f;
		Transform()->SetLocalRot(Rot);

		if (dot > -5.0 && dot < 5.0 && dist < 1.f)
		{
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
	{
		m_SpinCount = 0;
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
			if (RangeSearch(m_SpinDownRange))
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
	Vec3 vMovePos = {};

	vMovePos += fDT * BackDir * 1800.0f;

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

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

void CTongueScript::OnCollisionEnter(CGameObject* _pOther)
{
	// �÷��̾��� ������ �������
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		CAnimator3D* CurAni = Animator3D();
		UINT iCurClipIdx = CurAni->GetClipIdx();

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
		{
			--m_ShieldPoint;

			if (0 == m_ShieldPoint)
			{
				m_ShieldPoint = 3;
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.05f, L"GuardBreak");
			}
		}
		else
		{
			--m_MonsterInfo.Hp;
		}
	}
}

void CTongueScript::OnCollision(CGameObject* _pOther)
{
}

void CTongueScript::OnCollisionExit(CGameObject* _pOther)
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
	Vec3 vMovePos = {};
	m_AttackDir.Normalize();

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * 2400.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * 2400.0f;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

}

CTongueScript::CTongueScript()
	: m_SpinCount(0)
	, m_fTheta(0.0f)
	, m_AttackDir{}
	, m_BulletLimit(false)
	, m_ShieldPoint(3)
	, m_ChaseSpeed(650.0f)
	, m_GuardRange(400.0f)
	, m_BackStepRange(400.0f)
	, m_ChaseRange(1200.0f)
	, m_SpinDownRange(600.0f)
	, m_ChaseRotSpeed(5.0f)
	, m_AttackRotSpeed(2.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::TONGUESCRIPT;
	m_MonsterInfo.Hp = 8;
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
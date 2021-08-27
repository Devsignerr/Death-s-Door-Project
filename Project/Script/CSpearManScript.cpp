#include "pch.h"
#include "CSpearManScript.h"
#include "CPlayerScript.h"

void CSpearManScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CSpearManScript::update()
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

void CSpearManScript::Idle()
{
	if (RangeSearch(1500.0f))
	{
		ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
	}
}

void CSpearManScript::Move()
{
}

void CSpearManScript::Chase()
{
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	Vec3 vMovePos = {};

	
	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	bool IsGround = GroundCheck(vPos + vMovePos);

	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if(IsGround==true)
		Transform()->SetLocalPos(vPos + vMovePos);
	

	MonsterRotateSystem(m_ChaseRotSpeed);

	if (RangeSearch(m_AttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.1f, L"LeftCutReady");
	}
}

void CSpearManScript::ReadyAction()
{

	if (true == m_IsEvasion)
	{
		if (MonsterRotateSystem(m_ReadyAction2RotSpeed))
		{
			m_IsEvasion = false;
		}
	}
	else
	{
		MonsterRotateSystem(m_ReadyActionRotSpeed);
	}

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftCutReady")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.1f, L"LeftCut");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftCutLink")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.1f, L"RightCut");
		}
	}
}

void CSpearManScript::Attack()
{
	// 공격 두번후 반대 방향으로 점프하면서 회피하는것같다
	// 플레이어 위치에 맞춰 공격 방향 달라짐

	MonsterRotateSystem(m_AttackRotSpeed);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftCut")
	{
		if (378 <= CurAni->GetFrameIdx() && 380 >= CurAni->GetFrameIdx())
		{
			Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);


			Vec3 vMovePos = {};

			vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vFront.x * m_AttackMoveSpeed;
			vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vFront.z * m_AttackMoveSpeed;

			bool IsGround = GroundCheck(vPos + vMovePos);
			if (!IsGround)
				IsGround = ResearchNode(vPos + vMovePos);

			if (true == IsGround)
				Transform()->SetLocalPos(vPos + vMovePos);
		}


		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_Attack2Range))
			{
				ChangeState(MONSTERSTATE::READY_ACTION, 0.1f, L"LeftCutLink");
			}
			else
			{
				ChangeState(MONSTERSTATE::FINISH_ACTION, 0.1f, L"LeftCutEnd");
			}
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightCut")
	{
		if (445 <= CurAni->GetFrameIdx() && 450 >= CurAni->GetFrameIdx())
		{
			Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

			Vec3 vMovePos = {};
			vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vFront.x * m_AttackMoveSpeed;
			vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vFront.z * m_AttackMoveSpeed;

			bool IsGround = GroundCheck(vPos + vMovePos);
			if (!IsGround)
				IsGround = ResearchNode(vPos + vMovePos);

			if (true == IsGround)
				Transform()->SetLocalPos(vPos + vMovePos);

		}

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_EvasionRange))
			{
				ChangeState(MONSTERSTATE::JUMP, 0.1f, L"Evasion");
			}
			else
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
			}
		}
	}

}

void CSpearManScript::FinishAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftCutEnd")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_AttackRange))
			{
				(MONSTERSTATE::READY_ACTION, 0.1f, L"LeftCutReady");
			}
			else
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
			}
		}
	}
}

void CSpearManScript::Jump()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Evasion")
	{
		if (133 > CurAni->GetFrameIdx())
		{
			Vec3 Rot = Transform()->GetLocalRot();
			Rot.y -= fDT * (XM_PI * 2.0f);
			Transform()->SetLocalRot(Rot);
		}

		Vec3 Pos = Transform()->GetLocalPos();
		Vec3 RightDir = Transform()->GetLocalDir(DIR_TYPE::UP);
		RightDir *= 1000.0f;
		Vec3 vMovePos = {};

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * RightDir.x;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * RightDir.z;

		bool IsGround = GroundCheck(Pos + vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(Pos + vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(Pos + vMovePos);

		// 회전

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_IsEvasion = true;
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
		}
	}
}

void CSpearManScript::Death()
{
}

void CSpearManScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}
}

void CSpearManScript::OnCollision(CGameObject* _pOther)
{
}

void CSpearManScript::OnCollisionExit(CGameObject* _pOther)
{
}

CSpearManScript::CSpearManScript()
	: m_IsEvasion(false)
	, m_ChaseSpeed(650.0f)
	, m_AttackMoveSpeed(1800.0f)
	, m_AttackRange(1000.0f)
	, m_Attack2Range(1500.0f)
	, m_EvasionRange(800.0f)
	, m_AttackRotSpeed(4.5f)
	, m_ChaseRotSpeed(3.5f)
	, m_ReadyActionRotSpeed(2.5f)
	, m_ReadyAction2RotSpeed(20.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SPEARMANSCRIPT;
	m_MonsterInfo.Hp = 15;
}

CSpearManScript::~CSpearManScript()
{
}

void CSpearManScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CSpearManScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
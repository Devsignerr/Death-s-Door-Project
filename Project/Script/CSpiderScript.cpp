#include "pch.h"
#include "CSpiderScript.h"
#include "CRandomMgrScript.h"
#include "CPlayerScript.h"

void CSpiderScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CSpiderScript::update()
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


void CSpiderScript::Idle()
{
	if (RangeSearch(1000.0f))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"ReadyAction");
	}
}

void CSpiderScript::Move()
{
	// 플레이어가 완전 근접해있는경우 뒷걸음질
	// 플레이어가 멀리떨어지면 따라감
	// 플레이거가 가까이 있는 경우 게걸음
	// 플레이어가 특정 범위 안에 있는경우 공격
	// 아마 벽에 일정시간 부딛히는 경우 점프하는것 같다	

	MonsterRotateSystem(m_RotSpeed);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftMove")
	{
		Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vPos = Transform()->GetLocalPos();

		vPos -= vRight;
		Transform()->SetLocalPos(vPos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightMove")
	{
		Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vPos = Transform()->GetLocalPos();

		vPos += vRight;
		Transform()->SetLocalPos(vPos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"FrontMove")
	{
		Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vDiff = vPlayerPos - vPos;
		vDiff.Normalize();

		vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_MoveSpeed;
		vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_MoveSpeed;

		MonsterRotateSystem(m_RotSpeed);
		Transform()->SetLocalPos(vPos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"BackMove")
	{

		Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vDiff = vPlayerPos - vPos;
		vDiff.Normalize();

		vPos.x -= CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_MoveSpeed;
		vPos.z -= CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_MoveSpeed;

		MonsterRotateSystem(m_RotSpeed);
		Transform()->SetLocalPos(vPos);

		m_BackMoveTime += fDT;

		if (1.0f < m_BackMoveTime)
		{
			m_BackMoveCheck = false;
			m_BackMoveTime = 0.0f;
			if (RangeSearch(300.0f))
			{
				SetMonsterJumpInfo(0.6f, 1000.0f);
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
			}
		}
	
	}

	m_MoveTime += fDT;

	if (1.5f < m_MoveTime && false == m_BackMoveCheck)
	{
		m_MoveTime = 0.0f;
		if (false == RangeSearch(m_BackStepRange) && RangeSearch(m_FrontMoveRange))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);
		
			if (0 == Pattern && m_MoveCount <= 2)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
				++m_MoveCount;
			}
			else if (1 == Pattern && m_MoveCount <= 2)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");
				++m_MoveCount;
			}
			else if (2 == Pattern || m_MoveCount > 2)
			{
				m_MoveCount = 0;
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
			}
		}
	}

	
	if (false == RangeSearch(m_FrontMoveRange) && CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::MOVE, 0.2f, L"FrontMove");
	}



}

void CSpiderScript::ReadyAction()
{
	static bool Focus = false;
	if (MonsterRotateSystem(m_RotSpeed))
		Focus = true;

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	// 애니메이션 끝나면 

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		if (RangeSearch(m_BackStepRange))
		{
			SetMonsterJumpInfo(0.6f, 1000.0f);
			ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
		}
		else if(false == RangeSearch(m_BackStepRange) && RangeSearch(m_FrontMoveRange))
		{
			if (Focus)
			{
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
				Focus = false;
			}
			else
			{
				bool LeftMove = CRandomMgrScript::GetRandomintNumber(0, 1);
		
				if (LeftMove)
					ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
				else
					ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");
			}
		
		}
		else
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"FrontMove");
		}

	}

}

void CSpiderScript::Attack()
{
	// 애니메이션 끝나면 


	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (124 == CurAni->GetFrameIdx())
	{
		m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
		m_fTheta = -XM_PI / 2.f;
	}

	if (154 < CurAni->GetFrameIdx() && 160 > CurAni->GetFrameIdx())
	{
		CalAttackDistance();
	}
	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (RangeSearch(m_BackStepRange))
		{
			m_BackMoveCheck = true;
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"BackMove");
		}
		else if (false == RangeSearch(m_BackStepRange) && RangeSearch(m_FrontMoveRange))
		{
			bool LeftMove = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (LeftMove)
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
			else
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");

		}
		else
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"FrontMove");
		}
	}
}

void CSpiderScript::Jump()
{
	MonsterJumpSystem();
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(m_BackStepRange) && RangeSearch(m_FrontMoveRange))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);

			if (0 == Pattern)
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
			else if (1 == Pattern)
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");
			else if (2 == Pattern)
			{
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
			}
		}
		else
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"FrontMove");
		}

	}
	// 3번 이상 반복되면 강제로 공격으로 넘어갈수있게
}

void CSpiderScript::Death()
{
}

void CSpiderScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}
}

void CSpiderScript::OnCollision(CGameObject* _pOther)
{
}

void CSpiderScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CSpiderScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();
	float Speed = -(sinf(m_fTheta)) * 2.0f;

	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 2000.0f;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 2000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * XM_PI / 2.0f;

	if ((XM_PI * 3 / 2) < m_fTheta)
	{
		m_fTheta = -XM_PI / 2.f;
	}

	Transform()->SetLocalPos(Pos);
}

CSpiderScript::CSpiderScript()
	: m_MoveTime(0.0f)
	, m_fTheta(0.0f)
	, m_AttackDir{}
	, m_BackMoveTime(0.0f)
	, m_BackMoveCheck(false)
	, m_MoveCount(0)
	, m_MoveSpeed(450.0f)
	, m_RotSpeed(5.0f)
	, m_FrontMoveRange(1000.0f)
	, m_BackStepRange(400.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SPIDERSCRIPT;

	m_MonsterInfo.Hp = 3;
}

CSpiderScript::~CSpiderScript()
{
}


void CSpiderScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CSpiderScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
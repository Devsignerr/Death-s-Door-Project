#include "pch.h"
#include "CSpiderScript.h"
#include "CRandomMgrScript.h"
#include "CPlayerScript.h"

void CSpiderScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vRot);
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

	MonsterRotateSystem(5.0f);

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

		vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 450.0f;
		vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 450.0f;

		MonsterRotateSystem(5.0f);
		Transform()->SetLocalPos(vPos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"BackMove")
	{

		Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vDiff = vPlayerPos - vPos;
		vDiff.Normalize();

		vPos.x -= CTimeMgr::GetInst()->GetfDT() * vDiff.x * 450.0f;
		vPos.z -= CTimeMgr::GetInst()->GetfDT() * vDiff.z * 450.0f;

		MonsterRotateSystem(5.0f);
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
		if (false == RangeSearch(400.0f) && RangeSearch(1000.0f))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);
			static int MoveCount = 0;
		
			if (0 == Pattern)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
				++MoveCount;
			}
			else if (1 == Pattern)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");
				++MoveCount;
			}
			else if (2 == Pattern || MoveCount > 3)
			{
				MoveCount = 0;
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
			}
		}
	}

	
	if (false == RangeSearch(1000.0f) && CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::MOVE, 0.2f, L"FrontMove");
	}



}

void CSpiderScript::Chase()
{
}

void CSpiderScript::ReadyAction()
{
	static bool Focus = false;
	if (MonsterRotateSystem(5.0f))
		Focus = true;

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	// 애니메이션 끝나면 

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		if (RangeSearch(400.0f))
		{
			SetMonsterJumpInfo(0.6f, 1000.0f);
			ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
		}
		else if(false == RangeSearch(400.0f) && RangeSearch(1000.0f))
		{
			if (Focus)
			{
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
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
		m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
		m_fTheta = -XM_PI / 2.f;
	}

	if (154 < CurAni->GetFrameIdx() && 170 > CurAni->GetFrameIdx())
	{
		CalAttackDistance();
	}
	
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (RangeSearch(400.0f))
		{
			m_BackMoveCheck = true;
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"BackMove");
		}
		else if (false == RangeSearch(400.0f) && RangeSearch(1000.0f))
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

void CSpiderScript::FinishAction()
{
}

void CSpiderScript::Jump()
{
	MonsterJumpSystem();
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(400.0f) && RangeSearch(1000.0f))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);

			if (0 == Pattern)
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"LeftMove");
			else if (1 == Pattern)
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"RightMove");
			else if (2 == Pattern)
			{
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
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

void CSpiderScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();
	float Speed = -(sinf(m_fTheta)) * 2.0f;

	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed;
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
{
	m_iScriptType = (int)SCRIPT_TYPE::SPIDERSCRIPT;

	m_MonsterInfo.Hp = 3;
	m_MonsterInfo.Speed = 200.0f;
	m_MonsterInfo.RecognitionRange = 700.0f;
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
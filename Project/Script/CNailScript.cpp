#include "pch.h"
#include "CNailScript.h"
#include "CPlayerScript.h"

void CNailScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CNailScript::update()
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

void CNailScript::Idle()
{
	// 특정 범위를 랜덤으로 어슬렁 거리는것같다
	// 플레이어가 공격 범위에 들어온 거리에 따라 다른 패턴으로 공격을 시작함

	if (RangeSearch(1500.0f))
	{
		ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
	}

}

void CNailScript::Chase()
{

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	MonsterRotateSystem(m_ChaseRotSpeed);
	Transform()->SetLocalPos(vPos);

	if (RangeSearch(m_NailAttackRange))
	{
		m_fTheta = XM_PI / 2.0f;
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
	}
	else if (false == RangeSearch(m_NailAttackRange) && true == RangeSearch(m_JumpAttackRange))
	{
		ResetJumpAttackInfo();
		CalJumpAttackDistance();
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
	}
}

void CNailScript::ReadyAction()
{

	MonsterRotateSystem(m_ReadyActionRotSpeed);


	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"NailAttackReady")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_ShiftAmount = {};
			m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
			ResetBackStepInfo();
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"NailAttack2");

		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttackReady")
	{
		ResetBackStepInfo();
		ChangeState(MONSTERSTATE::ATTACK, 0.02f, L"JumpAttack");
	}

	// 근접 공격이 닿을만한 거리가 아닌경우 
	// 근접 공격이 닿을만한 거리인 경우

}

void CNailScript::Attack()
{
	// 만약 근접공격 이후 플레이어가 밀착해있으면 백스텝
	// 이동하지 않는 근접공격 -> 플레이어와 거리가 가까운 상태가 유지될때
	// 이동하는 근접공격 -> 공격 준비중 플레이어의 거리가 떨어졌을때
	// 점프 공격 -> 플레이어가 멀리 이동했을때 
	// 공격 범위권을 벗어나면 CHASE로 간 후에 근접공격 실시

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"NailAttack2")
	{

		if (271 < CurAni->GetFrameIdx())
		{
			CalAttackDistance();
		}
		else
		{
			m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
		}

	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttack")
	{
		JumpAttackStay();

	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(m_NailAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"NailAttack2")
	{

		if (RangeSearch(m_BackStepRange))
		{
			ChangeState(MONSTERSTATE::JUMP, 0.02f, L"BackStep");
		}
		else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_NailAttackRange))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
		}
		else if (false == RangeSearch(m_NailAttackRange) && true == RangeSearch(m_JumpAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.1f, L"Run2");
		}
		else if (false == RangeSearch(m_JumpAttackRange))
		{
			ResetJumpAttackInfo();
			CalJumpAttackDistance();
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
		}
	}
	else 	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttack")
	{
		if (RangeSearch(m_BackStepRange))
		{
			ChangeState(MONSTERSTATE::JUMP, 0.005f, L"BackStep");
		}
		else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_NailAttackRange))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
		}
		else if (false == RangeSearch(m_NailAttackRange) && true == RangeSearch(m_JumpAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.1f, L"Run2");
		}
		else if (false == RangeSearch(m_JumpAttackRange))
		{
			ResetJumpAttackInfo();
			CalJumpAttackDistance();
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
		}
	}
}

void CNailScript::Jump()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (200 >= CurAni->GetFrameIdx() || 284 == CurAni->GetFrameIdx())
	{
		CalBackStepDistance();
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (RangeSearch(m_NailAttackRange))
		{
			ResetBackStepInfo();
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
		}
		else if (false == RangeSearch(m_NailAttackRange) && true == RangeSearch(m_JumpAttackRange))
		{
			ResetJumpAttackInfo();
			CalJumpAttackDistance();
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
		}
		else if (false == RangeSearch(m_JumpAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
		}
	}
}

void CNailScript::Death()
{
}

void CNailScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}

}

void CNailScript::OnCollision(CGameObject* _pOther)
{
}

void CNailScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CNailScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();

	float Speed = (sinf(m_fTheta)) * 10.0f;

	float Distance = Vec3::Distance(CPlayerScript::GetPlayerPos(), Pos);
	m_ShiftAmount.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 500.0f;
	m_ShiftAmount.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 500.0f;

	float ShiftAmount = abs(m_ShiftAmount.x) + abs(m_ShiftAmount.z);

	if (600.0f < ShiftAmount)
		return;

	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 1000.0f;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 1000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * (XM_PI / 2.0f);

	if (XM_PI < m_fTheta)
	{
		m_fTheta = XM_PI / 2.0f;
	}

	Transform()->SetLocalPos(Pos);
}

void CNailScript::CalBackStepDistance()
{
	// 186 ~ 200
	Vec3 Pos = Transform()->GetLocalPos();
	m_BackStepDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
	m_BackStepDir *= 1000.0f;

	float Speed = (sinf(m_fBackStepTheta)) * 2.5f;

	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * Speed;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * Speed;
	m_fBackStepTheta += CTimeMgr::GetInst()->GetfDT() * (XM_PI / 2.0f);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	wstring Tmp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	int a = CurAni->GetFrameIdx();
	if (193 < CurAni->GetFrameIdx() && 207 >= CurAni->GetFrameIdx())
	{
		Pos.x -= CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * (Speed);
		Pos.z -= CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * (Speed);
	}
	else
	{
		Pos.x += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * (Speed / 8.0f);
		Pos.z += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * (Speed / 8.0f);
	}

	if (XM_PI < m_fBackStepTheta)
	{
		m_fBackStepTheta = XM_PI / 2.0f;
	}

	Transform()->SetLocalPos(Pos);
}

void CNailScript::CalJumpAttackDistance()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	m_Pos = Transform()->GetLocalPos();

	m_MaxHeightTime = 0.3f;
	float DestHeight = PlayerPos.y - m_Pos.y;

	float Height = m_MaxHeight - m_Pos.y;
	m_Gravity = 2 * Height / (m_MaxHeightTime * m_MaxHeightTime);

	m_VelocityY = sqrtf(2 * m_Gravity * Height);

	float a = m_Gravity;
	float b = -2 * m_VelocityY;
	float c = 2 * DestHeight;

	m_DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	m_VelocityX = -(m_Pos.x - PlayerPos.x) / m_DestAttachTime;
	m_VelocityZ = -(m_Pos.z - PlayerPos.z) / m_DestAttachTime;
}

void CNailScript::JumpAttackStay()
{
	m_Time += fDT;
	Vec3 Pos = Transform()->GetLocalPos();

	if (m_Time <= m_DestAttachTime)
	{
		float X = m_Pos.x + m_VelocityX * m_Time;
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;
		float Z = m_Pos.z + m_VelocityZ * m_Time;

		Pos.x = X;
		Pos.y = Y;
		Pos.z = Z;

		Transform()->SetLocalPos(Pos);
	}
	else
	{
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;
		Pos.y = Y;

		if (0.0f > Pos.y)
			Pos.y = 0.0f;

		Transform()->SetLocalPos(Pos);
	}
}

void CNailScript::ResetJumpAttackInfo()
{
	m_Pos = {};
	m_Distance = 0.0f;
	m_Time = 0.0f;
	m_DestAttachTime = 0.0f;
	m_Gravity = 0.0f;
	m_VelocityX = 0.0f;
	m_VelocityY = 0.0f;
	m_VelocityZ = 0.0f;
	m_MaxHeightTime = 0.0f;
	m_MaxHeight = 500.0f;
}

void CNailScript::ResetBackStepInfo()
{
	Vec3 Pos = Transform()->GetLocalPos();
	m_BackStepDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
	m_BackStepDir *= 1000.0f;
}

CNailScript::CNailScript()
	: m_NailAttackRange(1000.0f)
	, m_JumpAttackRange(1500.0f)
	, m_BackStepRange(200.0f)
	, m_ChaseRotSpeed(2.5f)
	, m_ReadyActionRotSpeed(4.5)
	, m_fTheta(XM_PI / 2.0f)
	, m_fBackStepTheta(XM_PI / 2.0f)
	, m_Pos{}
	, m_Distance(0.0f)
	, m_Time(0.0f)
	, m_DestAttachTime(0.0f)
	, m_Gravity(0.0f)
	, m_VelocityX(0.0f)
	, m_VelocityY(0.0f)
	, m_VelocityZ(0.0f)
	, m_MaxHeightTime(0.0f)
	, m_MaxHeight(500.0f)
	, m_ChaseSpeed(650.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::NAILSCRIPT;
	m_MonsterInfo.Hp = 6;
}

CNailScript::~CNailScript()
{
}

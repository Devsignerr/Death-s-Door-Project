#include "pch.h"
#include "CNailScript.h"
#include "CPlayerScript.h"

void CNailScript::awake()
{
	CMonsterScript::awake();

	CreateCol(L"NailCol", Vec3(0.0f, 0.0f, 150.0f), Vec3(200.0f, 150.0f, 300.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"NailAttackCol", Vec3(0.0f, 250.0f, 150.0f), Vec3(200.0f, 150.0f, 200.0f), LAYER_TYPE::MONSTER_ATTACK_COL);

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
	// Ư�� ������ �������� ��� �Ÿ��°Ͱ���
	// �÷��̾ ���� ������ ���� �Ÿ��� ���� �ٸ� �������� ������ ������

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
	Vec3 vMovePos = {};

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

	MonsterRotateSystem(m_ChaseRotSpeed);

	if (RangeSearch(m_NailAttackRange))
	{
		m_fTheta = XM_PI / 2.0f;
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
	}
	else if (false == RangeSearch(m_NailAttackRange) && true == RangeSearch(m_JumpAttackRange))
	{
		SetbJump(true);
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

	// ���� ������ �������� �Ÿ��� �ƴѰ�� 
	// ���� ������ �������� �Ÿ��� ���

}

void CNailScript::Attack()
{
	// ���� �������� ���� �÷��̾ ������������ �齺��
	// �̵����� �ʴ� �������� -> �÷��̾�� �Ÿ��� ����� ���°� �����ɶ�
	// �̵��ϴ� �������� -> ���� �غ��� �÷��̾��� �Ÿ��� ����������
	// ���� ���� -> �÷��̾ �ָ� �̵������� 
	// ���� �������� ����� CHASE�� �� �Ŀ� �������� �ǽ�

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;

	if (279 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
	}

	if (281 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false);

	}


	if (558 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
		TransColPos(Vec3(0.0f, 0.0f, 50.0f));
		TransColScale(Vec3(350.0f, 350.0f, 100.0f));
	}

	if (545 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false, LAYER_TYPE::MONSTER_COL);
	}

	if (562 == CurAni->GetFrameIdx())
	{
		TransColScale(Vec3(200.0f, 150.0f, 200.0f));
		OnOffAttackCol(false);

		OnOffAttackCol(true, LAYER_TYPE::MONSTER_COL);
		TransColPos(Vec3(0.0f, 0.0f, 150.0f), LAYER_TYPE::MONSTER_COL);

	}

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
			SetbJump(true);
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
			SetbJump(true);
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
	// �÷��̾��� ������ �������
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
	Vec3 vMovePos = {};
	float Speed = (sinf(m_fTheta)) * 10.0f;

	float Distance = Vec3::Distance(CPlayerScript::GetPlayerPos(), Pos);
	m_ShiftAmount.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 500.0f;
	m_ShiftAmount.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 500.0f;

	float ShiftAmount = abs(m_ShiftAmount.x) + abs(m_ShiftAmount.z);

	if (600.0f < ShiftAmount)
		return;

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 1000.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 1000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * (XM_PI / 2.0f);

	if (XM_PI < m_fTheta)
	{
		m_fTheta = XM_PI / 2.0f;
	}

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);
}

void CNailScript::CalBackStepDistance()
{
	// 186 ~ 200
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};
	m_BackStepDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
	m_BackStepDir *= 1000.0f;

	float Speed = (sinf(m_fBackStepTheta)) * 2.5f;

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * Speed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * Speed;
	m_fBackStepTheta += CTimeMgr::GetInst()->GetfDT() * (XM_PI / 2.0f);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	wstring Tmp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	int a = CurAni->GetFrameIdx();
	if (193 < CurAni->GetFrameIdx() && 207 >= CurAni->GetFrameIdx())
	{
		vMovePos.x -= CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * (Speed);
		vMovePos.z -= CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * (Speed);
	}
	else
	{
		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.x * (Speed / 8.0f);
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_BackStepDir.z * (Speed / 8.0f);
	}

	if (XM_PI < m_fBackStepTheta)
	{
		m_fBackStepTheta = XM_PI / 2.0f;
	}


	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

}

void CNailScript::CalJumpAttackDistance()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	m_Pos = Transform()->GetLocalPos();

	m_MaxHeight = m_Pos.y + 500.0f;

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
	Vec3 vMovePos = {};

	if (m_Time <= m_DestAttachTime)
	{
		if (false == m_WallCheck)
		{
			vMovePos.x = m_Pos.x + m_VelocityX * m_Time;
			vMovePos.z = m_Pos.z + m_VelocityZ * m_Time;
		}
		else
		{
			vMovePos.x = Pos.x;
			vMovePos.z = Pos.z;
		}

		vMovePos.y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;

		bool IsGround = GroundCheck(vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vMovePos);
		else
			m_WallCheck = true;
	}
	else
	{
		m_WallCheck = false;
	}

}

void CNailScript::ResetJumpAttackInfo()
{
	m_Distance = 0.0f;
	m_Time = 0.0f;
	m_DestAttachTime = 0.0f;
	m_Gravity = 0.0f;
	m_VelocityX = 0.0f;
	m_VelocityY = 0.0f;
	m_VelocityZ = 0.0f;
	m_MaxHeightTime = 0.0f;
	m_MaxHeight = m_Pos.y + 500.0f;
	m_Pos = {};
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
	, m_WallCheck(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::NAILSCRIPT;
	m_MonsterInfo.Hp = 6;
}

CNailScript::~CNailScript()
{
}

#include "pch.h"
#include "CBatScript.h"

#include "CPlayerScript.h"

void CBatScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CBatScript::update()
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

void CBatScript::Idle()
{
	// �÷��̾ ���� �ȿ� ���ö����� ���� ����
	// ���� �ȿ������� �ν��ߴٴ� ��� �ִ�

	Vec3 vBatRot = Transform()->GetLocalRot();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};

	vMovePos += CTimeMgr::GetInst()->GetfDT() * (Transform()->GetLocalDir(DIR_TYPE::UP) / m_fRotRange) * m_Speed;

	vBatRot.y += m_fDegreeToRot * fDT;
	Transform()->SetLocalRot(vBatRot);

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

	if (RangeSearch(m_AttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION,0.2f, L"Recognize");
	}
}


void CBatScript::Chase()
{
	CalChaseMove();

	if (RangeSearch(m_AttackRange))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.05f, L"Attack");
	}
}

void CBatScript::ReadyAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.1f, L"Attack");
	}
}

void CBatScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (80 > CurAni->GetFrameIdx())
	{
		m_fTheta = -XM_PI / 2.f;
		m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
		MonsterRotateSystem(5.0f);
	}

	if (81 < CurAni->GetFrameIdx())
	{
		CalAttackDistance();
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		// ���� ���̶��
		if (false == RangeSearch(m_AttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.05f, L"Chase");
		}
	}
}

void CBatScript::Death()
{
}

void CBatScript::OnCollisionEnter(CGameObject* _pOther)
{
	// �÷��̾��� ������ �������
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}
}

void CBatScript::OnCollision(CGameObject* _pOther)
{
}

void CBatScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CBatScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};
	float Speed = (sinf(m_fTheta) + 1.f) * 5.0f;
	
	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 7000.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 7000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * XM_PI / 2.0f;

	if ((XM_PI* 3.0f/ 2.0f) < m_fTheta)
	{
		m_fTheta = -XM_PI / 2.f;
	}


	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

}

void CBatScript::CalChaseMove()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	// �¿�� ���ұ����ϰ� �̵�

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();
	Vec3 vRelative = PlayerPos - Pos;
	Vec3 vMovePos = {};
	vRelative.Normalize();

	m_fTimetoCheckPlayerPos += fDT;

	if (m_fTimetoCheckPlayerPos > 0.8f)
	{
		//���̰� 
		m_fInternalRadianWithPlayer = vFront.Dot(vRelative);

		Vec3 vCross = vFront.Cross(vRelative);

		float fDot = vCross.Dot(vUp);

		//�÷��̾�� �� ���ʿ� �ִ� 
		if (fDot > 0.0f)
			m_bPlayerMyLeft = true;

		//�÷��̾�� �� �����ʿ� �ִ� 
		else if (fDot < 0.0f)
			m_bPlayerMyLeft = false;

		m_fTimetoCheckPlayerPos = 0.f;
	}

	if (m_bPlayerMyLeft)
		Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
	else
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);

	vMovePos += (vFront / 0.95f) * fDT * 500.f;


	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

	Transform()->SetLocalRot(Rot);
	
}



CBatScript::CBatScript()
	: m_fDegreeToRot(1.0f)
	, m_fRotRange(1.0f)
	, m_AttackDir{}
	, m_fTheta(-XM_PI/2.f)
	, m_fTimetoCheckPlayerPos(0.f)
	, m_fInternalRadianWithPlayer(0.f)
	, m_AttackRange(700.0f)
	, m_Speed(800.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BATSCRIPT;
	m_MonsterInfo.Hp = 1;
}

CBatScript::~CBatScript()
{
}


void CBatScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CBatScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
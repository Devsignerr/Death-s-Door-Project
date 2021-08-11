#include "pch.h"
#include "CBatScript.h"

#include "CPlayerScript.h"

void CBatScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vBatRot = Transform()->GetLocalRot();
	vBatRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vBatRot);
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
	// 플레이어가 범위 안에 들어올때까지 빙빙 돈다
	// 범위 안에들어오면 인식했다는 모션 있다

	Vec3 vBatRot = Transform()->GetLocalRot();
	Vec3 Pos = Transform()->GetLocalPos();
	Pos += CTimeMgr::GetInst()->GetfDT() * (Transform()->GetLocalDir(DIR_TYPE::UP) / m_fRotRange) * m_MonsterInfo.Speed;
	vBatRot.y += m_fDegreeToRot * fDT;
	Transform()->SetLocalRot(vBatRot);
	Transform()->SetLocalPos(Pos);

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		int a = 0;
	}

	if (RangeSearch(m_MonsterInfo.RecognitionRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION,0.2f, L"Recognize");
	}
}

void CBatScript::Move()
{


}

void CBatScript::Chase()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	// 좌우로 구불구불하게 이동
	{
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 Pos = Transform()->GetLocalPos();
		Vec3 Rot = Transform()->GetLocalRot();
		Vec3 vRelative = PlayerPos - Pos;
		vRelative.Normalize();
	
		m_fTimetoCheckPlayerPos += fDT;
	
		if (m_fTimetoCheckPlayerPos > 0.8f)
		{
			//사이각 
			m_fInternalRadianWithPlayer = vFront.Dot(vRelative);
	
			Vec3 vCross = vFront.Cross(vRelative);
	
			float fDot = vCross.Dot(vUp);
	
			//플레이어는 내 왼쪽에 있다 
			if (fDot > 0.0f) 
				m_bPlayerMyLeft = true;
	
			//플레이어는 내 오른쪽에 있다 
			else if (fDot < 0.0f)
				m_bPlayerMyLeft = false;
	
			m_fTimetoCheckPlayerPos = 0.f;
		}
	
		if (m_bPlayerMyLeft)
			Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
		else 
			Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
	
		Pos += (vFront / 0.95f) * fDT * 500.f;
	
		Transform()->SetLocalPos(Pos);
		Transform()->SetLocalRot(Rot);
	}

	if (RangeSearch(m_MonsterInfo.RecognitionRange))
	{
		m_fTheta = -XM_PI / 2.f;
		m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
	}
}

void CBatScript::ReadyAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		m_fTheta = -XM_PI / 2.f;
		m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();

		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
	}
}

void CBatScript::Attack()
{
	

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		// 범위 밖이라면
		if (false == RangeSearch(m_MonsterInfo.RecognitionRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
		}
	
	}
	if (65 == CurAni->GetFrameIdx())
	{
		m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
		m_fTheta = -XM_PI / 2.f;
	}
	if (85 > CurAni->GetFrameIdx())
	{
		MonsterRotateSystem(5.0f);
	}
	if (90 > CurAni->GetFrameIdx())
	{
		CalAttackDistance();
	}
	

}

void CBatScript::FinishAction()
{
}

void CBatScript::Jump()
{
}

void CBatScript::Death()
{
}

void CBatScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();
	float Speed = (sinf(m_fTheta) + 1.f) * 5.0f;

	Pos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed;
	Pos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * XM_PI / 2.0f;

	if ((XM_PI*3/2) < m_fTheta)
	{
		m_fTheta = -XM_PI / 2.f;
	}

	Transform()->SetLocalPos(Pos);
}



CBatScript::CBatScript()
	: m_fDegreeToRot(1.0f)
	, m_fRotRange(1.0f)
	, m_AttackDir{}
	, m_fTheta(-XM_PI/2.f)
	, m_fTestTheta(0.0f)
	, m_ChaseDir{}
	, m_Temp(1.0f)
	, m_fTimetoCheckPlayerPos(0.f),
	m_fInternalRadianWithPlayer(0.f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BATSCRIPT;
	m_MonsterInfo.Hp = 1;
	m_MonsterInfo.Speed = 800.0f;
	m_MonsterInfo.RecognitionRange = 700.0f;
	m_MonsterInfo.DelayNextActionTime = 0.0f;
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
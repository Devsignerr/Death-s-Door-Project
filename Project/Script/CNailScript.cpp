#include "pch.h"
#include "CNailScript.h"
#include "CPlayerScript.h"

void CNailScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vRot);
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

void CNailScript::Move()
{
}

void CNailScript::Chase()
{

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 650.0f;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 650.0f;

	MonsterRotateSystem(2.5f);
	Transform()->SetLocalPos(vPos);

	if (RangeSearch(500.0f))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
	}
}

void CNailScript::ReadyAction()
{

	MonsterRotateSystem(2.5f);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	
	if(CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"NailAttackReady")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"NailAttack2");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttackReady")
	{
		ChangeState(MONSTERSTATE::ATTACK, 2.0f, L"JumpAttack");
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
		m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
		if (281 >= CurAni->GetFrameIdx())
		{
			// 박쥐
			Vec3 vPos = Transform()->GetLocalPos();
			m_AttackDir.Normalize();

			vPos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * 2000.0f;
			vPos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * 2000.0f;

			Transform()->SetLocalPos(vPos);
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttack")
	{
		// 포물선
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(1200.0f))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"NailAttack2")
	{
	
		if (RangeSearch(200.0f))
		{
			ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
		}
		else if (false == RangeSearch(200.0f) && true == RangeSearch(500.0f))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
		}
		else if (false == RangeSearch(500.0f))
		{

			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
		}
	}
	else 	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttack")
	{
		if (RangeSearch(100.0f))
		{
			ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
		}
		else if (false == RangeSearch(100.0f) && true == RangeSearch(200.0f))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttack");
		}
		else if (false == RangeSearch(200.0f) && true == RangeSearch(500.0f))
		{
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"NailAttackReady");
		}
		else if (false == RangeSearch(500.0f))
		{

			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"JumpAttackReady");
		}
	}
}

void CNailScript::FinishAction()
{
}

void CNailScript::Jump()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	// 포물선

	if (200 >= CurAni->GetFrameIdx())
	{
		Vec3 BackDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 Pos = Transform()->GetLocalPos();
		Pos += fDT * BackDir * 1800.0f;
	
		Transform()->SetLocalPos(Pos);
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(500.0f))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
		}
	}
}

void CNailScript::Death()
{
}


CNailScript::CNailScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::NAILSCRIPT;


}

CNailScript::~CNailScript()
{
}



void CNailScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CNailScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
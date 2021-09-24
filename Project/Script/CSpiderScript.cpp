#include "pch.h"
#include "CSpiderScript.h"
#include "CRandomMgrScript.h"
#include "CPlayerScript.h"

#include <Engine/CCollider3D.h>

void CSpiderScript::awake()
{
	CMonsterScript::awake();
	CreateCol(L"SpiderCol", Vec3(0.0f, 50.0f, 100.0f), Vec3(200.0f, 200.0f, 200.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"SpiderAttackCol", Vec3(0.0f, 300.0f, 100.0f), Vec3(200.0f, 200.0f, 200.0f), LAYER_TYPE::MONSTER_ATTACK_COL);
	OnOffAttackCol(false);
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	// 백스텝, 프론트무브
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
		Play_Sound(L"LurkerDodge1", 1, true);
		Play_Sound(L"LurkerDogeWoosh5", 1, true);
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


	Ptr<CSound> Sound = Play_Sound(L"LurkerStep2",1,false,0.1f);

	m_SoundTimer += fDT;

	if (0.35f < m_SoundTimer)
	{
		m_SoundTimer = 0.0f;
		Sound->Stop();
	}

	MonsterRotateSystem(m_RotSpeed);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftMove")
	{
		Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vPos = Transform()->GetLocalPos();

		Vec3 vMovePos = {};

		vMovePos -= vRight;
		bool IsGround = GroundCheck(vPos + vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vPos + vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vPos + vMovePos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightMove")
	{
		Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vMovePos = {};

		vMovePos += vRight;
		bool IsGround = GroundCheck(vPos + vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vPos + vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vPos + vMovePos);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"FrontMove")
	{
		Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vDiff = vPlayerPos - vPos;
		Vec3 vMovePos = {};
		vDiff.Normalize();

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_MoveSpeed;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_MoveSpeed;

		bool IsGround = GroundCheck(vPos + vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vPos + vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vPos + vMovePos);

		MonsterRotateSystem(m_RotSpeed);
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"BackMove")
	{

		Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vDiff = vPlayerPos - vPos;
		Vec3 vMovePos = {};
		vDiff.Normalize();

		vMovePos.x -= CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_MoveSpeed;
		vMovePos.z -= CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_MoveSpeed;

		bool IsGround = GroundCheck(vPos + vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vPos + vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vPos + vMovePos);

		MonsterRotateSystem(m_RotSpeed);

		m_BackMoveTime += fDT;

		if (1.0f < m_BackMoveTime)
		{
			m_BackMoveCheck = false;
			m_BackMoveTime = 0.0f;
			if (RangeSearch(300.0f))
			{
				SetMonsterJumpInfo(0.6f, 1000.0f);
				Play_Sound(L"LurkerDodgeWoosh1", 1, true);
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
				Play_Sound(L"LurkerPrepAttack1", 1, true);
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

	if (189 == CurAni->GetFrameIdx())
	{
		PlaySound(L"LurkerLand4", true);
	}

	// 애니메이션 끝나면 

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		if (RangeSearch(m_BackStepRange))
		{
			SetMonsterJumpInfo(0.6f, 1000.0f);
			Play_Sound(L"LurkerDodgeWoosh1", 1, true);
			ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep");
		}
		else if (false == RangeSearch(m_BackStepRange) && RangeSearch(m_FrontMoveRange))
		{
			if (Focus)
			{
				m_fTheta = -XM_PI / 2.f;
				m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
				Play_Sound(L"LurkerPrepAttack1", 1, true);
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

	if (158 == CurAni->GetFrameIdx())
	{
		PlaySound(L"LurkerAttack1", true);
		OnOffAttackCol(true);
	}

	if (166 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false);
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

	if (208 == CurAni->GetFrameIdx())
	{
		PlaySound(L"LurkerLand2", true);
	}

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
				Play_Sound(L"LurkerPrepAttack1",1,true);
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
	//230
	CAnimator3D* CurAni = Animator3D();
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	if (230 <= CurAni->GetFrameIdx())
	{
		if (false == CurAni->Animator3D()->GetbStop())
			CurAni->Animator3D()->StopAnimation();

		m_PaperBurnTime += fDT;

		for (int i = 0; i < childvec.size(); ++i)
		{
			if (childvec[i]->MeshRender())
			{
				Vec4 BurnInfo = Vec4(1.0f, 0.f, 0.f, m_PaperBurnTime / 2.f);
				int BurnType = (UINT)BURN_TYPE::MONSTER_BURN;

				EffectParamSetting(Vec4(10.f, 1.f, 1.f, 1.f), Vec4(0.01f, 0.005f, 0.005f, 1.f), m_RedTex);

				childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_1, &BurnType);
				childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &BurnInfo);
			}

		}
	}

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->Collider3D())
			childvec[i]->Collider3D()->Activate(false);
	}

	if (3.0f < m_PaperBurnTime)
	{
		if (!GetObj()->IsDead())
		{
			CScript::DeleteObject(GetObj());
		}
	}
}

void CSpiderScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_MonsterInfo.Hp <= 0)
		return;

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		Play_Sound(L"LurkerTakeDamage1", 1, true);
		--m_MonsterInfo.Hp;

		if (0 == m_MonsterInfo.Hp)
		{
			const vector<CGameObject*>& childvec = GetObj()->GetChild();

			for (int i = 0; i < childvec.size(); ++i)
			{
				if (childvec[i]->MeshRender())
				{
					UINT Count = childvec[i]->MeshRender()->GetMtrlCount();
					for (UINT j = 0; j < Count; ++j)
					{
						Ptr<CMaterial> mtrl = childvec[i]->MeshRender()->GetSharedMaterial(j);
						mtrl->SetData(SHADER_PARAM::TEX_4, m_PaperBurnTex.Get());
					}
				}
			}

			m_bDamaged = false;

			Play_Sound(L"LurkerDeath1", 1, true);
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else
		{
			CActorScript::OnCollisionEnter(_pOther);
		}

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
	Vec3 vMovePos = {};

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 2000.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 2000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * XM_PI / 2.0f;

	if ((XM_PI * 3 / 2) < m_fTheta)
	{
		m_fTheta = -XM_PI / 2.f;
	}

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

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
#include "pch.h"
#include "CNailScript.h"
#include "CPlayerScript.h"

#include <Engine/CCollider3D.h>

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
	// 특정 범위를 랜덤으로 어슬렁 거리는것같다
	// 플레이어가 공격 범위에 들어온 거리에 따라 다른 패턴으로 공격을 시작함

	if (RangeSearch(1500.0f))
	{
		ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Run2");
	}

}

void CNailScript::Chase()
{
	Ptr<CSound> Sound = Play_Sound(L"GruntStep", 1, false, 0.2f);

	m_SoundTimer += fDT;

	if (0.35f < m_SoundTimer)
	{
		m_SoundTimer = 0.0f;
		Sound->Stop();
	}

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
		Play_Sound(L"Grunt_DashAttackVoice2", 1, true);
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
		Play_Sound(L"Grunt_JumpAttack2", 1, true);
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

	if (279 == CurAni->GetFrameIdx())
	{
		PlaySound(L"GruntMeleeAttack3",true);
		OnOffAttackCol(true);
	}

	if (281 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false);

	}

	if (545 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false, LAYER_TYPE::MONSTER_COL);
	}

	if (558 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
		PlaySound(L"MeleeBodyFall4", true);
		TransColPos(Vec3(0.0f, 0.0f, 50.0f));
		TransColScale(Vec3(350.0f, 350.0f, 100.0f));
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
			Play_Sound(L"Grunt_Backstep4", 1, true);
			ChangeState(MONSTERSTATE::JUMP, 0.02f, L"BackStep");
		}
		else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_NailAttackRange))
		{
			Play_Sound(L"Grunt_DashAttackVoice2", 1, true);
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
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true
		&& CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"JumpAttack")
	{


		if (RangeSearch(m_BackStepRange))
		{
			SetbJump(true);
			Play_Sound(L"Grunt_Backstep4", 1, true);
			ChangeState(MONSTERSTATE::JUMP, 0.005f, L"BackStep");
		}
		else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_NailAttackRange))
		{
			Play_Sound(L"Grunt_DashAttackVoice2", 1, true);
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
			Play_Sound(L"Grunt_DashAttackVoice2", 1, true);
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
	//602

	CAnimator3D* CurAni = Animator3D();
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	if (590 <= CurAni->GetFrameIdx())
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
		DeleteObject(GetGameObject());
	}
}

void CNailScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_MonsterInfo.Hp <= 0)
		return;

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;


	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		Play_Sound(L"GruntTakeDamage", 1, true);
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
			Play_Sound(L"GruntDeath1", 1, true);
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else
		{
			CActorScript::OnCollisionEnter(_pOther);
		}


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

#include "pch.h"
#include "CSpearManScript.h"
#include "CPlayerScript.h"
#include <Engine/CCollider3D.h>

void CSpearManScript::awake()
{
	CMonsterScript::awake();

	CreateCol(L"SpearManCol", Vec3(0.0f, 0.0f, 250.0f), Vec3(400.0f, 200.0f, 500.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"SpearManAttackCol", Vec3(0.0f, 500.0f, 100.0f), Vec3(500.0f, 500.0f, 250.0f), LAYER_TYPE::MONSTER_ATTACK_COL);

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
	

	Ptr<CSound> Sound1 = Play_Sound(L"ChunkyDodgerThud1");
	Ptr<CSound> Sound2 = Play_Sound(L"ChunkyDodgerBell1",1,false, 0.05f);

	m_SoundTimer += fDT;

	if (0.9f < m_SoundTimer)
	{
		m_SoundTimer = 0.0f;
		Sound1->Stop();
		Sound2->Stop();
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

	if (IsGround == true)
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
			Play_Sound(L"ChunkyDodgerFirstAttackEnhance3", 1, true);
			Play_Sound(L"ChunkyDodgerAttackSwing2",1,true);
			Play_Sound(L"ChunkyDodgerBell5", 1, true);
			
			ChangeState(MONSTERSTATE::ATTACK, 0.1f, L"LeftCut");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftCutLink")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			Play_Sound(L"ChunkyDodgerSecondAttackEnhance3", 1, true);
			Play_Sound(L"ChunkyDodgerAttackSwing4", 1, true);
			Play_Sound(L"ChunkyDodgerBell4", 1, true);
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
		if (378 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(true);
		}

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

		if (380 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(false);
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
		if (448 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(true);
		}

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

		if (450 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(false);
		}

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_EvasionRange))
			{
				Play_Sound(L"ChunkyDodgerBell3", 1, true);
				Play_Sound(L"ChunkyDodgerJumpWoosh2", 1, true);
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

		if (145 == CurAni->GetFrameIdx())
		{
			PlaySound(L"ChunkyDodgerLand6", true);
		}

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_IsEvasion = true;
		
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
		}
	}
}

void CSpearManScript::Death()
{
	m_PaperBurnTime += fDT;

	const vector<CGameObject*>& childvec = GetObj()->GetChild();

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

void CSpearManScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_MonsterInfo.Hp <= 0)
		return;

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		Play_Sound(L"ChunkyDodgerTakeDamage2", 1, true);
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
			CAnimator3D* CurAni = Animator3D();
			CurAni->Animator3D()->StopAnimation();

			m_bDamaged = false;
			Play_Sound(L"ChunkyDodgerDeath3",1,true);
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else
		{
			CActorScript::OnCollisionEnter(_pOther);
		}

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
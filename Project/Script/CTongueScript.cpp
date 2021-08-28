#include "pch.h"
#include "CTongueScript.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"
#include "CTongueBullet.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>

void CTongueScript::awake()
{
	CMonsterScript::awake();
	CreateCol(L"TongueCol", Vec3(0.0f, 0.0f, 400.0f), Vec3(700.0f, 400.0f, 800.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"TongueAttackCol", Vec3(0.0f, 500.0f, 200.0f), Vec3(600.0f, 400.0f, 400.0f), LAYER_TYPE::MONSTER_ATTACK_COL);

	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CTongueScript::update()
{
	CMonsterScript::update();
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	switch (m_CurState)
	{
	case CMonsterScript::MONSTERSTATE::IDLE: Idle(); break;
	case CMonsterScript::MONSTERSTATE::MOVE: Move(); break;
	case CMonsterScript::MONSTERSTATE::CHASE: Chase(); break;
	case CMonsterScript::MONSTERSTATE::READY_ACTION: ReadyAction(); break;
	case CMonsterScript::MONSTERSTATE::ATTACK: Attack(); break;
	case CMonsterScript::MONSTERSTATE::FINISH_ACTION: FinishAction(); break;
	case CMonsterScript::MONSTERSTATE::SPECIAL_ACTION: SpecialAction(); break;
	case CMonsterScript::MONSTERSTATE::JUMP: Jump(); break;
	case CMonsterScript::MONSTERSTATE::DEATH: Death(); break;
	}
}



void CTongueScript::Idle()
{
	// 방어중에  플레이어의 위치에따라 같이 회전 
	// 방어 깨지면  공격

	if (false == RangeSearch(1000.0f))
	{
		ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
	}
}

void CTongueScript::Move()
{
	// 바로 플레이어 앞까지 간후 방어태세
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	Vec3 vMovePos = {};
	vDiff.Normalize();

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_ChaseSpeed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_ChaseSpeed;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

	MonsterRotateSystem(m_ChaseRotSpeed);

	if (RangeSearch(m_GuardRange))
	{

		ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"Guard");

		//ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
	}
}

void CTongueScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (1011 == CurAni->GetFrameIdx() || 166 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
	}

	if (1013 == CurAni->GetFrameIdx() || 168 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true && CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance")
	{
		if (false == RangeSearch(m_ChaseRange))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"SpinDown")
	{
		MonsterRotateSystem(3.0f);
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			ChangeState(MONSTERSTATE::FINISH_ACTION, 0.2f, L"SpinUp");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LongDistance")
	{
		if (273 <= CurAni->GetFrameIdx() && 320 >= CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_AttackRotSpeed);
		}
		if (338 == CurAni->GetFrameIdx())
		{
			LongDistanceAttack();
		}
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_BulletLimit = false;
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");

		}
		else if (false == RangeSearch(m_ChaseRange))
		{
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (1012 > CurAni->GetFrameIdx())
		{
			m_AttackDir = CPlayerScript::GetPlayerPos() - Transform()->GetLocalPos();
			MonsterRotateSystem(m_AttackRotSpeed);
		}
		if (1008 <= CurAni->GetFrameIdx() && 1016 >= CurAni->GetFrameIdx())
		{
			CalAttackDistance();
		}

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			MeleeAttack();

			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);

			if (0 == Pattern)
			{
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep2");
			}
			else if (1 == Pattern)
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
			}
			else if (2 == Pattern)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"Guard");
			}

		}

	}
}

void CTongueScript::FinishAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"SpinUp")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_BackStepRange))
			{
				ChangeState(MONSTERSTATE::JUMP, 0.2f, L"BackStep2");
			}
			else if (false == RangeSearch(m_BackStepRange) && true == RangeSearch(m_ChaseRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
			else if (false == RangeSearch(m_ChaseRange))
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
			}
		}
	}

}

void CTongueScript::SpecialAction()
{
	// 가드
	// 가드 유지
	// 가드 회전
	// 이동

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardBreak");
	}

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (534 == CurAni->GetFrameIdx())
	{
		m_ShieldPoint = 3;
		TransColPos(Vec3(0.0f, 300.0f, 200.0f), LAYER_TYPE::MONSTER_COL);
		TransColScale(Vec3(600.0f, 200.0f, 400.0f), LAYER_TYPE::MONSTER_COL);

		OnOffAttackCol(true);
		TransColPos(Vec3(0.0f, 300.0f, 200.0f), LAYER_TYPE::MONSTER_ATTACK_COL);
		TransColScale(Vec3(600.0f, 200.0f, 400.0f), LAYER_TYPE::MONSTER_ATTACK_COL);
	}

	if (539 == CurAni->GetFrameIdx())
	{
		TransColPos(Vec3(0.0f, 500.0f, 200.0f), LAYER_TYPE::MONSTER_ATTACK_COL);
		TransColScale(Vec3(600.0f, 400.0f, 400.0f), LAYER_TYPE::MONSTER_ATTACK_COL);
		OnOffAttackCol(false);
	}


	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vCross = relativePos.Cross(vFront);
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	float dot = vCross.Dot(vUP);
	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	wstring Temp = CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Guard")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_SpinCount = 0;
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay")
	{
		if (dist > 1.9f)
			ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");

		//플레이어는 내 왼쪽에 있다 
		else if (dot > 200.0)
		{
			++m_SpinCount;

			if (1 == m_SpinCount)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"LeftSpin");
			}

		}

		//플레이어는 내 오른쪽에 있다 
		else if (dot < -200.0)
		{
			++m_SpinCount;

			if (1 == m_SpinCount)
			{
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"RightSpin");
			}
		}

	}
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin")
	{

		m_SpinCount = 0;
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * 1.0f;
		Transform()->SetLocalRot(Rot);

		if (dot > -5.0 && dot < 5.0 && dist < 1.f)
		{
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}

	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
	{
		m_SpinCount = 0;
		Rot.y += CTimeMgr::GetInst()->GetfDT() * 1.0f;
		Transform()->SetLocalRot(Rot);
		if (dot > -5.0 && dot < 5.0 && dist < 1.f)
		{
			ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.2f, L"GuardStay");
		}
	}
	else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardBreak")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (RangeSearch(m_SpinDownRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"SpinDown");
			}
			else
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
		}
	}

}

void CTongueScript::Jump()
{
	Vec3 BackDir = -Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};

	vMovePos += fDT * BackDir * 1800.0f;

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"BackStep2")
	{
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (0 == Pattern)
			{
				ChangeState(MONSTERSTATE::MOVE, 0.2f, L"Chase");
			}
			else if (1 == Pattern)
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}
		}
	}
}

void CTongueScript::Death()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	CurAni->Animator3D()->StopAnimation();

	m_PaperBurnTime += fDT;

	vector<CGameObject*> childvec = GetObj()->GetChild();

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->MeshRender())
			childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_PaperBurnTime);

		if (childvec[i]->Collider3D())
			childvec[i]->Collider3D()->Activate(false);
	}

	if (1.0f < m_PaperBurnTime)
	{
		DeleteObject(GetGameObject());
	}
}

void CTongueScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		CAnimator3D* CurAni = Animator3D();
		UINT iCurClipIdx = CurAni->GetClipIdx();

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
		{
			--m_ShieldPoint;

			if (0 == m_ShieldPoint)
			{
				m_ShieldPoint = 3;
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.05f, L"GuardBreak");
			}
		}
		else
		{
			--m_MonsterInfo.Hp;
		}
	}

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		CAnimator3D* CurAni = Animator3D();
		UINT iCurClipIdx = CurAni->GetClipIdx();

		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Guard" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"GuardStay" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"LeftSpin" ||
			CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"RightSpin")
		{
			--m_ShieldPoint;

			if (0 == m_ShieldPoint)
			{
				m_ShieldPoint = 3;

				TransColPos(Vec3(0.0f, 0.0f, 400.0f), LAYER_TYPE::MONSTER_COL);
				TransColScale(Vec3(700.0f, 400.0f, 800.0f), LAYER_TYPE::MONSTER_COL);
				ChangeState(MONSTERSTATE::SPECIAL_ACTION, 0.05f, L"GuardBreak");
			}
		}
		else
		{
			--m_MonsterInfo.Hp;

			if (0 == m_MonsterInfo.Hp)
			{
				vector<CGameObject*> childvec = GetObj()->GetChild();

				for (int i = 0; i < childvec.size(); ++i)
				{
					if (childvec[i]->MeshRender())
					{
						UINT Count = childvec[i]->MeshRender()->GetMtrlCount();
						for (UINT j = 0; j < Count; ++j)
						{
							Ptr<CMaterial> mtrl = childvec[i]->MeshRender()->GetCloneMaterial(j);
							mtrl->SetData(SHADER_PARAM::TEX_4, m_PaperBurnTex.Get());
							childvec[i]->MeshRender()->SetMaterial(mtrl, j);
						}
					}
				}

				ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
			}
		}
	}
}

void CTongueScript::OnCollision(CGameObject* _pOther)
{
}

void CTongueScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CTongueScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CTongueBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CTongueBullet* Script = (CTongueBullet*)Obj->GetScript();

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}

void CTongueScript::MeleeAttack()
{
}

void CTongueScript::CalAttackDistance()
{
	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};
	m_AttackDir.Normalize();

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * 2400.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * 2400.0f;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

}

CTongueScript::CTongueScript()
	: m_SpinCount(0)
	, m_fTheta(0.0f)
	, m_AttackDir{}
	, m_BulletLimit(false)
	, m_ShieldPoint(3)
	, m_ChaseSpeed(650.0f)
	, m_GuardRange(400.0f)
	, m_BackStepRange(400.0f)
	, m_ChaseRange(1200.0f)
	, m_SpinDownRange(600.0f)
	, m_ChaseRotSpeed(5.0f)
	, m_AttackRotSpeed(2.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::TONGUESCRIPT;
	m_MonsterInfo.Hp = 8;
}

CTongueScript::~CTongueScript()
{
}

void CTongueScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CTongueScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
#include "pch.h"
#include "CSkullScript.h"
#include "CPlayerScript.h"
#include "CSkullBullet.h"

#include <Engine/CCollider3D.h>


void CSkullScript::awake()
{
	CMonsterScript::awake();
	CreateCol(L"SkullCol", Vec3(0.0f, 50.0f, 250.0f), Vec3(400.0f, 300.0f, 500.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"SkullAttackCol", Vec3(0.0f, 400.0f, 250.0f), Vec3(400.0f, 400.0f, 500.0f), LAYER_TYPE::MONSTER_ATTACK_COL);

	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CSkullScript::update()
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

void CSkullScript::OnCollisionEnter(CGameObject* _pOther)
{
	CActorScript::OnCollisionEnter(_pOther);

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
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
						Ptr<CMaterial> mtrl = childvec[i]->MeshRender()->GetSharedMaterial(j);
						mtrl->SetData(SHADER_PARAM::TEX_4, m_PaperBurnTex.Get());
					}
				}
			}
			CAnimator3D* CurAni = Animator3D();
			CurAni->Animator3D()->StopAnimation();

			m_bDamaged = false;
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}


	}

}

void CSkullScript::OnCollision(CGameObject* _pOther)
{
}

void CSkullScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CSkullScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CSkullBullet);

		Vec3 FirePos = Transform()->GetLocalPos();

		Vec3 OffsetPos = GetOffsetFirePos(FirePos, m_fFrontOffset, m_fUpOffset,1.f);
		Vec3 Right = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		OffsetPos += Right * 150.0f;

		Obj->Transform()->SetLocalPos(OffsetPos);
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CSkullBullet* Script = (CSkullBullet*)Obj->GetScript();

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}

void CSkullScript::MeleeAttack()
{
}

void CSkullScript::Idle()
{
	if (RangeSearch(2000.0f))
	{
		ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
	}
}

void CSkullScript::Chase()
{
	// 초기 상태
	// 플레이어가 공격 범위내에 들어온경우

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

	MonsterRotateSystem(m_RotSpeed);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (RangeSearch(m_MeleeAttackRange))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
		}
	}
}

void CSkullScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (260 == CurAni->GetFrameIdx() || 310 < CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
	}

	if (270 == CurAni->GetFrameIdx() || 320 < CurAni->GetFrameIdx())
	{
		OnOffAttackCol(false);
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (264 > CurAni->GetFrameIdx() || 274 < CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_RotSpeed);
		}

		if (264 <= CurAni->GetFrameIdx() && 266 >= CurAni->GetFrameIdx())
		{
			Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vDiff = vPlayerPos - vPos;
			vDiff.Normalize();

			Vec3 vMovePos = {};

			vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_AttackMoveSpeed;
			vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_AttackMoveSpeed;

			bool IsGround = GroundCheck(vPos + vMovePos);
			if (!IsGround)
				IsGround = ResearchNode(vPos + vMovePos);

			if (true == IsGround)
				Transform()->SetLocalPos(vPos + vMovePos);

		}

	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance")
	{
		// LongDistance 아닐때
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee" && RangeSearch(m_MeleeAttack2Range))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee2");
			}
			else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee2" && RangeSearch(m_MeleeAttackRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
			}
			else if (false == RangeSearch(m_MeleeAttackRange) && RangeSearch(m_LongDistanceAttackRange))
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");

			}
			else
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			}

		}
	}
	else
	{
		// LongDistance 일때
		// 한번 던지고 따라가면서 점점 거리를 좁힌다

		if (378 == CurAni->GetFrameIdx())
		{
			LongDistanceAttack();
		}


		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_BulletLimit = false;
			if (RangeSearch(m_MeleeAttackRange))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
			}
			else
			{
				ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Chase");
			}
		}
	}

}

void CSkullScript::Death()
{
	m_PaperBurnTime += fDT;

	vector<CGameObject*> childvec = GetObj()->GetChild();

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
		DeleteObject(GetGameObject());
	}
}

CSkullScript::CSkullScript()
	: m_BulletLimit(false)
	, m_ChaseSpeed(450.0f)
	, m_MeleeAttackRange(800.0f)
	, m_MeleeAttack2Range(1000.0f)
	, m_LongDistanceAttackRange(1800.0f)
	, m_AttackMoveSpeed(900.0f)
	, m_RotSpeed(5.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SKULLSCRIPT;
	m_MonsterInfo.Hp = 10;
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "FrontOffset", &m_fFrontOffset));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "UpOffset", &m_fUpOffset));
	m_fUpOffset = 200.0f;
}

CSkullScript::~CSkullScript()
{
}


void CSkullScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CSkullScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
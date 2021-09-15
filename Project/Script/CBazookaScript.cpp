#include "pch.h"
#include "CBazookaScript.h"

#include "CPlayerScript.h"
#include "CBazookaBullet.h"
#include <Engine/CCollider3D.h>
#include <Engine/CParticleSystem.h>

void CBazookaScript::awake()
{
	CMonsterScript::awake();

	CreateCol(L"BazookaCol", Vec3(0.0f, 0.0f, 300.0f), Vec3(300.0f, 300.0f, 500.0f), LAYER_TYPE::MONSTER_COL);
	CreateCol(L"BazookaAttackCol", Vec3(0.0f, 350.0f, 200.0f), Vec3(600.0f, 300.0f, 200.0f), LAYER_TYPE::MONSTER_ATTACK_COL);

	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CBazookaScript::update()
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

void CBazookaScript::Idle()
{
	if (RangeSearch(m_LongDistanceAttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}
void CBazookaScript::Chase()
{
	Ptr<CSound> Sound1 = Play_Sound(L"PlagueBoyStepVoiceA1");
	Ptr<CSound> Sound2 = Play_Sound(L"PlagueBoyStep4");

	m_SoundTimer += fDT;

	if (0.75f < m_SoundTimer)
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

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * m_Speed;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * m_Speed;

	bool IsGround = GroundCheck(vPos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(vPos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(vPos + vMovePos);

	MonsterRotateSystem(m_ChaseRotSpeed);
	// 플레이어 따라오게 
	if (RangeSearch(m_LongDistanceAttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
	}
}

void CBazookaScript::ReadyAction()
{

	bool Focus = false;

	if (MonsterRotateSystem(m_AimRotSpeed))
		Focus = true;


	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (76 == CurAni->GetFrameIdx())
	{
		PlaySound(L"PlagueBoyCharge1",1, true);
	}


	if (false == Focus && CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim", true);
	}

	// 만약 플레이어가 근접해 있다면 근접공격

	if (RangeSearch(m_MeleeAttackRange))
	{
		Play_Sound(L"PlagueBoyChargeSlam3", 1, true);
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		MeleeAttack();
	}

	m_AimTime += fDT;

	if (true == Focus && m_AimTime > 2.0f)
	{
		m_AimTime = 0.0f;

		if (true == RangeSearch(m_LongDistanceAttackRange))
		{
			Play_Sound(L"PlagueBoyFire1", 1, true);
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
			LongDistanceAttack();
		}
		else if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Move");
		}
	}


	// 일정시간동안 각도 계산
	// 플레이어가 멀어지면 사정거리 닿을때까지 이동

}




void CBazookaScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (201 > CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(m_AttackRotSpeed);
		}

		if (197 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(true);
		}

		if (203 == CurAni->GetFrameIdx())
		{
			OnOffAttackCol(false);
		}
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		if (RangeSearch(m_MeleeAttackRange))
		{
			Play_Sound(L"PlagueBoyChargeSlam3", 1, true);
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
		}
		else if (RangeSearch(m_LongDistanceAttackRange) && false == RangeSearch(m_MeleeAttackRange))
		{
			
			ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Aim");
		}
		else if (false == RangeSearch(m_LongDistanceAttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.2f, L"Move");
		}
	}

}

void CBazookaScript::Death()
{
	m_PaperBurnTime += fDT;

	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->MeshRender())
		{
			Vec4 BurnInfo = Vec4(1.0f, 0.f, 0.f, m_PaperBurnTime/3.f);
			int BurnType = (UINT)BURN_TYPE::MONSTER_BURN;

			EffectParamSetting(Vec4(10.f, 1.f, 1.f, 1.f), Vec4(0.01f, 0.005f, 0.005f, 1.f), m_RedTex);

			childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_1, &BurnType);
			childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &BurnInfo);
		}
			

		if (childvec[i]->Collider3D())
			childvec[i]->Collider3D()->Activate(false);
	}

	if (5.0f < m_PaperBurnTime)
	{
		DeleteObject(GetObj());
	}
}

void CBazookaScript::MeleeAttack()
{
}

void CBazookaScript::LongDistanceAttack()
{
	CGameObject* Obj = nullptr;

	Obj=IntanciatePrefab(L"BAZOOKA_BULLET", (UINT)LAYER_TYPE::MONSTER_EFFECT);

	Obj->ParticleSystem()->SetStartScale(Vec3(300.f, 300.f, 300.f));
	Obj->ParticleSystem()->SetPaperburnPTC(true);
	Vec3 FirePos = Transform()->GetLocalPos();

	Vec3 OffsetPos = GetOffsetFirePos(FirePos, m_fFrontOffset, m_fUpOffset ,m_fRightOffset);

	Obj->Transform()->SetLocalPos(OffsetPos);


	CBazookaBullet* Script = (CBazookaBullet*)Obj->GetScript();
	Script->SetActive(true);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

	CurScene->AddObject(Obj, LAYER_TYPE::MONSTER_BULLET_COL);



	CGameObject* Col = new CGameObject;
	Col->SetName(L"BazookBullet_Col");

	Col->AddComponent(new CTransform);
	Col->AddComponent(new CMeshRender);
	Col->AddComponent(new CCollider3D);

	Col->Transform()->SetLocalPos(Vec3(0.f,0.f,0.f));
	Col->Transform()->SetLocalScale(Vec3(200.f,200.f,200.f));

	Col->Collider3D()->SetParentOffsetPos(Vec3(0.f, 200.f, 0.f));

	Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CurScene->AddObject(Col, (UINT)LAYER_TYPE::MONSTER_BULLET_COL);


	Obj->AddChild(Col);

	Obj->awake();
}

void CBazookaScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_MonsterInfo.Hp <= 0)
		return;

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;


	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		Play_Sound(L"BruteTakeDamage3", 1, true);
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
			Play_Sound(L"PlagueBoyDeath", 1, true);
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else 
		{
			CActorScript::OnCollisionEnter(_pOther);
		}

	}

	
}

void CBazookaScript::OnCollision(CGameObject* _pOther)
{
}

void CBazookaScript::OnCollisionExit(CGameObject* _pOther)
{
}

CBazookaScript::CBazookaScript()
	: m_AimTime(0.0f)
	, m_LongDistanceAttackRange(2000.0f)
	, m_Speed(900.0f)
	, m_MeleeAttackRange(500.0f)
	, m_ChaseRotSpeed(5.0f)
	, m_AimRotSpeed(2.0f)
	, m_AttackRotSpeed(3.4f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BAZOOKASCRIPT;
	m_MonsterInfo.Hp = 20;

	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "FrontOffset", &m_fFrontOffset));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "UpOffset", &m_fUpOffset));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "RightOffset", &m_fRightOffset));

	m_fFrontOffset = 200.f;
	m_fUpOffset = 250.f;
	m_fRightOffset = -200.f;
}

CBazookaScript::~CBazookaScript()
{
}


void CBazookaScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CBazookaScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}

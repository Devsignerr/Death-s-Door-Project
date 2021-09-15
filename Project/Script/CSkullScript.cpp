#include "pch.h"
#include "CSkullScript.h"
#include "CPlayerScript.h"
#include "CSkullBullet.h"

#include <Engine/CParticleSystem.h>
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
	if (m_MonsterInfo.Hp <= 0)
		return;

	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
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
			Play_Sound(L"BruteDeath4");
			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else
		{
			CActorScript::OnCollisionEnter(_pOther);
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
		Play_Sound(L"Brute_ThrowLayer3", 1, true);

		m_BulletLimit = true;
		CGameObject* Obj = nullptr;

		Obj = IntanciatePrefab(L"SKULL_BULLET", (UINT)LAYER_TYPE::MONSTER_EFFECT);

		Obj->ParticleSystem()->SetStartScale(Vec3(300.f, 300.f, 300.f));
		Obj->ParticleSystem()->SetPaperburnPTC(true);

		Vec3 FirePos = Transform()->GetLocalPos();
		Vec3 OffsetPos = GetOffsetFirePos(FirePos, m_fFrontOffset, m_fUpOffset, m_fRightOffset);

		Obj->Transform()->SetLocalPos(OffsetPos);

		CSkullBullet* Script = (CSkullBullet*)Obj->GetScript();
		Script->SetActive(true);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, LAYER_TYPE::MONSTER_EFFECT);



		CGameObject* Col = new CGameObject;
		Col->SetName(L"SkullBullet_Col");

		Col->AddComponent(new CTransform);
		Col->AddComponent(new CMeshRender);
		Col->AddComponent(new CCollider3D);

		Col->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		Col->Transform()->SetLocalScale(Vec3(200.f, 200.f, 200.f));

		Col->Collider3D()->SetParentOffsetPos(Vec3(0.f, 200.f, 0.f));

		Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		CurScene->AddObject(Col, (UINT)LAYER_TYPE::MONSTER_BULLET_COL);

		Obj->AddChild(Col);

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

	Ptr<CSound> Sound =  Play_Sound(L"BruteStep3", 1, false, 0.2f);

	m_SoundTimer += fDT;

	if (0.75f < m_SoundTimer)
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

	MonsterRotateSystem(m_RotSpeed);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (RangeSearch(m_MeleeAttackRange))
	{
		Play_Sound(L"BrutePrepAttackVoice2",1,true);
		Play_Sound(L"BrutePrepAttack2", 1, true);
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

	if (260 == CurAni->GetFrameIdx() )
	{
		PlaySound(L"BruteSwing1",true,0.5f);
		OnOffAttackCol(true);
	}

	else if (317 == CurAni->GetFrameIdx())
	{
		PlaySound(L"BruteSlam3", true, 0.5f);
		OnOffAttackCol(true);
	}

	if (270 == CurAni->GetFrameIdx() || 317 < CurAni->GetFrameIdx())
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
				Play_Sound(L"BrutePrepSlamVoice2", 1, true);
				Play_Sound(L"BrutePrepAttack2", 1, true);
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee2");
			}
			else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee2" && RangeSearch(m_MeleeAttackRange))
			{
				Play_Sound(L"BrutePrepAttackVoice2", 1, true);
				Play_Sound(L"BrutePrepAttack2", 1, true);
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
				Play_Sound(L"BrutePrepAttackVoice2", 1, true);
				Play_Sound(L"BrutePrepAttack2", 1, true);
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
		DeleteObject(GetGameObject());
	}
}

CSkullScript::CSkullScript()
	: m_BulletLimit(false)
	, m_ChaseSpeed(450.0f)
	, m_MeleeAttackRange(800.0f)
	, m_MeleeAttack2Range(1000.0f)
	, m_LongDistanceAttackRange(1400.0f)
	, m_AttackMoveSpeed(900.0f)
	, m_RotSpeed(5.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SKULLSCRIPT;
	m_MonsterInfo.Hp = 10;
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "FrontOffset", &m_fFrontOffset));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "UpOffset", &m_fUpOffset));
	m_fUpOffset = 200.0f;
	m_fFrontOffset = 0.f;
	m_fRightOffset = 150.f;
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
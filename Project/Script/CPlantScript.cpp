#include "pch.h"
#include "CPlantScript.h"
#include "CPlayerScript.h"
#include "CPlantBullet.h"

#include <Engine/CCollider3D.h>
#include "CCrowEggBullet.h"
#include <Engine/CLayer.h>

void CPlantScript::awake()
{
	CActorScript::awake();
	CreateCol();

	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CPlantScript::update()
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

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		Test();
	}

}

void CPlantScript::Idle()
{
	MonsterRotateSystem(m_RotSpeed);

	m_AttackDelayTime += CTimeMgr::GetInst()->GetfDT();

	if (m_AttackDelayTime > 0.75f)
	{
		m_AttackDelayTime = 0.0f;

		if (RangeSearch(m_AttackRange))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
		}

	}
}

void CPlantScript::Attack()
{
	// �ִϸ��̼� ������ ���Ÿ� ���� ������Ʈ ���� ��
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (89 == CurAni->GetFrameIdx())
	{
		LongDistanceAttack();
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		m_BulletLimit = false;
		ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	}
}


void CPlantScript::Death()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (180 <= CurAni->GetFrameIdx())
	{
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
	}

	if (1.0f < m_PaperBurnTime)
	{
		DeleteObject(GetGameObject());
	}
}

void CPlantScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 Pos = Transform()->GetLocalPos();

		Vec3 Dir = -Transform()->GetLocalDir(DIR_TYPE::FRONT);

		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);
		Obj->AddComponent(new CPlantBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CPlantBullet* Script = (CPlantBullet*)Obj->GetScript();
		Script->awake();
		Script->SetBulletDir(Dir);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MONSTER_BULLET_COL);
	}
}

void CPlantScript::CreateCol()
{
	{
		Vec3 Pos = Transform()->GetLocalPos();

		CGameObject* Obj = new CGameObject;
		Obj->SetName(L"PlantCol");

		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);

		Obj->Transform()->SetLocalPos(Vec3(0.0f, 2.0f, 0.0f));
		Obj->Transform()->SetLocalScale(Vec3(2.0f, 4.0f, 2.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MONSTER_COL);

		AddChild(GetObj(), Obj);
	}
}

void CPlantScript::Test()
{
	Vec3 Pos = Transform()->GetLocalPos();

	CGameObject* Obj = new CGameObject;
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CCrowEggBullet);
	Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());

	CCrowEggBullet* Script = (CCrowEggBullet*)Obj->GetScript();
	Script->SetStartPos(Pos);
	Script->SetRange(1000.0f);
	Script->awake();

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::INDETERMINATE);
}

void CPlantScript::OnCollisionEnter(CGameObject* _pOther)
{
	// �÷��̾��� ������ �������
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

void CPlantScript::OnCollision(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;
	if ((UINT)LAYER_TYPE::INDETERMINATE == Obj->GetLayerIndex())
	{
		vector<CGameObject*> Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

		CCrowEggBullet* Script = nullptr;

		for (size_t i = 0; i < Temp.size(); ++i)
		{
			Script = (CCrowEggBullet*)Temp[i]->GetScript();
			if (Script)
			{
				if (false == Script->GetHead())
					Script->FindToDeadCheck(Obj);
				break;
			}
		}
	}
}

void CPlantScript::OnCollisionExit(CGameObject* _pOther)
{
}


CPlantScript::CPlantScript()
	: m_BulletLimit(false)
	, m_AttackDelayTime(0.0f)
	, m_RotSpeed(5.0f)
	, m_AttackRange(1500.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::PLANTSCRIPT;
	m_MonsterInfo.Hp = 3;
}

CPlantScript::~CPlantScript()
{
}

void CPlantScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CPlantScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
#include "pch.h"
#include "CPlantScript.h"
#include "CPlayerScript.h"
#include "CPlantBullet.h"

void CPlantScript::awake()
{
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

	if (0 == m_MonsterInfo.Hp)
	{
		ChangeState(MONSTERSTATE::DEATH,0.03f,L"Death",true);
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
	// 애니메이션 끝나면 원거리 공격 오브젝트 생성 후
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
		Obj->AddComponent(new CPlantBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CPlantBullet* Script = (CPlantBullet*)Obj->GetScript();
		Script->SetBulletDir(Dir);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 10);
	}
}

void CPlantScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	CGameObject* Obj = _pOther;

	if (11 == Obj->GetLayerIndex())
	{
		--m_MonsterInfo.Hp;
	}


}

void CPlantScript::OnCollision(CGameObject* _pOther)
{
}

void CPlantScript::OnCollisionExit(CGameObject* _pOther)
{
}


CPlantScript::CPlantScript()
	: m_BulletLimit(false)
	, m_AttackDelayTime(0.0f)
	, m_RotSpeed(5.0f)
	, m_AttackRange(1000.0f)
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
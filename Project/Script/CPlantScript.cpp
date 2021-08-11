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

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		

	}

}

void CPlantScript::Idle()
{
	
	MonsterRotateSystem(2.0f);


	m_MonsterInfo.DelayNextActionTime -= CTimeMgr::GetInst()->GetfDT();
	if (0.0f >= m_MonsterInfo.DelayNextActionTime)
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Attack");
		m_MonsterInfo.DelayNextActionTime = 0.75f;
	
	}
}

void CPlantScript::Attack()
{
	// 애니메이션 끝나면 원거리 공격 오브젝트 생성 후
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (89 == CurAni->GetFrameIdx())
	{
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 Pos = Transform()->GetLocalPos();

		Vec3 Distance = PlayerPos - Pos;

		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CPlantBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CPlantBullet* Script = (CPlantBullet*)Obj->GetScript();
		Script->SetBulletDir(Distance);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 10);
	}
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	}
}


void CPlantScript::Death()
{

}

void CPlantScript::OnCollisionEnter(CGameObject* _pOther)
{
	// 플레이어의 공격을 받은경우
	--m_MonsterInfo.Hp;

	if (0 == m_MonsterInfo.Hp)
	{
		ChangeState(MONSTERSTATE::DEATH);
	}
}

void CPlantScript::OnCollision(CGameObject* _pOther)
{
}

void CPlantScript::OnCollisionExit(CGameObject* _pOther)
{
}


CPlantScript::CPlantScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::PLANTSCRIPT;
	m_MonsterInfo.Hp = 3;
	m_MonsterInfo.Speed = 6.0f;
	m_MonsterInfo.RecognitionRange = 700.0f;
	m_MonsterInfo.DelayNextActionTime = 0.75f;
		
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
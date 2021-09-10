#include "pch.h"
#include "CBatScript.h"

#include "CPlayerScript.h"
#include <Engine/CCollider3D.h>

void CBatScript::awake()
{
	CActorScript::awake();
	CreateCol();
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
}

void CBatScript::update()
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

void CBatScript::Idle()
{
	// 플레이어가 범위 안에 들어올때까지 빙빙 돈다
	// 범위 안에들어오면 인식했다는 모션 있다

	Vec3 vBatRot = Transform()->GetLocalRot();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};

	vMovePos += CTimeMgr::GetInst()->GetfDT() * (Transform()->GetLocalDir(DIR_TYPE::UP) / m_fRotRange) * m_Speed;

	vBatRot.y += m_fDegreeToRot * fDT;
	Transform()->SetLocalRot(vBatRot);

	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

	if (RangeSearch(m_AttackRange))
	{
		ChangeState(MONSTERSTATE::READY_ACTION, 0.2f, L"Recognize");
	}
}


void CBatScript::Chase()
{
	CalChaseMove();

	if (RangeSearch(m_AttackRange))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.05f, L"Attack");
	}
}

void CBatScript::ReadyAction()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		ChangeState(MONSTERSTATE::ATTACK, 0.1f, L"Attack");
	}
}

void CBatScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (80 > CurAni->GetFrameIdx())
	{
		m_fTheta = -XM_PI / 2.f;
		m_AttackDir = Transform()->GetLocalDir(DIR_TYPE::UP);
		MonsterRotateSystem(5.0f);
	}

	if (81 < CurAni->GetFrameIdx())
	{

		CalAttackDistance();
	}

	if (85 == CurAni->GetFrameIdx())
	{
		OnOffAttackCol(true);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		OnOffAttackCol(false);

		// 범위 밖이라면
		if (false == RangeSearch(m_AttackRange))
		{
			ChangeState(MONSTERSTATE::CHASE, 0.05f, L"Chase");
		}
	}
}

void CBatScript::Death()
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
		DeleteObject(GetObj());
	}
}

void CBatScript::OnCollisionEnter(CGameObject* _pOther)
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
						//childvec[i]->MeshRender()->SetMaterial(mtrl, j);
					}
				}
			}

			CAnimator3D* CurAni = Animator3D();
			CurAni->Animator3D()->StopAnimation();
			m_bDamaged = false;

			ChangeState(MONSTERSTATE::DEATH, 0.03f, L"Death", true);
		}
		else {
			CActorScript::OnCollisionEnter(_pOther);
		}

		
	}
}

void CBatScript::OnCollision(CGameObject* _pOther)
{
}

void CBatScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CBatScript::CalAttackDistance()
{
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 vMovePos = {};
	float Speed = (sinf(m_fTheta) + 1.f) * 5.0f;

	vMovePos.x += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.x * Speed * 7000.0f;
	vMovePos.z += CTimeMgr::GetInst()->GetfDT() * m_AttackDir.z * Speed * 7000.0f;
	m_fTheta += CTimeMgr::GetInst()->GetfDT() * XM_PI / 2.0f;

	if ((XM_PI * 3.0f / 2.0f) < m_fTheta)
	{
		m_fTheta = -XM_PI / 2.f;
	}


	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

}

void CBatScript::CalChaseMove()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	// 좌우로 구불구불하게 이동

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();
	Vec3 vRelative = PlayerPos - Pos;
	Vec3 vMovePos = {};
	vRelative.Normalize();

	m_fTimetoCheckPlayerPos += fDT;

	if (m_fTimetoCheckPlayerPos > 0.8f)
	{
		//사이각 
		m_fInternalRadianWithPlayer = vFront.Dot(vRelative);

		Vec3 vCross = vFront.Cross(vRelative);

		float fDot = vCross.Dot(vUp);

		//플레이어는 내 왼쪽에 있다 
		if (fDot > 0.0f)
			m_bPlayerMyLeft = true;

		//플레이어는 내 오른쪽에 있다 
		else if (fDot < 0.0f)
			m_bPlayerMyLeft = false;

		m_fTimetoCheckPlayerPos = 0.f;
	}

	if (m_bPlayerMyLeft)
		Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
	else
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);

	vMovePos += (vFront / 0.95f) * fDT * 500.f;


	bool IsGround = GroundCheck(Pos + vMovePos);
	if (!IsGround)
		IsGround = ResearchNode(Pos + vMovePos);

	if (true == IsGround)
		Transform()->SetLocalPos(Pos + vMovePos);

	Transform()->SetLocalRot(Rot);

}

void CBatScript::CreateCol()
{
	{
		Vec3 Pos = Transform()->GetLocalPos();

		CGameObject* Obj = new CGameObject;
		Obj->SetName(L"BatCol");

		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);

		Obj->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 150.0f));
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 300.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		//Obj->MeshRender()->Activate(false);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MONSTER_COL);

		GetObj()->AddChild(Obj);

		Obj->MeshRender()->Activate(false);
	}

	{
		Vec3 Pos = Transform()->GetLocalPos();

		CGameObject* Obj = new CGameObject;
		Obj->SetName(L"BatAttackCol");

		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);

		Obj->Transform()->SetLocalPos(Vec3(0.0f, 150.0f, 100.0f));
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 200.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		Obj->Collider3D()->SetParentOffsetPos(Vec3(0.0f, 150.0f, 100.0f));

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MONSTER_ATTACK_COL);

		//Obj->MeshRender()->Activate(false);

		GetObj()->AddChild(Obj);

		Obj->MeshRender()->Activate(false);
		Obj->Collider3D()->Activate(false);
	}
}



CBatScript::CBatScript()
	: m_fDegreeToRot(1.0f)
	, m_fRotRange(1.0f)
	, m_AttackDir{}
	, m_fTheta(-XM_PI / 2.f)
	, m_fTimetoCheckPlayerPos(0.f)
	, m_fInternalRadianWithPlayer(0.f)
	, m_AttackRange(700.0f)
	, m_Speed(800.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::BATSCRIPT;
	m_MonsterInfo.Hp = 1;
}

CBatScript::~CBatScript()
{
}


void CBatScript::SaveToScene(FILE* _pFile)
{
	CMonsterScript::SaveToScene(_pFile);
}

void CBatScript::LoadFromScene(FILE* _pFile)
{
	CMonsterScript::LoadFromScene(_pFile);
}
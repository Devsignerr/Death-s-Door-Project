#include "pch.h"
#include "CSkullScript.h"
#include "CPlayerScript.h"
#include "CSkullBullet.h"


void CSkullScript::awake()
{
	ChangeState(MONSTERSTATE::IDLE, 0.2f, L"Idle");
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = -XM_PI / 2.0f;
	Transform()->SetLocalRot(vRot);
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

void CSkullScript::LongDistanceAttack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CSkullBullet);

		Obj->Transform()->SetLocalPos(Transform()->GetLocalPos());
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

void CSkullScript::Move()
{
	
}

void CSkullScript::Chase()
{
	// �ʱ� ����
	// �÷��̾ ���� �������� ���°��

	Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vDiff = vPlayerPos - vPos;
	vDiff.Normalize();

	vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 450.0f;
	vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 450.0f;

	MonsterRotateSystem(5.0f);
	Transform()->SetLocalPos(vPos);

	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	if (RangeSearch(800.0f))
	{
		ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == RangeSearch(2000.0f))
		{
			ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"LongDistance");
		}
	}
}

void CSkullScript::ReadyAction()
{
	// �÷��̾ �������� �������� ���°�� 
	// �����ڷ� 2�� ����
	// 1�� �����ڿ� �÷��̾ ������ ����� �ļ�Ÿ�� �̾����� �ʴ´�
	// �÷��̾�� �Ÿ��� �ʹ� ��������� ���Ÿ� ����

}

void CSkullScript::Attack()
{
	CAnimator3D* CurAni = Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee")
	{
		if (264 > CurAni->GetFrameIdx() || 274 < CurAni->GetFrameIdx())
		{
			MonsterRotateSystem(5.0f);
		}

		if (264 <= CurAni->GetFrameIdx() && 266 >= CurAni->GetFrameIdx())
		{
			Vec3 vPlayerPos = CPlayerScript::GetPlayerPos();
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vDiff = vPlayerPos - vPos;
			vDiff.Normalize();

			vPos.x += CTimeMgr::GetInst()->GetfDT() * vDiff.x * 900.0f;
			vPos.z += CTimeMgr::GetInst()->GetfDT() * vDiff.z * 900.0f;

			Transform()->SetLocalPos(vPos);
		}

	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName != L"LongDistance" )
	{
		// LongDistance �ƴҶ�
		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee" && RangeSearch(1000.0f))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee2");
			}
			else if (CurAni->GetMTAnimClip()->at(iCurClipIdx).strAnimName == L"Melee2" && RangeSearch(800.0f))
			{
				ChangeState(MONSTERSTATE::ATTACK, 0.2f, L"Melee");
			}
			else if (false == RangeSearch(800.0f) && RangeSearch(2000.0f))
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
		// LongDistance �϶�
		// �ѹ� ������ ���󰡸鼭 ���� �Ÿ��� ������
		
		if (378 == CurAni->GetFrameIdx())
		{
			LongDistanceAttack();
		}


		if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
		{
			m_BulletLimit = false;
			if (RangeSearch(800.0f))
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

void CSkullScript::FinishAction()
{
}

void CSkullScript::Jump()
{
}

void CSkullScript::Death()
{
}

CSkullScript::CSkullScript()
	: m_BulletLimit(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::SKULLSCRIPT;
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
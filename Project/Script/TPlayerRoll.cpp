#include "pch.h"
#include "TPlayerRoll.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>

void TPlayerRoll::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (KEY_TAP(KEY_TYPE::MBTN))
	{
		m_IsRollSlash = true;
	}

	Vec3 PlayerFront = ((CPlayerScript*)GetScript())->GetPlayerFront();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 vMovePos = {  };

	vMovePos.x += PlayerFront.x * fDT * 2000.0f;
	vMovePos.z += PlayerFront.z * fDT * 2000.0f;

	vMovePos += CPlayerScript::GetOtherPower();

	bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(Pos + vMovePos);

	if (!IsGround)
		IsGround = ((CPlayerScript*)GetScript())->ResearchNode(Pos + vMovePos);

	if (true == IsGround)
	{


		GetObj()->Transform()->SetLocalPos(Pos + vMovePos);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (m_IsRollSlash)
		{
			m_Script->SetState(PLAYER_STATE::ROLL_SLASH);
			GetFSM()->ChangeState(L"Roll_Slash", 0.03f, L"Roll_Slash", false);
		}
		else
		{ 
			m_Script->SetState(PLAYER_STATE::IDLE);
			GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
		}
	}
}

void TPlayerRoll::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CPlayerScript*)GetScript();

	GetScript()->Play_Sound(L"NewRoll1", 1, true, 0.3f);

	m_Script->SetState(PLAYER_STATE::ROLL);

	CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
	Obj->Collider3D()->Activate(false);

	//CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerCol",(UINT)LAYER_TYPE::PLAYER_COL);
	//Obj->Collider3D()->SetParentOffsetPos(Vec3(0.0f, 0.0f, 0.5f));
	//Obj->MeshRender()->Activate(false);
	//Obj->Collider3D()->Activate(false);
}

void TPlayerRoll::Exit()
{
	//CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerCol", (UINT)LAYER_TYPE::PLAYER_COL);
	//Obj->MeshRender()->Activate(true);
	//Obj->Collider3D()->Activate(true);
	m_IsRollSlash = false;
}

TPlayerRoll::TPlayerRoll()
	: m_IsRollSlash(false)
	, m_Script(nullptr)
{
}

TPlayerRoll::~TPlayerRoll()
{
}

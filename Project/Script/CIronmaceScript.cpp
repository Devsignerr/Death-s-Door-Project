#include "pch.h"
#include "CIronmaceScript.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CState.h>

#include "CPlayerScript.h"

#pragma region IRonStateHeader

#include "TIronChopAttack.h"
#include "TIronChopAttackEnd.h"
#include "TIronChopAttackCombo.h"
#include "TIronChopAttackComboEnd.h"
#include "TIronCutScene.h"
#include "TIronDeath.h"
#include "TIronJumpAttack.h"
#include "TIronJumpAttack2.h"
#include "TIronJumpFinish.h"
#include "TIronJumpReady.h"
#include "TIronRunAttack.h"
#include "TIronSpinAttack.h"
#include "TIronSpinAttackCombo.h"
#include "TIronWalk.h"

#pragma endregion


void CIronmaceScript::ChangeState(IRONMACE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	m_eState = _eState;

	wstring StateName = {};

	//STATE 따라 이름 바꿔줘야 함 
	map<IRONMACE_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
		StateName = iter->second;
	else
		assert(0);

	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}


void CIronmaceScript::awake()
{
	CBossScript::awake();
	CreateCol(L"IronMaceCol", Vec3(0.0f, 0.0f, 600.0f), Vec3(600.0f, 500.0f, 1200.0f), LAYER_TYPE::BOSS_COL);
	CreateCol(L"IronMaceAttackCol", Vec3(-100.0f, 800.0f, 150.0f), Vec3(500.0f, 1100.0f, 300.0f), LAYER_TYPE::BOSS_ATTACK_COL);

	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;
	m_pFSM->SetObject(GetObj());

	TIronCutScene* CutSceneState = new TIronCutScene;							//여기에서 스테이트 생성 , FSM에 추가시킴 
	m_pFSM->AddState(L"CutScene", CutSceneState);								// FSM에 상태를 추가한다 
	m_mapState.insert(make_pair(IRONMACE_STATE::CUTSCENE, L"CutScene"));		//내 상태와 , 상태이름을 맵으로 저장한다 

	TIronWalk* WalkState = new TIronWalk;
	m_pFSM->AddState(L"Walk", WalkState);
	m_mapState.insert(make_pair(IRONMACE_STATE::WALK, L"Walk"));

	TIronRunAttack* RunAttackState = new TIronRunAttack;
	m_pFSM->AddState(L"RunAttack", RunAttackState);
	m_mapState.insert(make_pair(IRONMACE_STATE::RUN_ATTACK, L"RunAttack"));

	TIronChopAttack* ChopAttackState = new TIronChopAttack;
	m_pFSM->AddState(L"ChopAttack", ChopAttackState);
	m_mapState.insert(make_pair(IRONMACE_STATE::CHOPATTACK, L"ChopAttack"));

	TIronChopAttackEnd* ChopAttackEndState = new TIronChopAttackEnd;
	m_pFSM->AddState(L"ChopAttackEnd", ChopAttackEndState);
	m_mapState.insert(make_pair(IRONMACE_STATE::CHOPATTACKEND, L"ChopAttackEnd"));

	TIronChopAttackCombo* ChopAttackComboState = new TIronChopAttackCombo;
	m_pFSM->AddState(L"ChopAttackCombo", ChopAttackComboState);
	m_mapState.insert(make_pair(IRONMACE_STATE::CHOPATTACKCOMBO, L"ChopAttackCombo"));

	TIronChopAttackComboEnd* ChopAttackComboEndState = new TIronChopAttackComboEnd;
	m_pFSM->AddState(L"ChopAttackComboEnd", ChopAttackComboEndState);
	m_mapState.insert(make_pair(IRONMACE_STATE::CHOPATTACKCOMBOEND, L"ChopAttackComboEnd"));

	TIronJumpAttack2* JumpAttack2State = new TIronJumpAttack2;
	m_pFSM->AddState(L"JumpAttack2", JumpAttack2State);
	m_mapState.insert(make_pair(IRONMACE_STATE::JUMPATTACK2, L"JumpAttack2"));

	TIronSpinAttack* SpinAttackState = new TIronSpinAttack;
	m_pFSM->AddState(L"SpinAttack", SpinAttackState);
	m_mapState.insert(make_pair(IRONMACE_STATE::SPINATTACK, L"SpinAttack"));

	TIronSpinAttackCombo* SpinAttackComboState = new TIronSpinAttackCombo;
	m_pFSM->AddState(L"SpinAttackCombo", SpinAttackComboState);
	m_mapState.insert(make_pair(IRONMACE_STATE::SPINATTACKCOMBO, L"SpinAttackCombo"));

	TIronDeath* DeathState = new TIronDeath;
	m_pFSM->AddState(L"Death", DeathState);
	m_mapState.insert(make_pair(IRONMACE_STATE::DEATH, L"Death"));

	ChangeState(IRONMACE_STATE::CUTSCENE, 0.04f, L"CutScene");
}

void CIronmaceScript::update()
{
	CBossScript::update();
	m_pFSM->update();


}

void CIronmaceScript::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_Hp <= 0)
		return;

	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		--m_Hp;

		if (0 == m_Hp)
		{
			m_pFSM->ChangeState(L"Death", 0.04f, L"Death", true);
		}

		else
		{
			CActorScript::OnCollisionEnter(_pOther);
		}
	}
}

void CIronmaceScript::OnCollision(CGameObject* _pOther)
{
}

void CIronmaceScript::OnCollisionExit(CGameObject* _pOther)
{
}

CIronmaceScript::CIronmaceScript()
	: m_pFSM(nullptr)
	, m_eState(IRONMACE_STATE::END)
	, m_Hp(15)
{
	m_iScriptType = (int)SCRIPT_TYPE::IRONMACESCRIPT;
}

CIronmaceScript::~CIronmaceScript()
{
	SAFE_DELETE(m_pFSM);
}

void CIronmaceScript::SaveToScene(FILE* _pFile)
{
	CBossScript::SaveToScene(_pFile);
}

void CIronmaceScript::LoadFromScene(FILE* _pFile)
{
	CBossScript::LoadFromScene(_pFile);
}
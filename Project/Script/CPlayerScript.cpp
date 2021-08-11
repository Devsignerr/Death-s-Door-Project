#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CState.h>
#include "TPlayerIdle.h"
#include "TPlayerRun.h"

#include <Script/CCameraScript.h>
Vec3 CPlayerScript::PlayerPos = {};
Vec3 CPlayerScript::vPlayerFront = {};


CPlayerScript::CPlayerScript():
	m_eState(PLAYER_STATE::IDLE),
	m_fRotationSpeed(10.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::PLAYERSCRIPT;
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_pFSM)
	{
		delete m_pFSM;
		m_pFSM = nullptr;
	}
}

void CPlayerScript::awake()
{
	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;			  //FSM ��ü ���� 
	m_pFSM->SetObject(GetObj());  //FSM�� Ÿ��Object��  �˷��� 

																
	TPlayerIdle* pIdleState = new TPlayerIdle;				   //���⿡�� ������Ʈ ���� , FSM�� �߰���Ŵ 
	m_pFSM->AddState(L"Idle", pIdleState);					   // FSM�� ���¸� �߰��Ѵ� 
	m_mapState.insert(make_pair(PLAYER_STATE::IDLE, L"Idle")); //�� ���¿� , �����̸��� ������ �����Ѵ� 
	
	TPlayerRun* pRunState = new TPlayerRun;
	m_pFSM->AddState(L"Run", pRunState);
	m_mapState.insert(make_pair(PLAYER_STATE::RUN, L"Run"));

	ChangeState(PLAYER_STATE::IDLE, 0.2f, L"Idle", false);
}

void CPlayerScript::start()
{
}

void CPlayerScript::update()
{
	m_pFSM->update();
}

void CPlayerScript::lateupdate()
{
	m_pFSM->lateupdate();
}

void CPlayerScript::ChangeState(PLAYER_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	//FSM�� Script�� ������ State�� ������ 
	m_eState = _eState;

	//STATE Ÿ���� Key�� ���� �̸��� �����´� 
	wstring StateName = {}; //m_mapState.at(_eState);

	//STATE ���� �̸� �ٲ���� �� 
	map<PLAYER_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
	{
		StateName = iter->second;
	}
	else 
	{
		assert(0);
	}


	//FSM �� ���º��� ����� ������ 
	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}


void CPlayerScript::KeyInput()
{
	//if (KEY_TAP(KEY_TYPE::LBTN))
	//{
	//	ChangeState(PLAYER_STATE::RUN, 0.2f, L"Run", false);
	//}
	//
	//if (KEY_TAP(KEY_TYPE::RBTN))
	//{
	//	ChangeState(PLAYER_STATE::IDLE, 0.2f, L"Idle", false);
	//}
}

void CPlayerScript::OnCollisionEnter(CGameObject* _pOther)
{
	
}

void CPlayerScript::OnCollision(CGameObject* _pOther)
{
}

void CPlayerScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}

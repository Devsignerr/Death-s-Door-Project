#include "pch.h"
#include "CCrowScript.h"
#include "CCrowBullet.h"
#include "CCrowBatBullet.h"
#include "CCrowEggBullet.h"


#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>

#pragma region CrowStateHeader

#include "TCrowBatBullet.h"
#include "TCrowCutScene.h"
#include "TCrowDeath.h"
#include "TCrowGuidedBullet.h"
#include "TCrowIdle.h"
#include "TCrowJump.h"
#include "TCrowLeftSpin.h"
#include "TCrowRightSpin.h"
#include "TCrowSliding.h"
#include "TCrowSlidingReady.h"
#include "TCrowSpitting.h"
#include "TCrowStomp.h"
#include "TCrowWalk.h"
#include "TCrowRun.h"
#include "TCrowStandingDeath.h"
#include "TCrowSlidingRot.h"

#pragma endregion


void CCrowScript::ChangeState(CROW_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	m_eState = _eState;

	wstring StateName = {};

	//STATE ���� �̸� �ٲ���� �� 
	map<CROW_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
		StateName = iter->second;
	else
		assert(0);

	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}

void CCrowScript::awake()
{
	CBossScript::awake();
	CreateCol(L"CrowBossCol", Vec3(0.0f, 50.0f, 100.0f), Vec3(150.0f, 250.0f, 100.0f), LAYER_TYPE::BOSS_COL);

	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;
	m_pFSM->SetObject(GetObj());

	TCrowCutScene* CutSceneState = new TCrowCutScene;							//���⿡�� ������Ʈ ���� , FSM�� �߰���Ŵ 
	m_pFSM->AddState(L"CutScene", CutSceneState);								// FSM�� ���¸� �߰��Ѵ� 
	m_mapState.insert(make_pair(CROW_STATE::CUTSCENE, L"CutScene"));			//�� ���¿� , �����̸��� ������ �����Ѵ� 

	TCrowRun* RunState = new TCrowRun;
	m_pFSM->AddState(L"Run", RunState);
	m_mapState.insert(make_pair(CROW_STATE::RUN, L"Run"));

	TCrowStandingDeath* StandingDeathState = new TCrowStandingDeath;
	m_pFSM->AddState(L"StandingDeath", StandingDeathState);
	m_mapState.insert(make_pair(CROW_STATE::STANDINGDEATH, L"StandingDeath"));

	TCrowWalk* WalkState = new TCrowWalk;
	m_pFSM->AddState(L"Walk", WalkState);
	m_mapState.insert(make_pair(CROW_STATE::WALK, L"Walk"));

	TCrowStomp* StompState = new TCrowStomp;
	m_pFSM->AddState(L"Stomp", StompState);
	m_mapState.insert(make_pair(CROW_STATE::STOMP, L"Stomp"));

	TCrowSpitting* SpittingState = new TCrowSpitting;
	m_pFSM->AddState(L"Spitting", SpittingState);
	m_mapState.insert(make_pair(CROW_STATE::SPITTING, L"Spitting"));

	TCrowSlidingReady* SlidingReadyState = new TCrowSlidingReady;
	m_pFSM->AddState(L"SlidingReady", SlidingReadyState);
	m_mapState.insert(make_pair(CROW_STATE::SLIDINGREADY, L"SlidingReady"));

	TCrowSliding* SlidingState = new TCrowSliding;
	m_pFSM->AddState(L"Sliding", SlidingState);
	m_mapState.insert(make_pair(CROW_STATE::SLIDING, L"Sliding"));

	TCrowRightSpin* RightSpinState = new TCrowRightSpin;
	m_pFSM->AddState(L"RightSpin", RightSpinState);
	m_mapState.insert(make_pair(CROW_STATE::RIGHTSPIN, L"RightSpin"));

	TCrowLeftSpin* LeftSpinState = new TCrowLeftSpin;
	m_pFSM->AddState(L"LeftSpin", LeftSpinState);
	m_mapState.insert(make_pair(CROW_STATE::LEFTSPIN, L"LeftSpin"));

	TCrowJump* JumpState = new TCrowJump;
	m_pFSM->AddState(L"Jump", JumpState);
	m_mapState.insert(make_pair(CROW_STATE::JUMP, L"Jump"));

	TCrowIdle* IdleState = new TCrowIdle;
	m_pFSM->AddState(L"Idle", IdleState);
	m_mapState.insert(make_pair(CROW_STATE::IDLE, L"Idle"));

	TCrowGuidedBullet* GuidedBulletState = new TCrowGuidedBullet;
	m_pFSM->AddState(L"GuidedBullet", GuidedBulletState);
	m_mapState.insert(make_pair(CROW_STATE::GUIDEDBULLET, L"GuidedBullet"));

	TCrowDeath* DeathState = new TCrowDeath;
	m_pFSM->AddState(L"Death", DeathState);
	m_mapState.insert(make_pair(CROW_STATE::DEATH, L"Death"));

	TCrowBatBullet* BatBullet = new TCrowBatBullet;
	m_pFSM->AddState(L"BatBullet", BatBullet);
	m_mapState.insert(make_pair(CROW_STATE::BATBULLET, L"BatBullet"));

	ChangeState(CROW_STATE::CUTSCENE, 0.04f, L"CutScene");
}

void CCrowScript::update()
{
	CBossScript::update();
	m_pFSM->update();

	if (0 >= m_Hp)
	{
		ChangeState(CROW_STATE::DEATH, 0.1f, L"Death", true);
	}
}

void CCrowScript::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;
	if ((int)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		--m_Hp;
	}
}

void CCrowScript::OnCollision(CGameObject* _pOther)
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
				{
					Script->FindToDeadCheck(Obj);
					break;
				}
			}
		}
	}
}

void CCrowScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CCrowScript::SaveToScene(FILE* _pFile)
{
	CBossScript::SaveToScene(_pFile);
}

void CCrowScript::LoadFromScene(FILE* _pFile)
{
	CBossScript::LoadFromScene(_pFile);
}

CCrowScript::CCrowScript()
	: m_pFSM(nullptr)
	, m_eState(CROW_STATE::END)
	, m_Hp(15)
{
	m_iScriptType = (int)SCRIPT_TYPE::CROWSCRIPT;
}

CCrowScript::~CCrowScript()
{
	SAFE_DELETE(m_pFSM);
}

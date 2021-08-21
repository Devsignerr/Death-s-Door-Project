#include "pch.h"
#include "CCastleScript.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CState.h>

#include "CPlayerScript.h"
#include "CRandomMgrScript.h"

#pragma region CastleStateHeader
#include "TCastleCutScene.h"
#include "TCastleFly.h"
#include "TCastleFly_Finish.h"
#include "TCastleFly_Start.h"
#include "TCastleFly_Stay.h"
#include "TCastleLaser_Finish.h"
#include "TCastleLaser_Ready.h"
#include "TCastleLeftBack_HalfSpin.h"
#include "TCastleLeftFront_HalfSpin.h"
#include "TCastleMissile_Close.h"
#include "TCastleMissile_Open.h"
#include "TCastleRightBack_HalfSpin.h"
#include "TCastleRightFront_HalfSpin.h"
#include "TCastleSpin.h"
#include "TCastleWalk.h"
#include "TCastleDeath.h"
#pragma endregion


void CCastleScript::ChangeState(CASTLE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	m_eState = _eState;

	wstring StateName = {};

	//STATE ���� �̸� �ٲ���� �� 
	map<CASTLE_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
		StateName = iter->second;
	else
		assert(0);

	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}

void CCastleScript::awake()
{
	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;
	m_pFSM->SetObject(GetObj());

	TCastleCutScene* CutSceneState = new TCastleCutScene;						//���⿡�� ������Ʈ ���� , FSM�� �߰���Ŵ 
	m_pFSM->AddState(L"CutScene", CutSceneState);								// FSM�� ���¸� �߰��Ѵ� 
	m_mapState.insert(make_pair(CASTLE_STATE::CUTSCENE, L"CutScene"));			//�� ���¿� , �����̸��� ������ �����Ѵ� 

	TCastleFly* FlyState = new TCastleFly;
	m_pFSM->AddState(L"Fly", FlyState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY, L"Fly"));

	TCastleFly_Finish* FlyFinishState = new TCastleFly_Finish;
	m_pFSM->AddState(L"Fly_Finish", FlyFinishState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_FINISH, L"Fly_Finish"));

	TCastleFly_Start* FlyStartState = new TCastleFly_Start;
	m_pFSM->AddState(L"Fly_Start", FlyStartState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_START, L"Fly_Start"));

	TCastleFly_Stay* FlyStayState = new TCastleFly_Stay;
	m_pFSM->AddState(L"Fly_Stay", FlyStayState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_STAY, L"Fly_Stay"));

	TCastleLaser_Finish* LaserFinishState = new TCastleLaser_Finish;
	m_pFSM->AddState(L"Laser_Finish", LaserFinishState);
	m_mapState.insert(make_pair(CASTLE_STATE::LASER_FINISH, L"Laser_Finish"));

	TCastleLaser_Ready* LaserReadyState = new TCastleLaser_Ready;
	m_pFSM->AddState(L"Laser_Ready", LaserReadyState);
	m_mapState.insert(make_pair(CASTLE_STATE::LASER_READY, L"Laser_Ready"));

	TCastleLeftBack_HalfSpin* LaftBackHalfSpinState = new TCastleLeftBack_HalfSpin;
	m_pFSM->AddState(L"LeftBack_HalfSpin", LaftBackHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::LEFTBACK_HALFSPIN, L"LeftBack_HalfSpin"));

	TCastleLeftFront_HalfSpin* LaftFrontHalfSpinState = new TCastleLeftFront_HalfSpin;
	m_pFSM->AddState(L"LeftFront_HalfSpin", LaftFrontHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::LEFTFRONT_HALFSPIN, L"LeftFront_HalfSpin"));

	TCastleMissile_Close* MissileCloseState = new TCastleMissile_Close;
	m_pFSM->AddState(L"Missile_Close", MissileCloseState);
	m_mapState.insert(make_pair(CASTLE_STATE::MISSILE_CLOSE, L"Missile_Close"));

	TCastleMissile_Open* MissileOpenState = new TCastleMissile_Open;
	m_pFSM->AddState(L"Missile_Open", MissileOpenState);
	m_mapState.insert(make_pair(CASTLE_STATE::MISSILE_OPEN, L"Missile_Open"));

	TCastleRightBack_HalfSpin* RightBackHalfSpinState = new TCastleRightBack_HalfSpin;
	m_pFSM->AddState(L"RightBack_HalfSpin", RightBackHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::RIGHTBACK_HALFSPIN, L"RightBack_HalfSpin"));

	TCastleRightFront_HalfSpin* RightFrontHalfSpinState = new TCastleRightFront_HalfSpin;
	m_pFSM->AddState(L"RightFront_HalfSpin", RightFrontHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::RIGHTFRONT_HALFSPIN, L"RightFront_HalfSpin"));

	TCastleSpin* SpinState = new TCastleSpin;
	m_pFSM->AddState(L"Spin", SpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::SPIN, L"Spin"));

	TCastleWalk* WalkState = new TCastleWalk;
	m_pFSM->AddState(L"Walk", WalkState);
	m_mapState.insert(make_pair(CASTLE_STATE::WALK, L"Walk"));

	TCastleDeath* DeathState = new TCastleDeath;
	m_pFSM->AddState(L"Death", DeathState);
	m_mapState.insert(make_pair(CASTLE_STATE::DEATH, L"Death"));
	

	ChangeState(CASTLE_STATE::CUTSCENE, 0.04f, L"CutScene");
}

void CCastleScript::update()
{
	CBossScript::update();
	m_pFSM->update();

	if (0 >= m_Hp)
	{
		ChangeState(CASTLE_STATE::DEATH, 0.1f, L"Death",true);
	}
}

void CCastleScript::CheckAttackDirection()
{
	CASTLE_DIRECTION AttackDir = {};

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	relativePos.y = 0.0f;
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);
	vFront.y = 0.0f;
	vFront.Normalize();
	relativePos.Normalize();
	//���� �÷��̾ �ٶ󺸴� ���Ϳ� �� front ���͸� ���� 
	Vec3 vCross = relativePos.Cross(vFront);

	//�� ������� ���� ���Ϳ� ������ up����(������ �� front �ƴ�)�� ���������� , 
	// �� ���� ����� �÷��̾�� �� ���� , ������ �����ʿ� �ִ� �� 

	//����Ʈ�� �������� ȸ�����Ѽ� �������Ƿ� �����ͷ� ���� 
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);
	float dist = (-vFront + relativePos).Length();

	Vec3 relativePos2 = Pos - PlayerPos;
	relativePos2.Normalize();
	float Dot = vFront.Dot(relativePos2);
	Dot = acos(Dot);
	if (Dot > -XM_PI / 2.0f && Dot < XM_PI / 2.0f)
	{
		// ����

		// ����
		if (dot > 0.0f)
			AttackDir = CASTLE_DIRECTION::FRONT_LEFT;
		// ������
		else if (dot < 0.0f)
			AttackDir = CASTLE_DIRECTION::FRONT_RIGHT;
	}
	else
	{
		// ���� 

		// ����
		if (dot > 0.0)
			AttackDir = CASTLE_DIRECTION::BACK_LEFT;
		// ������
		else if (dot < 0.0)
			AttackDir = CASTLE_DIRECTION::BACK_RIGHT;
	}

	int AllDir = CRandomMgrScript::GetRandomintNumber(0, 2);

	if (1 == AllDir)
	{
		AttackDir = CASTLE_DIRECTION::ALLDIR;
	}

	switch (AttackDir)
	{
	case CCastleScript::CASTLE_DIRECTION::FRONT_RIGHT:
		ChangeState(CASTLE_STATE::RIGHTFRONT_HALFSPIN, 0.1f, L"RightFront_HalfSpin");
		break;
	case CCastleScript::CASTLE_DIRECTION::FRONT_LEFT:
		ChangeState(CASTLE_STATE::LEFTFRONT_HALFSPIN, 0.1f, L"LeftFront_HalfSpin");
		break;
	case CCastleScript::CASTLE_DIRECTION::BACK_RIGHT:
		ChangeState(CASTLE_STATE::RIGHTBACK_HALFSPIN, 0.1f, L"RightBack_HalfSpin");
		break;
	case CCastleScript::CASTLE_DIRECTION::BACK_LEFT:
		ChangeState(CASTLE_STATE::LEFTBACK_HALFSPIN, 0.1f, L"LeftBack_HalfSpin");
		break;
	case CCastleScript::CASTLE_DIRECTION::ALLDIR:
		ChangeState(CASTLE_STATE::SPIN, 0.1f, L"Spin");
		break;
	default:
		break;
	}
}

void CCastleScript::PatternChoice()
{
	int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

	if (0 == Pattern)
	{
		ChangeState(CASTLE_STATE::FLY_START, 0.1f, L"Fly_Start");
	}
	else if (1 == Pattern)
	{
		ChangeState(CASTLE_STATE::FLY, 0.1f, L"Fly");
	}
}

void CCastleScript::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;
	if ((int)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		--m_Hp;
	}
}

void CCastleScript::OnCollision(CGameObject* _pOther)
{
}

void CCastleScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CCastleScript::SaveToScene(FILE* _pFile)
{
	CBossScript::SaveToScene(_pFile);
}

void CCastleScript::LoadFromScene(FILE* _pFile)
{
	CBossScript::LoadFromScene(_pFile);
}

CCastleScript::CCastleScript()
	: m_pFSM(nullptr)
	, m_eState(CASTLE_STATE::END)
	, m_Hp(15)
{
	m_iScriptType = (int)SCRIPT_TYPE::CASTLESCRIPT;
}

CCastleScript::~CCastleScript()
{
	SAFE_DELETE(m_pFSM);
}

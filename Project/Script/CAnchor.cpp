#include "pch.h"
#include "CAnchor.h"

#include "CPlayerScript.h"

void CAnchor::awake()
{
	CreateCol(Vec3(10.0f, 100.0f, 10.0f), Vec3(0.0f, 35.0f, 0.0f),L"AnchorCol");
}

void CAnchor::update()
{
	//if (true == m_MoveCheck)
	//{
	//	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	//	PlayerPos += m_MoveDir * fDT * m_Speed;
	//
	//	CPlayerScript::GetPlayer()->Transform()->SetLocalPos(PlayerPos);
	//}
}

void CAnchor::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	// 갈고리와 충돌한 경우 
	//if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	//{
	//	m_MoveCheck = true;
	//	m_MoveDir = CPlayerScript::GetPlayerFront();
	//}
	//
	//if ((UINT)LAYER_TYPE::PLAYER_COL == Obj->GetLayerIndex())
	//{
	//	m_MoveCheck = false;
	//}
}

void CAnchor::OnCollision(CGameObject* _pOther)
{
}

void CAnchor::OnCollisionExit(CGameObject* _pOther)
{
}

void CAnchor::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CAnchor::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

CAnchor::CAnchor()
{
	m_iScriptType = (int)SCRIPT_TYPE::ANCHOR;
	m_GimicType = GIMICTYPE::ANCHOR;
}

CAnchor::~CAnchor()
{
}

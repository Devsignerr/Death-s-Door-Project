#include "pch.h"
#include "CState.h"
#include "CFSM.h"
#include "CGameObject.h"


CState::CState():
	m_pFSM(nullptr),
	m_strStateName{}
{
}

CState::~CState()
{
}

CGameObject* CState::GetObj()
{
	return m_pFSM->GetObj();
}

CScript* CState::GetScript()
{
	return m_pFSM->GetObj()->GetScript();
}

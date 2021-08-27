#include "pch.h"
#include "CSceneChange.h"
#include "CFadeScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CTimeMgr.h>

void CSceneChange::awake()
{
	m_NextSceneName = GetObj()->GetName();
}

void CSceneChange::update()
{

	if (true == m_fadeCheck)
	{
		if (true == CFadeScript::GetIsFadeInOut())
		{
			m_fadeCheck = false;
			CSceneMgr::GetInst()->SceneChange(m_NextSceneName.c_str());
		}
	}

	//if (true == m_fadeCheck)
	//{
	//	testtime += fDT;
	//}
	//
	//if (3.0f < testtime)
	//{
	//	m_fadeCheck = false;
	//
	//}
}

CSceneChange::CSceneChange()
	: CScript((UINT)SCRIPT_TYPE::SCENECHANGE)
	, m_fadeCheck(false)
	, testtime(0.0f)
{
}

CSceneChange::~CSceneChange()
{
}

void CSceneChange::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if (10 == Obj->GetLayerIndex())
	{
		CFadeScript::Fade_Out();
		m_fadeCheck = true;
	}


}

void CSceneChange::OnCollision(CGameObject* _pOther)
{
}

void CSceneChange::OnCollisionExit(CGameObject* _pOther)
{

}

void CSceneChange::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CSceneChange::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

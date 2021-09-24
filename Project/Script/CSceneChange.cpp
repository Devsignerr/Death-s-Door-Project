#include "pch.h"
#include "CSceneChange.h"
#include "CFadeScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CTimeMgr.h>

void CSceneChange::awake()
{
	m_NextSceneName = GetObj()->GetName(); 

	MeshRender()->Activate(false);
}

void CSceneChange::update()
{
	if (true == m_fadeCheck)
	{
		CGameObject* pPostEffect = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PostEffect Object", (UINT)LAYER_TYPE::POSTEFFECT);

		CFadeScript* script = (CFadeScript*)pPostEffect->GetScript();

		if (true == script->GetIsFadeInOut())
		{
			m_fadeCheck = false;
			CSceneMgr::GetInst()->SceneChange(m_NextSceneName.c_str());
		}
	}
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

	if ((UINT)LAYER_TYPE::PLAYER_COL == Obj->GetLayerIndex())
	{
		CGameObject* pPostEffect = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PostEffect Object", (UINT)LAYER_TYPE::POSTEFFECT);

		CFadeScript* script = (CFadeScript*)pPostEffect->GetScript();

		script->Fade_Out();

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

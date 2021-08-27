#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CEventMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::init()
{
	m_pCurScene = new CScene;
}

void CSceneMgr::progress()
{
	if (m_pCurScene->GetState() == SCENE_STATE::PLAY)
	{
		m_pCurScene->update();
		m_pCurScene->lateupdate();
	}
	
	m_pCurScene->finalupdate();
}

CGameObject* CSceneMgr::FindObjectByName(const wstring& _strName)
{
	assert(m_pCurScene);

	return m_pCurScene->FindObjectByName(_strName);
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)m_pCurScene;
	evn.wParam = (DWORD_PTR)_pNextScene;

	CEventMgr::GetInst()->AddEvent(evn);
}


void CSceneMgr::LoadScene(CScene* _pScene, const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetResPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	HRESULT hr = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"Scene Load Failed", L"Error", MB_OK);
		return;
	}

	_pScene->LoadFromScene(pFile);

	fclose(pFile);
}

void CSceneMgr::SceneChange(const wstring& _SceneName)
{
	CResMgr::GetInst()->GetNavMeshVec().clear();

	CScene* pScene = new CScene;

	LoadScene(pScene, L"scene\\" + _SceneName + L".scene");

	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)m_pCurScene;
	evn.wParam = (DWORD_PTR)pScene;

	CEventMgr::GetInst()->AddEvent(evn);

}

#pragma once

#include "CSingleton.h"

class CScene;
class CGameObject;

class CSceneMgr
	: public CSingleton<CSceneMgr>
{
	SINGLE(CSceneMgr);
private:
	CScene* m_pCurScene;

public:
	void init();
	void progress();


private:
	void LoadScene(CScene* _pScene, const wstring& _strRelativePath);
public:
	void SceneChange(const wstring& _SceneName);
	
public:
	CScene* GetCurScene() { return m_pCurScene; }
	CGameObject* FindObjectByName(const wstring& _strName);

	void ChangeScene(CScene* _pNextScene);

	friend class CEventMgr;
};


#include "pch.h"
#include "CScript.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CEventMgr.h"
#include "CResMgr.h"
#include <Script/CScriptMgr.h>


CScript::CScript(int _iScriptType)
	: m_iScriptType(_iScriptType)
	, CComponent(COMPONENT_TYPE::SCRIPT)
{
}

CScript::CScript(const CScript& _origin)
	: CComponent(_origin)
	, m_iScriptType(_origin.m_iScriptType)
{
	for (size_t i = 0; i < _origin.m_vecDesc.size(); ++i)
	{
		tDataDesc desc(_origin.m_vecDesc[i].eType, _origin.m_vecDesc[i].strName, nullptr);

		DWORD_PTR pOffset = (DWORD_PTR)_origin.m_vecDesc[i].pData - (DWORD_PTR)&_origin;
		desc.pData = (void*)((DWORD_PTR)this + pOffset);

		m_vecDesc.push_back(desc);
	}
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _pPrefab, Vec3 _vWorldPos, int _iLayerIdx)
{
	CGameObject* pObject = _pPrefab->Instantiate();
	pObject->Transform()->SetLocalPos(_vWorldPos);
	
	tEvent even = {};

	even.eEvent = EVENT_TYPE::CREATE_OBJECT;
	even.lParam = (DWORD_PTR)pObject;
	even.wParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(even);
}

void CScript::DeleteObject(CGameObject* _pTarget)
{
	tEvent even = {};

	even.eEvent = EVENT_TYPE::DELETE_OBJECT;
	even.lParam = (DWORD_PTR)_pTarget;	

	CEventMgr::GetInst()->AddEvent(even);
}

void CScript::AddChild(CGameObject* _pParent, CGameObject* _pChild)
{
	tEvent even = {};

	even.eEvent = EVENT_TYPE::ADD_CHILD;
	even.lParam = (DWORD_PTR)_pParent;
	even.wParam = (DWORD_PTR)_pChild;

	CEventMgr::GetInst()->AddEvent(even);
}

void CScript::DisconnenctWithParent(CGameObject* _pTarget)
{
	tEvent even = {};

	even.eEvent = EVENT_TYPE::DISCONNECT_PARENT;
	even.lParam = (DWORD_PTR)_pTarget;

	CEventMgr::GetInst()->AddEvent(even);
}

void CScript::update()
{



}

void CScript::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	wstring ScriptName = CScriptMgr::GetScriptName(this);
	SaveWString(ScriptName, _pFile);
	//fwrite(&m_iScriptType, sizeof(UINT), 1, _pFile);
}

Ptr<CSound> CScript::Play_Sound(wstring _wstr, int _iRoopCount, bool _bOverlap, float _Volume)
{
	Ptr<CSound> sound = nullptr;

	sound = CResMgr::GetInst()->FindRes<CSound>(_wstr);

	if (nullptr == sound)
	{
		sound = CResMgr::GetInst()->Load<CSound>(_wstr, L"sound\\Allsound\\" + _wstr + L".ogg");
	}

	assert(sound.Get());

	sound->Play(_iRoopCount, _bOverlap, _Volume);

	return sound;
}


Ptr<CSound> CScript::Play_RegionLoopSound(wstring _wstr, unsigned int StartPoint, int _iRoopCount, bool _bOverlap, float _Volume)
{
	Ptr<CSound> sound = nullptr;

	sound = CResMgr::GetInst()->FindRes<CSound>(_wstr);

	if (nullptr == sound)
	{
		sound = CResMgr::GetInst()->Load<CSound>(_wstr, L"sound\\Allsound\\" + _wstr + L".ogg");
	}

	assert(sound.Get());

	sound->PlayRegionLoop(_iRoopCount, StartPoint, _bOverlap, _Volume);

	return sound;
}


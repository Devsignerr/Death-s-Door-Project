#pragma once
#include "CEntity.h"

class CGameObject;
class CState;

class CFSM :
    public CEntity
{
private:
	map<wstring, CState*>	m_mapState;
	CGameObject*		    m_pObject;
	CState*					m_pCurState;

public:
	void update();
	void lateupdate();

public:
	void SetObject(CGameObject* _pObject) { m_pObject = _pObject; }


public:
	void AddState(wstring _strStateName, CState* _pState);
	CState* FindState(wstring _strStateName);
	void ChangeState(wstring _strNextStateName, float _BlendingTime, const wstring& _AniName, bool _Stay);
	CGameObject* GetObj() { return m_pObject; }

	CLONE(CFSM);

public:
	CFSM();
	~CFSM();
};


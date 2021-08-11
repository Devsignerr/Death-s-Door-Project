#pragma once

class CFSM;
class CScript;

class CState 
{
private:
	CFSM* m_pFSM;
	wstring m_strStateName; //��� ���� ������� 

private:
	void SetFSM(CFSM* _pFSM) { m_pFSM = _pFSM; }
	void SetStateName(const wstring& _strName) { m_strStateName = _strName; }

	virtual void update() {};
	virtual void lateupdate() {};

	virtual void Enter() = 0;
	virtual void Exit() = 0;

protected:
	CFSM* GetFSM() { return m_pFSM; }
	CGameObject* GetObj();
	CScript* GetScript();
public:
	CState();
	virtual ~CState();

	friend class CFSM;
};


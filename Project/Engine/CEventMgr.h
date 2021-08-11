#pragma once
#include "CSingleton.h"

#include <queue>
using std::priority_queue;

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	
	ADD_CHILD,
	DISCONNECT_PARENT,

	SCENE_CHANGE,

	END,
};

struct tEvent
{
	EVENT_TYPE	eEvent;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


class Compare
{
public:
	bool operator() (const tEvent& _left, const tEvent& _right)
	{
		return (UINT)_left.eEvent > (UINT)_right.eEvent;
	}
};


class CGameObject;

class CEventMgr :
    public CSingleton<CEventMgr>
{
    SINGLE(CEventMgr);
private:
	priority_queue<tEvent, vector<tEvent>, Compare>	m_event;
	vector<CGameObject*>							m_vecDeadObj;

	bool											m_bEvent; // HierachyGUI 가 알림받을 용도
	bool											m_bDeadEventExist;
public:
	void update();
	void AddEvent(const tEvent& _event) { m_event.push(_event); }

	bool HasEvnOcrd() { return m_bEvent; }

private:
	void ExcuteEvent(const tEvent& _event);
};


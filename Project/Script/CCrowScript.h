#pragma once
#include "CBossScript.h"

class CFSM;
class CCrowScript : public CBossScript
{
private:
	enum class CROW_STATE
	{
		IDLE,
		CUTSCENE,
		WALK,
		SPITTING,
		SLIDING,
		SLIDINGROT,
		SLIDINGREADY,
		GUIDEDBULLET,
		BATBULLET,
		DEATH,
		LEFTSPIN,
		RIGHTSPIN,
		JUMP,
		STOMP,
		RUN,
		STANDINGDEATH,
		END
	};

private:
	CFSM* m_pFSM;
	CROW_STATE					m_eState;
	map<CROW_STATE, wstring>	m_mapState;
	int							m_Hp;

private:
	void ChangeState(CROW_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay = false);
public:
	void awake() override;
	void update() override;
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CCrowScript);
public:
	CCrowScript();
	~CCrowScript();
};


#pragma once
#include "CBossScript.h"
#include <queue>

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

	std::queue<CGameObject*>	m_queueChain;
	int							m_iChainCount;
	CGameObject*			    m_pChainCreater;

private:
	static bool               CrowDeathCheck;

public:
	static void SetCrowDeath() { CrowDeathCheck = true; }
	static bool GetCrowDeath() { return CrowDeathCheck; }

public:
	CGameObject* GetChainCreater() { return m_pChainCreater; }
	void SetChainCreater(CGameObject* _Obj) {m_pChainCreater = _Obj;}
public:
	void CreateChains();
	CGameObject* GetChain();
	void ReturnChain(CGameObject* _Obj);

private:
	void ChangeState(CROW_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay = false);
	void CreateCrowBullet(Vec3 _Pos, Vec3 _Dir);
public:
	void SetbDamaged() { m_bDamaged = false; }
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


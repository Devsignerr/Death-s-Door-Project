#pragma once
#include "CBossScript.h"

class CFSM;
class CIronmaceScript : public CBossScript
{
private:
	enum class IRONMACE_STATE
	{
		CUTSCENE,
		IDLE,
		WALK,
		RUN_ATTACK,
		JUMPREADY,
		JUMPFINISH,
		JUMPATTACK,
		JUMPATTACK2,
		CHOPATTACK,
		CHOPATTACKEND,
		CHOPATTACKCOMBO,
		CHOPATTACKCOMBOEND,
		SPINATTACK,
		SPINATTACKCOMBO,
		DEATH,
		END

	};

private:
	CFSM* m_pFSM;
	IRONMACE_STATE					m_eState;
	map<IRONMACE_STATE, wstring>	m_mapState;
	int								m_Hp;
	
private:
	void ChangeState(IRONMACE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay = false);
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

	CLONE(CIronmaceScript);
public:
	CIronmaceScript();
	~CIronmaceScript();
};


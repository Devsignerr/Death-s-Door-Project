#pragma once
#include "CBossScript.h"

class CFSM;
class CCastleScript :
	public CBossScript
{
private:
	enum class CASTLE_STATE
	{
		CUTSCENE,
		WALK,
		FLY,
		FLY_START,
		FLY_STAY,
		FLY_FINISH,
		SPIN,
		LASER_READY,
		LASER_FINISH,
		LEFTBACK_HALFSPIN,
		LEFTFRONT_HALFSPIN,
		RIGHTBACK_HALFSPIN,
		RIGHTFRONT_HALFSPIN,
		MISSILE_CLOSE,
		MISSILE_OPEN,
		DEATH,
		END

	};

	enum class CASTLE_DIRECTION
	{
		FRONT_RIGHT,
		FRONT_LEFT,
		BACK_RIGHT,
		BACK_LEFT,
		ALLDIR,
		END
	};

private:
	CFSM* m_pFSM;
	CASTLE_STATE				m_eState;
	map<CASTLE_STATE, wstring>	m_mapState;
	int							m_Hp;

private:
	void ChangeState(CASTLE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay = false);
	void CreateLaserPoint();

public:
	void awake() override;
	void update() override;
public:
	void CheckAttackDirection();
	void PatternChoice();
public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;
public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CCastleScript);
public:
	CCastleScript();
	~CCastleScript();
};


#pragma once
#include "CBossScript.h"

class CFSM;
class CCastleScript :
	public CBossScript
{
private:
	enum class CASTLE_STATE
	{
		CUTSCENEWALK,
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
	//CGameObject*				m_pAttackCollider;
	float					    m_fImpactPTCTime;
	float						m_fFlyCloudTime; //부양 시 생성되는 구름 파티클 생성 주기 

public:
	void SetFlyCloudTime(float _Time) { m_fFlyCloudTime = _Time; }

public:
	float GetImpactPTCTime() { return m_fImpactPTCTime; }
	void  SetImpactPTCTime(float _Time) { m_fImpactPTCTime = _Time; }
	void  ActivateImpact();
	void  ActivateFlyCloud();
	void  ActivateAttackCloud();
private:
	void ChangeState(CASTLE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay = false);
	void CreateLaserPoint();

public:
	void awake() override;
	void update() override;
public:
	void CheckAttackDirection();
	void PatternChoice();
	void SetbDamaged() { m_bDamaged = false; }
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


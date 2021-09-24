#pragma once
#include "CMapGimic.h"

class CMaterial;

class CUpDownSwitch : public CMapGimic
{
private:
	static vector<CGameObject*> m_UpDownSwitchObj;
private:
	bool			m_OnOffCheck;
	bool			m_MoveCheck;
	int				m_ThisNum;
	Vec3			m_UpSwitchPos;
	Vec3			m_OriginPos;
	Ptr<CMaterial>	m_pOriginMtrl;
	float			m_SoundTimer;

public:
	void SetOnOffCheck(bool _OnOffCheck) { m_OnOffCheck = _OnOffCheck; }
	void SetThisNum(int _Num) { m_ThisNum = _Num; }
	int	 GetThisNum() { return m_ThisNum; }
private:
	void SetMoveCheck(bool _MoveCheck) { m_MoveCheck = _MoveCheck; }
	void SpinLaserMove();
	void LastSwitchOnCheck();

private:
	void ResetSwitch();
public:
	void awake() override;
	void update() override;

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollision(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

public:
	CUpDownSwitch();
	~CUpDownSwitch();

public:
	CLONE(CUpDownSwitch);
};


#pragma once
#include "CMapGimic.h"
#include <queue>

class CFence :
	public CMapGimic
{
private:
	struct WALLINFO
	{
		Vec3 Pos;
		Vec3 Scale;
		Vec3 Rot;
	};
private:
	static bool m_IsOpen;
private:
	Vec3			m_DestPos;
	CGameObject* m_FenceWall;
private:
	void CreateWallCol();

public:
	static void SetOpen() { m_IsOpen = true; }
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
	CFence();
	~CFence();

public:
	CLONE(CFence);
};


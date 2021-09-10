#pragma once
#include <Engine/CScript.h>

class CTexture;
class CMaterial;

class CActorScript : public CScript
{
private:
	float m_Dist;

	//현재 몇번째 네비메쉬에 올라가있는지 
	UINT m_iCurNavMeshIdx;

	//현재 올라와있는 네비메쉬에서 몇번째 폴리곤에 올라가있는지 
	UINT m_iCurNavNodeIdx;
	int  RayDir;
	//네비메쉬 체크에 실패했음 
	bool m_bOutofMesh;
	bool m_bJump;
	Vec3 m_vEffectColor;

protected:
	bool  m_bDamaged;
	float m_fDamageEffectTime;
	float m_fCurDamageTime;

protected:
	vector<Ptr<CMaterial>> m_vecOriginMtrl;

	Ptr<CTexture> m_WhiteTex;
	Ptr<CTexture> m_RedTex;

public:
	Vec3 GetEffectColor() { return m_vEffectColor; }
	void GetDamage();
	void DamageEffectupdate();
	void EffectParamSetting(Vec4 Diff,Vec4 Emis ,Ptr<CTexture> _Tex);

protected:
	float m_fOffSetPos;
	Ptr<CTexture> m_PaperBurnTex;

	float GetDist() { return m_Dist; }
	void SetbJump(bool _bJump) { m_bJump = _bJump; }
	bool GetbJump() { return m_bJump; }
	

public:
	void CreateDeadParticle();
	void CreateCollider(UINT _LayerIdx, Vec3 _Scale, Vec3 OffsetPos);
	CGameObject* IntanciatePrefab(wstring _wstr,UINT _LayerIdx);
	bool GroundCheck();
	bool ResearchNode();

	//이 함수는 이동할 벡터량을 인자로 받아 네비메쉬를 벗어났다면 이동자체를 시키지 않는다 
	bool GroundCheck(Vec3 _MovePos,int _RayDir =-1);
	bool ResearchNode(Vec3 _MovePos, int _RayDir =-1);

public:
	virtual void ActivateImpactParticle(Vec4 EmisColor, Vec3 Pos, Vec3 Dir, int ActivateCount, float SpreadRange, Vec2 Scale = Vec2(300, 600), Vec2 Speed = Vec2(6000, 10000));

public:
	virtual void update();
	virtual void awake();

	virtual void OnCollisionEnter(CGameObject* _pOther);


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


	CLONE(CActorScript);

public:
	CActorScript();
	virtual ~CActorScript();
};


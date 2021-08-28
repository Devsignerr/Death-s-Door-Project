#pragma once
#include <Engine/CScript.h>

class CTexture;
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

protected:
	float m_fOffSetPos;
	Ptr<CTexture> m_PaperBurnTex;

	float GetDist() { return m_Dist; }
	void SetbJump(bool _bJump) { m_bJump = _bJump; }
	bool GetbJump() { return m_bJump; }
	

public:
	CGameObject* IstanciatePrefab(wstring _wstr,UINT _LayerIdx);
	bool GroundCheck();
	bool ResearchNode();

	//이 함수는 이동할 벡터량을 인자로 받아 네비메쉬를 벗어났다면 이동자체를 시키지 않는다 
	bool GroundCheck(Vec3 _MovePos,int _RayDir =-1);
	bool ResearchNode(Vec3 _MovePos, int _RayDir =-1);

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


#pragma once
#include <Engine/CScript.h>

class CTexture;
class CActorScript : public CScript
{
private:
	float m_Dist;

	//���� ���° �׺�޽��� �ö��ִ��� 
	UINT m_iCurNavMeshIdx;

	//���� �ö���ִ� �׺�޽����� ���° �����￡ �ö��ִ��� 
	UINT m_iCurNavNodeIdx;
	int  RayDir;
	//�׺�޽� üũ�� �������� 
	bool m_bOutofMesh;
	bool m_bJump;

protected:
	float m_fOffSetPos;
	Ptr<CTexture> m_PaperBurnTex;

	float GetDist() { return m_Dist; }
	void SetbJump(bool _bJump) { m_bJump = _bJump; }
	bool GetbJump() { return m_bJump; }
	

public:
	void CreateCollider(UINT _LayerIdx, Vec3 _Scale, Vec3 OffsetPos);
	CGameObject* IstanciatePrefab(wstring _wstr,UINT _LayerIdx);
	bool GroundCheck();
	bool ResearchNode();

	//�� �Լ��� �̵��� ���ͷ��� ���ڷ� �޾� �׺�޽��� ����ٸ� �̵���ü�� ��Ű�� �ʴ´� 
	bool GroundCheck(Vec3 _MovePos,int _RayDir =-1);
	bool ResearchNode(Vec3 _MovePos, int _RayDir =-1);

public:
	virtual void ActivateImpactParticle(Vec3 Pos ,Vec3 Dir, int ActivateCount, float SpreadRange);

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


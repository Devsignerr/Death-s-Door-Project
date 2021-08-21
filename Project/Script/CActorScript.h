#pragma once
#include <Engine/CScript.h>

class CActorScript : public CScript
{
private:
	float m_Dist;

	//���� ���° �׺�޽��� �ö��ִ��� 
	UINT m_iCurNavMeshIdx;

	//���� �ö���ִ� �׺�޽����� ���° �����￡ �ö��ִ��� 
	UINT m_iCurNavNodeIdx;

	//�׺�޽� üũ�� �������� 
	bool m_bOutofMesh;


protected:
	float m_fOffSetPos;


public:
	bool GroundCheck();
	bool ResearchNode();

	//�� �Լ��� �̵��� ���ͷ��� ���ڷ� �޾� �׺�޽��� ����ٸ� �̵���ü�� ��Ű�� �ʴ´� 
	bool GroundCheck(Vec3 _MovePos);
	bool ResearchNode(Vec3 _MovePos);

public:
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther);


	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


	CLONE(CActorScript);

public:
	CActorScript();
	~CActorScript();
};


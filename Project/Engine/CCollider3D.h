#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CCollider3D : public CComponent
{
private:
	Vec3			m_OffsetScale; // �浹ü ũ�� ����
	Vec3			m_OffsetPos;   // �Ǻ� ����

private:
	COLLIDER3D_TYPE m_Collider3DType;
	Matrix			m_MatColWorld;
	Ptr<CMesh>		m_Mesh;
	Ptr<CMaterial>  m_Material;

public:
	COLLIDER3D_TYPE GetCollider3DType() { return m_Collider3DType; }
	const Matrix& GetMatColWorld() { return m_MatColWorld; }
	void SetCollider3DType(COLLIDER3D_TYPE _Type);
public:
	void finalupdate() override;
	void UpdateData();
	void render();
public:
	// �浹 ���� ������ �� ȣ��Ǵ� �Լ�
	virtual void OnCollisionEnter(CCollider3D* _Other);
	// �浹 �����߿� ȣ��Ǵ� �Լ�
	virtual void OnCollisionStay(CCollider3D* _Other);
	// �浹�� �����Ǵ� ������ �� ȣ��Ǵ� �Լ�
	virtual void OnCollisionExit(CCollider3D* _Other);
public:
	void SaveToScene(FILE* _File) override;
	void LoadFromScene(FILE* _File) override;
	CCollider3D* Clone() override
	{
		return new CCollider3D(*this);
	}

public:
	CCollider3D();
	CCollider3D(const CCollider3D& _Origin);
	~CCollider3D();
};


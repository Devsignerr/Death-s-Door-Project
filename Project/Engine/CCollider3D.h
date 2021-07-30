#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CCollider3D : public CComponent
{
private:
	Vec3			m_OffsetScale; // 충돌체 크기 배율
	Vec3			m_OffsetPos;   // 피봇 설정

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
	// 충돌 진입 프레임 시 호출되는 함수
	virtual void OnCollisionEnter(CCollider3D* _Other);
	// 충돌 유지중에 호출되는 함수
	virtual void OnCollisionStay(CCollider3D* _Other);
	// 충돌이 해제되는 프레임 시 호출되는 함수
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


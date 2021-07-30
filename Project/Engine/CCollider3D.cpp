#include "pch.h"
#include "CCollider3D.h"
#include "CResMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTransform.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Collider3DType(COLLIDER3D_TYPE::CUBE)
	, m_OffsetScale(Vec3(1.0f, 1.0f, 1.0f))
	, m_OffsetPos(Vec3(0.0f, 0.0f, 1.0f))
{
	SetCollider3DType(m_Collider3DType);
	m_Material = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl");
}

CCollider3D::CCollider3D(const CCollider3D& _Origin)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Collider3DType(_Origin.m_Collider3DType)
	, m_OffsetScale(_Origin.m_OffsetScale)
	, m_OffsetPos(_Origin.m_OffsetPos)
{
	SetCollider3DType(m_Collider3DType);
	m_Material = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl");
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::finalupdate()
{
	m_MatColWorld = Transform()->GetWorldMat();
	Matrix MatColTrans = {};
	MatColTrans = DirectX::XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);
	Matrix MatColScale = DirectX::XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, m_OffsetScale.z);
	m_MatColWorld = MatColScale * m_MatColWorld * MatColTrans;
}

void CCollider3D::render()
{
	UpdateData();

	m_Material->UpdateData();
	m_Mesh->UpdateData(0);
	m_Mesh->render(0);
}

void CCollider3D::UpdateData()
{
	static const CConstBuffer* CBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_MatColWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	CBuffer->SetData(&g_transform);
	CBuffer->UpdateData((UINT)PIPELINE_STAGE::PS_VERTEX); //
}

void CCollider3D::OnCollisionEnter(CCollider3D* _Other)
{
	int a = 1;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::OnCollisionStay(CCollider3D* _Other)
{
	int a = 1;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::OnCollisionExit(CCollider3D* _Other)
{
	int a = 0;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::SaveToScene(FILE* _File)
{
}

void CCollider3D::LoadFromScene(FILE* _File)
{
}

void CCollider3D::SetCollider3DType(COLLIDER3D_TYPE _Type)
{
	m_Collider3DType = _Type;

	if (COLLIDER3D_TYPE::CUBE == m_Collider3DType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D");
	}
	else if (COLLIDER3D_TYPE::SHPHERE == m_Collider3DType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh_C2D");
	}

	else if (COLLIDER3D_TYPE::POINT == m_Collider3DType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	}
}

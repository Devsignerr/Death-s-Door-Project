#include "pch.h"
#include "CCollider3D.h"
#include "CResMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTransform.h"
#include "CScript.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_Collider3DType(COLLIDER3D_TYPE::CUBE)
	, m_OffsetScale(Vec3(1.0f, 1.0f, 1.0f))
	, m_OffsetPos(Vec3(0.0f, 0.0f, 1.0f))
{
	SetCollider3DType(m_Collider3DType);
	m_Material = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl");
}

CCollider3D::CCollider3D(CCollider3D& _Origin)
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

	Matrix matColTrans = XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, 0.f);
	Matrix matColScale = XMMatrixScaling(m_OffsetScale.x, m_OffsetScale.y, 1.f);

	m_MatColWorld = matColScale * m_MatColWorld * matColTrans;
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
	if (!IsEnable())
		return;

	const vector<CScript*>& vecScript = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionEnter(_Other->GetObj());
	}

	if (GetObj()->GetParent())
	{
		if (nullptr != GetObj()->GetParent()->GetScript())
		{
			GetObj()->GetParent()->GetScript()->OnCollisionEnter(_Other->GetObj());
		}
	}

	int a = 1;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::OnCollision(CCollider3D* _Other)
{
	if (!IsEnable())
		return;

	const vector<CScript*>& vecScript = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollision(_Other->GetObj());
	}

	if (GetObj()->GetParent())
	{
		if (nullptr != GetObj()->GetParent()->GetScript())
		{
			GetObj()->GetParent()->GetScript()->OnCollision(_Other->GetObj());
		}
	}
	int a = 1;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::OnCollisionExit(CCollider3D* _Other)
{
	if (!IsEnable())
		return;

	const vector<CScript*>& vecScript = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionExit(_Other->GetObj());
	}

	if (GetObj()->GetParent())
	{
		if (nullptr != GetObj()->GetParent()->GetScript())
		{
			GetObj()->GetParent()->GetScript()->OnCollisionExit(_Other->GetObj());
		}
	}
	int a = 0;
	m_Material->SetData(SHADER_PARAM::INT_0, &a);
}

void CCollider3D::SaveToScene(FILE* _File)
{
	CComponent::SaveToScene(_File);

	fwrite(&m_OffsetScale, sizeof(Vec2), 1, _File);
	fwrite(&m_OffsetPos, sizeof(Vec2), 1, _File);
	fwrite(&m_Collider3DType, sizeof(UINT), 1, _File);
}

void CCollider3D::LoadFromScene(FILE* _File)
{
	CComponent::LoadFromScene(_File);

	fread(&m_OffsetScale, sizeof(Vec2), 1, _File);
	fread(&m_OffsetPos, sizeof(Vec2), 1, _File);
	fread(&m_Collider3DType, sizeof(UINT), 1, _File);

	SetCollider3DType(m_Collider3DType);
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

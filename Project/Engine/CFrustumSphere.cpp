#include "pch.h"
#include "CFrustumSphere.h"
#include "CCollider3D.h"
#include "CTransform.h"

CFrustumSphere::CFrustumSphere()
	:CComponent(COMPONENT_TYPE::FRUSTUMSPHERE),
	m_vOffsetPos{},
	m_fRadius()
{

}

CFrustumSphere::CFrustumSphere(CFrustumSphere& _Origin)
	: CComponent(COMPONENT_TYPE::FRUSTUMSPHERE),
	m_vOffsetPos(_Origin.m_vOffsetPos),
	m_fRadius(_Origin.m_fRadius)
{

}


CFrustumSphere::~CFrustumSphere()
{
}


void CFrustumSphere::awake()
{

}

void CFrustumSphere::update()
{
	if (GetObj()->Collider3D())
	{
		GetObj()->Collider3D()->Activate(false);
	}	

	if (GetObj()->MeshRender())
	{
		GetObj()->Collider3D()->Activate(false);
	}
}

void CFrustumSphere::finalupdate()
{
	//m_fRadius = Transform()->GetLocalScale().x;
	//m_vOffsetPos = 
}

void CFrustumSphere::SaveToScene(FILE* _pFile)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3),1, _pFile);
	fwrite(&m_fRadius, sizeof(float), 1, _pFile);

	CComponent::SaveToScene(_pFile);
}

void CFrustumSphere::LoadFromScene(FILE* _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_fRadius, sizeof(float), 1, _pFile);

	CComponent::LoadFromScene(_pFile);
}


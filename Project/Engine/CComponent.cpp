#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _eType)
	: m_eComType(_eType)
	, m_pObject(nullptr)
	,m_bEnable(true)
{
}

CComponent::CComponent(const CComponent& _origin)
	: m_eComType(_origin.m_eComType)
	, m_pObject(nullptr)
	, m_bEnable(_origin.m_bEnable)
{
}

CComponent::~CComponent()
{
}

wstring CComponent::GetComponentName()
{
	if (m_eComType == COMPONENT_TYPE::ANIMATOR2D)
	{
		return L"ANIMATOR2D";
	}

	else if (m_eComType == COMPONENT_TYPE::ANIMATOR3D)
	{
		return L"ANIMATOR3D";
	}

	else if (m_eComType == COMPONENT_TYPE::CAMERA)
	{
		return L"CAMERA";
	}

	else if (m_eComType == COMPONENT_TYPE::COLLIDER2D)
	{
		return L"COLLIDER2D";
	}

	else if (m_eComType == COMPONENT_TYPE::COLLIDER3D)
	{
		return L"COLLIDER3D";
	}

	else if (m_eComType == COMPONENT_TYPE::FRUSTUMSPHERE)
	{
		return L"FRUSTUMSPHERE";
	}

	else if (m_eComType == COMPONENT_TYPE::LIGHT2D)
	{
		return L"LIGHT2D";
	}

	else if (m_eComType == COMPONENT_TYPE::LIGHT3D)
	{
		return L"LIGHT3D";
	}

	else if (m_eComType == COMPONENT_TYPE::MESHRENDER)
	{
		return L"MESHRENDER";
	}

	else if (m_eComType == COMPONENT_TYPE::PARTICLE)
	{
		return L"PARTICLE";
	}

	else if (m_eComType == COMPONENT_TYPE::TERRAIN)
	{
		return L"TERRAIN";
	}

	else if (m_eComType == COMPONENT_TYPE::TRANSFORM)
	{
		return L"TRANSFORM";
	}

	else if (m_eComType == COMPONENT_TYPE::UI)
	{
		return L"UI";
	}

}

void CComponent::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);

	//fwrite(&m_bEnable, sizeof(bool), 1, _pFile);

}

void CComponent::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);
		
	//fread(&m_bEnable, sizeof(bool), 1, _pFile);
}
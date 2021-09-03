#include "pch.h"
#include "CUI.h"


CUI::CUI():
	CComponent(COMPONENT_TYPE::UI)
{
}

CUI::CUI(CUI& _origin):
	CComponent(COMPONENT_TYPE::UI)
{
}

CUI::~CUI()
{
}


void CUI::awake()
{
}

void CUI::start()
{
}

void CUI::update()
{
}

void CUI::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
}

void CUI::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
}

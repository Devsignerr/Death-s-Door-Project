#include "pch.h"
#include "UIGUI.h"


UIGUI::UIGUI():
	ComponentGUI(COMPONENT_TYPE::UI)
{
}

UIGUI::~UIGUI()
{
}

void UIGUI::update()
{
	ComponentGUI::update();

	if (!IsActive())
		return;
}

void UIGUI::render()
{
	Start();

	End();
}


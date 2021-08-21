#pragma once
#include "ComponentGUI.h"
class FrustumGUI :
    public ComponentGUI
{
public:
	FrustumGUI();
	~FrustumGUI();
public:
	virtual void update();
	virtual void render();
};


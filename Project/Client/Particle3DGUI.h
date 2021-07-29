#pragma once
#include "ComponentGUI.h"

class Particle3DGUI :
	public ComponentGUI
{
public:
	Particle3DGUI();
	~Particle3DGUI();
public:
	virtual void update();
	virtual void render();
};


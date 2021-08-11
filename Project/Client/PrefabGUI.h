#pragma once

#include "ResInfoGUI.h"
#include <Engine/Ptr.h>
#include <Engine/CPrefab.h>


class PrefabGUI:
	public ResInfoGUI
{
public:
    virtual void render();

public:
    PrefabGUI();
    virtual ~PrefabGUI();
};


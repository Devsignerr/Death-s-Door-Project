#pragma once
#include <Engine/CScript.h>

class CEnding :
	public CScript
{

public:
    virtual void awake();
    virtual void update();
public:
    CEnding();
    ~CEnding();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CEnding);
};


#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CEnding :
	public CScript
{

private:
    CGameObject* m_Ending;
    float        m_Time;
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


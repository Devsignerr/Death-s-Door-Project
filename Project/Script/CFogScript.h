#pragma once
#include <Engine/CScript.h>

class CFogScript : public CScript
{
private:
    float m_fMaxHeight;
    float m_fBlendingRatio;

public:
    virtual void awake();
    virtual void update();
public:
    CFogScript();
    ~CFogScript();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CFogScript);
};


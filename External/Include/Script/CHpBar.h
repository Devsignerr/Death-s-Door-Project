#pragma once
#include <Engine/CScript.h>
class CHpBar : public CScript
{
public:
    vector<CGameObject*> m_HpBar;
    int                      m_Hp;
public:
    virtual void awake();
    virtual void update();
public:
    CHpBar();
    ~CHpBar();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CHpBar);
};


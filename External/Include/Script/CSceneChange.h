#pragma once
#include <Engine/CScript.h>

class CSceneChange : public CScript
{
private:
    wstring m_NextSceneName;
    bool    m_fadeCheck;
    float  testtime;
public:
    virtual void awake();
    virtual void update();
public:
    CSceneChange();
    ~CSceneChange();

public:
    void OnCollisionEnter(CGameObject* _pOther) override;
    void OnCollision(CGameObject* _pOther) override;
    void OnCollisionExit(CGameObject* _pOther) override;


    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CSceneChange);
};


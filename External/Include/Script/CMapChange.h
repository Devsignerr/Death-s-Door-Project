#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CMapChange : public CScript
{
private:
    static vector<CGameObject*> FindCollNum;
public:
    static bool IsMapChange;
private:
    int m_ThisCollNum;
    int m_ChangeCollNum;
    bool m_MapChangeCheck;
public:
    int GetThisColNum() { return m_ThisCollNum; }

public:
    virtual void awake();
    virtual void update();
public:
    CMapChange();
    ~CMapChange();

public:
    void OnCollisionEnter(CGameObject* _pOther) override;
    void OnCollision(CGameObject* _pOther) override;
    void OnCollisionExit(CGameObject* _pOther) override;


    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CMapChange);
};


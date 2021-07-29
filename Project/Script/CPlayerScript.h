#pragma once
#include <Engine\CScript.h>\


enum class PLAYER_STATE 
{
    READY,
    ATTACK,
    RUN
};

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>        m_pMissilePrefab;
    int                 m_iData;
    float               m_fData;
    Vec2                m_v2Data;
    Vec4                m_v4Data;
    PLAYER_STATE        m_eState;
    string              m_strState;
    float               m_fRotationSpeed;


public:
    virtual void awake();
    virtual void update();

private:
    void Move();
    void Attack();
    void CreateMissile();    

private:
    void ChangeState(PLAYER_STATE _NextState);
    PLAYER_STATE GetState() { return m_eState; }

    void Player_Ready();
    void Player_Attack();
    void Player_Run();


public:
    virtual void OnCollisionEnter(CGameObject* _pOther);

public:
    CLONE(CPlayerScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);


public:
    CPlayerScript();
    ~CPlayerScript();
};


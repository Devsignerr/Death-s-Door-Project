#pragma once
#include <Engine\CScript.h>
#include <queue>

class CMemoryPoolScript :
    public CScript
{
private:

    static int     m_iChainCount;
    static int     m_iExplosionPTC;
    static int     m_iFireDamagePTC;
    static int     m_iAttackImpact;
    static int     m_iCrowBullet;

    static std::queue<CGameObject*> m_queueExplosionPTC;
    static std::queue<CGameObject*> m_queueFireDamagePTC;
    static std::queue<CGameObject*> m_queueChain;
    static std::queue<CGameObject*> m_queueAttackImpact;
    static std::queue<CGameObject*> m_queueCrowBullet;

public:
    static void CreateAttackImpact();
    static void CreateExplosionPTC();
    static void CreateFireDamagePTC();
    static void CreateChain();
    static void CreateCrowBullet();

public:
    static void ReturnObj(CGameObject* _Obj);

public:
    static CGameObject* GetAttackImpact();
    static CGameObject* GetExplosionPTC();
    static CGameObject* GetChain();
    static CGameObject* GetFireDamagePTC();
    static CGameObject* GetCrowBullet();


public:
    virtual void awake();
    virtual void update();


public:
    CMemoryPoolScript();
    ~CMemoryPoolScript();


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CMemoryPoolScript);
};


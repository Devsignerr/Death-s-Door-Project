#pragma once
#include <Engine\CScript.h>
#include <queue>

//예상 투사체 목록
//성보스 미사일 (25개)
//크로우보스 까마귀 큰머리 , 작은 까마귀떼 (각 5개 , 50개)
//플레이어 파이어볼 , 화살 , 폭탄 (각1 개)

//예상 파티클 목록
//몬스터 몸에 붙을 불 (10개)
//

class CMemoryPoolScript :
    public CScript
{
private:

    static int     m_iChainCount;
    static int     m_iExplosionPTC;

    static std::queue<CGameObject*> m_queueExplosionPTC;
    static std::queue<CGameObject*> m_queueChain;


public:
    static void CreateExplosionPTC();
    static void CreateChain();

public:
    static void ReturnObj(CGameObject* _Obj);

public:
    static CGameObject* GetExplosionPTC();
    static CGameObject* GetChain();


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


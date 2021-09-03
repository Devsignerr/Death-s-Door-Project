#pragma once
#include <Engine\CScript.h>
#include <queue>

//���� ����ü ���
//������ �̻��� (25��)
//ũ�ο캸�� ��� ū�Ӹ� , ���� ��Ͷ� (�� 5�� , 50��)
//�÷��̾� ���̾ , ȭ�� , ��ź (��1 ��)

//���� ��ƼŬ ���
//���� ���� ���� �� (10��)
//

class CMemoryPoolScript :
    public CScript
{
private:

    static int     m_iChainCount;
    static int     m_iExplosionPTC;
    static int     m_iFireDamagePTC;
    static int     m_iAttackImpact;

    static std::queue<CGameObject*> m_queueExplosionPTC;
    static std::queue<CGameObject*> m_queueFireDamagePTC;
    static std::queue<CGameObject*> m_queueChain;
    static std::queue<CGameObject*> m_queueAttackImpact;



public:
    static void CreateAttackImpact();
    static void CreateExplosionPTC();
    static void CreateFireDamagePTC();
    static void CreateChain();

public:
    static void ReturnObj(CGameObject* _Obj);

public:
    static CGameObject* GetAttackImpact();
    static CGameObject* GetExplosionPTC();
    static CGameObject* GetChain();
    static CGameObject* GetFireDamagePTC();


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


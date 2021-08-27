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

    static std::queue<CGameObject*> m_queueChain;


public:

    static void CreateChain();

public:
    static void ReturnObj(CGameObject* _Obj);

public:

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


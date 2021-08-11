#pragma once
#include <Engine\CScript.h>
#include "CActorScript.h"

class CFSM;

enum class PLAYER_STATE 
{
    IDLE, //�ƹ��� ���۵� �ϰ� ���� ������ 
    RUN, //Walk �� ������ ���� �ٴµ� . Run���� ����ҵ��� 
    SLASH_CHARGE, // ���������� �¿� �ѹ��� �����ư��� �ִϸ��̼��� ����� 
    SLASH_CHARGE_MAX, //���� ���϶� 
    SLASH_CHARGE_ATTACK, //�������� . �׳� ���콺 �߰�Ű�� Ŭ���ϸ� ������ ª�� ����ǰ� �ٷ� ���������� �� .
    SLASH_ATTACK, // �ܼ��� ��Ŭ������ �ϴ� ���� . �¿� �����ư��� ������ 
    ROLL, //�����̽��� �Է½� ����
    ROLL_SLASH, //ROLL �����϶� ���콺 �߰�Ű�� ������ �ߵ��Ǵ� ���� 
    LADDER_UP, //��ٸ� �տ��� EŰ �Է½� ��ٸ� ���� �ö�Ÿ�� , ���� �ö󰡸� ���
    LADDER_DOWN, //���� ��� 
    LADDER_TOP, //��ٸ��� ���� �ö����� 
    DANCE, //�����
    DROWN,//���� ��������
    HOOK_SHOT, //�� �� ���ؽ� �� �ִϸ��̼��� �ٷ� �����
    HOOK_SHOT_FLY, //�ż� �� ���󰡴� ���� 
    ARROW , // ȭ�� �����ϴ� ��� . �ִϸ��̼��� �����µ��� ���콺 �Է����̸� ������ ������ �����ϵ��� 
    MAGIC, //�Ҹ��� ���� �� . ������ �׳� �ٷ� IDLE ���·� �ǵ��ư��� 
    BOMB, // ��ź���� ���� ��. ������ BOMB_END �ִϸ��̼� ���
    BOMB_END , // ��ź���� �߻� �ִϸ��̼�. ������ IDLE�� 
    GET_ITEM, //���� ������� 
    HIT_BACK, // ���¾Ƽ� ���󰥶�
    HIT_IDLE, //���¾Ƽ� �����ִ��� . �ƹ�Ű �ȴ����� �˾Ƽ� �Ͼ�� 
    HIT_RECOVER, // ȸ���ϰ� �ٽ� �Ͼ�� 
    END,
};

class CPlayerScript :
    public CActorScript
{
private:
    int                 m_iHP;
    int                 m_iMaxHP;

    int                 m_iMP;
    int                 m_iMaxMP;

    float               m_fSpeed;           //�׳� �̵� �� ���ǵ�
    float               m_fAttackSpeed;     //��Ŭ�� ���� �� �ణ ������ ������ ���ǵ� . �������õ� �� ���� �̿��Ұ� 
    float               m_fRotationSpeed;   //���� ��ȯ�� ȸ�� ���ǵ� 
    float               m_fRollSpeed;       //������ �̵� �� ���ǵ� 

    float               m_fAttackCoolTime;
    CGameObject*        m_Weapon;           //���� �ι�° �ڽ��̴� . 

    Ptr<CPrefab>        m_pArrowPrefab;     //Ȱ ��� �� ������ ������ 
    Ptr<CPrefab>        m_pBombPrefab;      //��ź ��� �� ������ ������
    Ptr<CPrefab>        m_pMagicPrefab;     // ���� ��� �� ������ ������ 
                                            // HookShot�� �������� �ƴ϶� MemoryPool ���� ��� ����� ����. �罽�� �ּ� 2~30���� �ɰ��̶� ������ �����Ҽ� ���� 

    CFSM*               m_pFSM;
    PLAYER_STATE        m_eState;
    string              m_strState;

    //===============================
    static Vec3 PlayerPos;
    static Vec3 vPlayerFront;

    //FSMŬ������  ����� ��󸶴� map �� KEY�� �˸°� �������ָ� �ȴ� 
    map<PLAYER_STATE ,wstring>	m_mapState;

public:
    static Vec3 GetPlayerPos() { return PlayerPos; }
    static Vec3 GetPlayerFront() { return vPlayerFront; }

public:
    virtual void awake() override; //���⿡�� FSM �ʱ�ȭ�� ������ 
    virtual void start() override;

    virtual void update() override;
    virtual void lateupdate() override;

private:
    void ChangeState(PLAYER_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay);
    PLAYER_STATE GetState() { return m_eState; }

public:
    void KeyInput();

public:
    virtual void OnCollisionEnter(CGameObject* _pOther);
    virtual void OnCollision(CGameObject* _pOther);
    virtual void OnCollisionExit(CGameObject* _pOther);

public:
    CLONE(CPlayerScript);
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);


public:
    CPlayerScript();
    ~CPlayerScript();
};


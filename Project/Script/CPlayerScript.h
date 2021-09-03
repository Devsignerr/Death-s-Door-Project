#pragma once
#include <Engine\CScript.h>
#include "CActorScript.h"

class CFSM;

enum class PLAYER_STATE 
{
    IDLE, //�ƹ��� ���۵� �ϰ� ���� ������ 
    RUN, //Walk �� ������ ���� �ٴµ� . Run���� ����ҵ��� 
    SLASH_CHARGE_L, // ���������� �¿� �ѹ��� �����ư��� �ִϸ��̼��� ����� 
    SLASH_CHARGE_R, // ���������� �¿� �ѹ��� �����ư��� �ִϸ��̼��� ����� 
    SLASH_CHARGE_MAX_L, //���� ���϶� 
    SLASH_CHARGE_MAX_R, //���� ���϶� 
    SLASH_CHARGE_ATTACK_L, //�������� . �׳� ���콺 �߰�Ű�� Ŭ���ϸ� ������ ª�� ����ǰ� �ٷ� ���������� �� .
    SLASH_CHARGE_ATTACK_R, //�������� . �׳� ���콺 �߰�Ű�� Ŭ���ϸ� ������ ª�� ����ǰ� �ٷ� ���������� �� .
    SLASH_ATTACK_L, // �ܼ��� ��Ŭ������ �ϴ� ���� . �¿� �����ư��� ������ 
    SLASH_ATTACK_R, // �ܼ��� ��Ŭ������ �ϴ� ���� . �¿� �����ư��� ������ 
    ROLL, //�����̽��� �Է½� ����
    ROLL_SLASH, //ROLL �����϶� ���콺 �߰�Ű�� ������ �ߵ��Ǵ� ���� 
    LADDER_UP, //��ٸ� �տ��� EŰ �Է½� ��ٸ� ���� �ö�Ÿ�� , ���� �ö󰡸� ��� X
    LADDER_DOWN, //���� ��� X
    LADDER_TOP, //��ٸ��� ���� �ö����� X
    DANCE, //����� X
    DROWN,//���� �������� X
    HOOK_SHOT, //�� �� ���ؽ� �� �ִϸ��̼��� �ٷ� ����� X
    HOOK_SHOT_FLY, //�ż� �� ���󰡴� ���� X
    ARROW , // ȭ�� �����ϴ� ��� . �ִϸ��̼��� �����µ��� ���콺 �Է����̸� ������ ������ �����ϵ��� 
    MAGIC, //�Ҹ��� ���� �� . ������ �׳� �ٷ� IDLE ���·� �ǵ��ư��� 
    BOMB, // ��ź���� ���� ��. ������ BOMB_END �ִϸ��̼� ���
    BOMB_END , // ��ź���� �߻� �ִϸ��̼�. ������ IDLE�� 
    GET_ITEM, //���� ������� 
    HIT_BACK, // ���¾Ƽ� ���󰥶�
    HIT_RECOVER, // ȸ���ϰ� �ٽ� �Ͼ�� 
    END,
};

enum class PLAYER_PROJECTILE_TYPE
{
    ARROW,
    MAGIC,
    BOMB,
    HOOK,
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

    CFSM*               m_pFSM;
    PLAYER_STATE        m_eState;
    string              m_strState;


    //===============================
    static Vec3 m_OtherPower;
    static Vec3 PlayerMovePos;
    static Vec3 PlayerPos;
    static Vec3 PlayerPrePos;
    static Vec3 vPlayerRot;
    static Vec3 vPlayerFront;
    static Vec3 vPlayerUp;
    static CPlayerScript* Player;

    //FSMŬ������  ����� ��󸶴� map �� KEY�� �˸°� �������ָ� �ȴ� 
    map<PLAYER_STATE ,wstring>	m_mapState;


public:
    static CGameObject* m_pMagic;
    static CGameObject* m_pArrow;
    static CGameObject* m_pBomb;
    static CGameObject* m_pHook;

    static CGameObject* m_pBow;
    static CGameObject* m_Weapon;

    static CGameObject* m_pRollSlash;
    static CGameObject* m_pHorizonSlashL;
    static CGameObject* m_pHorizonSlashR;

    static CGameObject* m_pHeavySlashL;
    static CGameObject* m_pHeavySlashR;

private:
    PLAYER_PROJECTILE_TYPE m_PlayerProjectileType;
public:
    PLAYER_PROJECTILE_TYPE GetPlayerProjectileType() { return m_PlayerProjectileType; }

private:
    float m_Distance;


public:
    static Vec3 GetPlayerMovePos(){ return PlayerMovePos; }
    static Vec3 GetPlayerPos() { return PlayerPos; }
    static Vec3 GetPlayerRot() { return vPlayerRot; }
    static Vec3 GetPlayerFront() { return vPlayerFront; }
    static Vec3 GetPlayerUp() { return vPlayerUp; }
    static CPlayerScript* GetPlayer() { return Player; }
    static void SetOtherPower(Vec3 _Power) { m_OtherPower = _Power; }
    static Vec3 GetOtherPower() { return m_OtherPower; }


public:
    virtual void awake() override; //���⿡�� FSM �ʱ�ȭ�� ������ 
    virtual void start() override;

    virtual void update() override;
    virtual void lateupdate() override;

public:
    void ChangeState(PLAYER_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay);
    PLAYER_STATE GetState() { return m_eState; }
    void CreateCol();
public:
    void KeyInput();
    Vec3 GetMouseClickPos();
    void RotatetoClick(Vec3 _ClickPos);
    void SetState(PLAYER_STATE _eState) { m_eState = _eState; }
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


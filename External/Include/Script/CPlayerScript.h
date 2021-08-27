#pragma once
#include <Engine\CScript.h>
#include "CActorScript.h"

class CFSM;

enum class PLAYER_STATE 
{
    IDLE, //아무런 동작도 하고 있지 않을때 
    RUN, //Walk 도 있지만 거의 뛰는듯 . Run으로 충분할듯함 
    SLASH_CHARGE_L, // 차지어택은 좌우 한번씩 번갈아가며 애니메이션이 재생됨 
    SLASH_CHARGE_R, // 차지어택은 좌우 한번씩 번갈아가며 애니메이션이 재생됨 
    SLASH_CHARGE_MAX_L, //차지 중일때 
    SLASH_CHARGE_MAX_R, //차지 중일때 
    SLASH_CHARGE_ATTACK_L, //차지어택 . 그냥 마우스 중간키를 클릭하면 차지가 짧게 재생되고 바로 차지어택을 함 .
    SLASH_CHARGE_ATTACK_R, //차지어택 . 그냥 마우스 중간키를 클릭하면 차지가 짧게 재생되고 바로 차지어택을 함 .
    SLASH_ATTACK_L, // 단순히 좌클릭으로 하는 공격 . 좌우 번갈아가며 공격함 
    SLASH_ATTACK_R, // 단순히 좌클릭으로 하는 공격 . 좌우 번갈아가며 공격함 
    ROLL, //스페이스바 입력시 구름
    ROLL_SLASH, //ROLL 상태일때 마우스 중간키를 누르면 발동되는 공격 
    LADDER_UP, //사다리 앞에서 E키 입력시 사다리 위로 올라타고 , 위로 올라가면 재생 X
    LADDER_DOWN, //위와 비슷 X
    LADDER_TOP, //사다리를 전부 올라갔을때 X
    DANCE, //춤춘다 X
    DROWN,//물에 빠졌을때 X
    HOOK_SHOT, //훅 샷 조준시 이 애니메이션이 바로 재생됨 X
    HOOK_SHOT_FLY, //훅샷 후 날라가는 상태 X
    ARROW , // 화살 장전하는 모션 . 애니메이션이 끝났는데도 마우스 입력중이면 마지막 프레임 유지하도록 
    MAGIC, //불마법 장전 중 . 시전시 그냥 바로 IDLE 상태로 되돌아간다 
    BOMB, // 폭탄마법 장전 중. 시전시 BOMB_END 애니메이션 재생
    BOMB_END , // 폭탄마법 발사 애니메이션. 끝나면 IDLE로 
    GET_ITEM, //무언가 얻었을때 
    HIT_BACK, // 얻어맞아서 날라갈때
    HIT_RECOVER, // 회복하고 다시 일어난다 
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

    float               m_fSpeed;           //그냥 이동 시 스피드
    float               m_fAttackSpeed;     //좌클릭 공격 시 약간 앞으로 나가는 스피드 . 차지어택도 이 값을 이용할것 
    float               m_fRotationSpeed;   //방향 전환시 회전 스피드 
    float               m_fRollSpeed;       //구르기 이동 시 스피드 

    float               m_fAttackCoolTime;

    CFSM*               m_pFSM;
    PLAYER_STATE        m_eState;
    string              m_strState;


    //===============================
    static Vec3 PlayerMovePos;
    static Vec3 PlayerPos;
    static Vec3 PlayerPrePos;
    static Vec3 vPlayerRot;
    static Vec3 vPlayerFront;
    static Vec3 vPlayerUp;
    static CPlayerScript* Player;

    //FSM클래스를  사용할 대상마다 map 의 KEY를 알맞게 정의해주면 된다 
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

public:
    virtual void awake() override; //여기에서 FSM 초기화도 진행함 
    virtual void start() override;

    virtual void update() override;
    virtual void lateupdate() override;

private:
    void ChangeState(PLAYER_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay);
    PLAYER_STATE GetState() { return m_eState; }

public:
    void KeyInput();
    Vec3 GetMouseClickPos();
    void RotatetoClick(Vec3 _ClickPos);
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


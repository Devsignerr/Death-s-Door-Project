#pragma once
#include <Engine\CScript.h>
#include "CActorScript.h"

class CFSM;

enum class PLAYER_STATE 
{
    IDLE, //아무런 동작도 하고 있지 않을때 
    RUN, //Walk 도 있지만 거의 뛰는듯 . Run으로 충분할듯함 
    SLASH_CHARGE, // 차지어택은 좌우 한번씩 번갈아가며 애니메이션이 재생됨 
    SLASH_CHARGE_MAX, //차지 중일때 
    SLASH_CHARGE_ATTACK, //차지어택 . 그냥 마우스 중간키를 클릭하면 차지가 짧게 재생되고 바로 차지어택을 함 .
    SLASH_ATTACK, // 단순히 좌클릭으로 하는 공격 . 좌우 번갈아가며 공격함 
    ROLL, //스페이스바 입력시 구름
    ROLL_SLASH, //ROLL 상태일때 마우스 중간키를 누르면 발동되는 공격 
    LADDER_UP, //사다리 앞에서 E키 입력시 사다리 위로 올라타고 , 위로 올라가면 재생
    LADDER_DOWN, //위와 비슷 
    LADDER_TOP, //사다리를 전부 올라갔을때 
    DANCE, //춤춘다
    DROWN,//물에 빠졌을때
    HOOK_SHOT, //훅 샷 조준시 이 애니메이션이 바로 재생됨
    HOOK_SHOT_FLY, //훅샷 후 날라가는 상태 
    ARROW , // 화살 장전하는 모션 . 애니메이션이 끝났는데도 마우스 입력중이면 마지막 프레임 유지하도록 
    MAGIC, //불마법 장전 중 . 시전시 그냥 바로 IDLE 상태로 되돌아간다 
    BOMB, // 폭탄마법 장전 중. 시전시 BOMB_END 애니메이션 재생
    BOMB_END , // 폭탄마법 발사 애니메이션. 끝나면 IDLE로 
    GET_ITEM, //무언가 얻었을때 
    HIT_BACK, // 얻어맞아서 날라갈때
    HIT_IDLE, //얻어맞아서 누워있는중 . 아무키 안눌러도 알아서 일어난다 
    HIT_RECOVER, // 회복하고 다시 일어난다 
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

    float               m_fSpeed;           //그냥 이동 시 스피드
    float               m_fAttackSpeed;     //좌클릭 공격 시 약간 앞으로 나가는 스피드 . 차지어택도 이 값을 이용할것 
    float               m_fRotationSpeed;   //방향 전환시 회전 스피드 
    float               m_fRollSpeed;       //구르기 이동 시 스피드 

    float               m_fAttackCoolTime;
    CGameObject*        m_Weapon;           //보통 두번째 자식이다 . 

    Ptr<CPrefab>        m_pArrowPrefab;     //활 모션 시 생성할 프리팹 
    Ptr<CPrefab>        m_pBombPrefab;      //폭탄 모션 시 생성할 프리팹
    Ptr<CPrefab>        m_pMagicPrefab;     // 마법 모션 시 생성할 프리팹 
                                            // HookShot은 프리팹이 아니라 MemoryPool 에서 얻어 상요할 예정. 사슬이 최소 2~30개는 될것이라 일일히 생성할순 없다 

    CFSM*               m_pFSM;
    PLAYER_STATE        m_eState;
    string              m_strState;

    //===============================
    static Vec3 PlayerPos;
    static Vec3 vPlayerFront;

    //FSM클래스를  사용할 대상마다 map 의 KEY를 알맞게 정의해주면 된다 
    map<PLAYER_STATE ,wstring>	m_mapState;

public:
    static Vec3 GetPlayerPos() { return PlayerPos; }
    static Vec3 GetPlayerFront() { return vPlayerFront; }

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


#pragma once
#include "CMapGimic.h"

enum class DOOR_STATE
{
    SPAWNING, // 소환중
    DISAPPEARING,  //사라지는중 
    STAY_ACTIVE, //소환되어서 대기하는중 
    STAY_DEACTIVE, // 없어져서 대기하는중 
};

class CDoorScript :
    public CMapGimic
{
public:
    float       m_fWaitTime;
    float       m_fBurn;
    DOOR_STATE  m_eState;
    static bool m_bIronDead;
    static bool m_bCrowDead;

public:
    static bool m_bActive;

public:
    void Spawn();
    void Disappear();
    void CreateCol(wstring _wstr);

public:
    virtual void awake();
    virtual void update();
public:
    CDoorScript();
    virtual ~CDoorScript();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CDoorScript);

};


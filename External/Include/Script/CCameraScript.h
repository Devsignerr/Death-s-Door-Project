#pragma once
#include <Engine\CScript.h>

enum class CAMERA_MODE 
{
    FREE,
    FOLLOW
};

class CCameraScript :
    public CScript
{
private:
    CGameObject* m_pPlayerPivot;
    CAMERA_MODE  m_eMode;
    float        m_fOrbit;
public:
    virtual void update();
    virtual void start();
public:
    CLONE(CCameraScript);

private:
    void SetCameraPos();
    void CameraFreeMove();
    void FollowMove();

public:
    CCameraScript();
    ~CCameraScript();
};


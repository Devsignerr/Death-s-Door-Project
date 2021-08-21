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
    CGameObject* m_pDirLight;
    CAMERA_MODE  m_eMode;

public:
    virtual void update();
    virtual void start();
public:
    CLONE(CCameraScript);
public:
    void CameraShake();
    void MakeLightFollow(Vec3 _MovePos);
private:
    void CameraFreeMove();
    void CameraFollowMove();
    void SetCameraPosRot(Vec3 _camPos , Vec3 _camRot);


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);


public:
    CCameraScript();
    ~CCameraScript();
};


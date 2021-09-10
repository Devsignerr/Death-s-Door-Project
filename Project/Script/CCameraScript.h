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
    static bool  m_IsCameraShake;

    static float       m_ShakeSpeed;   
    static float       m_ShakePower;
    static float       m_ShakeTime;

    float m_fSmoothStep; //선형보간 비율 
    Vec3  m_vCameraOffset; //플레이어에게서 카메라가 얼만큼 떨어져있는지 
    Vec3  m_vCameraRot; // 어떤 각도로 플레이어를 바라볼지 

public:
    static void SetCameraShake(float _ShakeTime, float _ShakeSpeed, float _ShakePower);

public:
    bool IsScaleTrans() { return m_IsOrthoScaleTrans; }
    void LookAtPlayer();
public:

    void CutSceneCamera();
    void ResetOriginCamera();
    void EndingSCeneCamera();
    void OrthoScaleSmoothTrans(Vec2 _Scale, float _SmoothSpeed);


    void OnOffCameraShake(bool _OnOff) { m_IsCameraShake = _OnOff; }
    void SetCameraShakeSpeed(float _Speed) { m_ShakeSpeed = _Speed; }
    void SetCameraShakePower(float _Power) { m_ShakePower = _Power; }

private:
    CGameObject* m_pDirLight;
    CAMERA_MODE  m_eMode;

    Vec3         m_OriginCameraPos;
    Vec3         m_OriginCameraRot;

    bool         m_IsOrthoScaleTrans;


public:
    virtual void update();
    virtual void awake();
public:
    CLONE(CCameraScript);
public:
    void CameraShake();
    void MakeLightFollow(Vec3 _MovePos);
private:
    void CameraFreeMove();
    void CameraFollowMove();


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);


public:
    CCameraScript();
    ~CCameraScript();
};

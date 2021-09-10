#include "pch.h"
#include "CCameraScript.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"
#include <Engine/CCamera.h>

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CEventMgr.h>

bool CCameraScript::m_IsCameraShake = false;
float CCameraScript::m_ShakeSpeed = 0.0f;
float CCameraScript::m_ShakePower = 0.0f;
float CCameraScript::m_ShakeTime = 0.0f;

CCameraScript::CCameraScript()
    : CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
    , m_pDirLight(nullptr)
    , m_eMode(CAMERA_MODE::FREE)
    , m_fSmoothStep(1.5f)
    , m_vCameraOffset(Vec3(3397.f,4882.f,-3135.f))
    , m_vCameraRot(Vec3(0.f, 0.f, 0.f))
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::SetCameraShake(float _ShakeTime, float _ShakeSpeed, float _ShakePower)
{
    m_IsCameraShake = true;
    m_ShakeTime = _ShakeTime;
    m_ShakeSpeed = _ShakeSpeed;
    m_ShakePower = _ShakePower;
}


void CCameraScript::update()
{
    if (m_pDirLight == nullptr)
        m_pDirLight = CSceneMgr::GetInst()->GetCurScene()->FindParentObj(L"Directional Light", (UINT)LAYER_TYPE::LIGHT);

    Vec2 vScale = Camera()->GetScale();

    if (KEY_HOLD(KEY_TYPE::NUM_1))
    {
        vScale += 1.f * fDT;
    }
    else if (KEY_HOLD(KEY_TYPE::NUM_2))
    {
        vScale -= 1.f * fDT;
    }

    if (KEY_TAP(KEY_TYPE::KEY_M))
    {
        if (m_eMode == CAMERA_MODE::FOLLOW)
        {
            m_eMode = CAMERA_MODE::FREE;
        }
        else
        {
            m_eMode = CAMERA_MODE::FOLLOW;

        }
    }

    if (m_eMode == CAMERA_MODE::FREE)
        CameraFreeMove();

    else if (m_eMode == CAMERA_MODE::FOLLOW)
        CameraFollowMove();

    if (true == m_IsCameraShake)
        CameraShake();

    if (true == m_IsOrthoScaleTrans)
        OrthoScaleSmoothTrans(Vec2(5.0f, 5.0f), 1.0f);
}


void CCameraScript::awake()
{
    m_pDirLight = CSceneMgr::GetInst()->GetCurScene()->FindParentObj(L"Directional Light", (UINT)LAYER_TYPE::LIGHT);

    Vec3 Pos = CPlayerScript::GetPlayer()->Transform()->GetLocalPos();
    Transform()->SetLocalPos(Pos+m_vCameraOffset);
    Transform()->finalupdate();

    LookAtPlayer();
}


void CCameraScript::CameraShake()
{
    m_ShakeTime -= fDT;

    static float AccTime = 0.0f;

    if (0.0f >= m_ShakeTime)
    {
        m_IsCameraShake = false;
    }
    else
    {
        Vec3 CameraPos = Transform()->GetLocalPos();
        AccTime += fDT * m_ShakeSpeed;
        Vec3 ShakeCameraPos = Vec3{ cos(AccTime) * m_ShakePower, cos(AccTime) * m_ShakePower,cos(AccTime) * m_ShakePower };

        CameraPos = DirectX::XMVectorAdd(CameraPos, ShakeCameraPos);
        // DirectX::XMMatrixLookToLH(CameraPos, Transform()->GetWorldDir(DIR_TYPE::FRONT), Transform()->GetWorldDir(DIR_TYPE::UP));

        Transform()->SetLocalPos(CameraPos);
    }

}

void CCameraScript::MakeLightFollow(Vec3 _MovePos)
{
    Vec3 LightPos = m_pDirLight->Transform()->GetLocalPos();
    LightPos += _MovePos;
    m_pDirLight->Transform()->SetLocalPos(LightPos);
}

void CCameraScript::CameraFreeMove()
{

    Vec3 vPos = Transform()->GetLocalPos();
    Vec3 vMovePos = {};

    if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
    {
        if (KEY_HOLD(KEY_TYPE::KEY_UP))
        {
            Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
            vMovePos += vFront * fDT * 2000.f;
        }

        if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
        {
            Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
            vMovePos += vBack * fDT * 2000.f;
        }
    }

    else if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
    {
        if (KEY_HOLD(KEY_TYPE::KEY_UP))
        {
            Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
            vFront.y = 0.f;
            vMovePos += vFront * fDT * 2000.f;
        }

        if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
        {
            Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
            vBack.y = 0.f;
            vMovePos += vBack * fDT * 2000.f;
        }
    }



    if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
    {
        Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        vMovePos += vLeft * fDT * 2000.f;
    }

    if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
    {
        Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        vMovePos += vRight * fDT * 2000.f;
    }

    if (KEY_HOLD(KEY_TYPE::RBTN))
    {
        Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

        Vec3 vRot = Transform()->GetLocalRot();

        vRot.x -= vMouseDir.y * fDT * 1.5f;
        vRot.y += vMouseDir.x * fDT * 1.5f;

        Transform()->SetLocalRot(vRot);
    }

    Transform()->SetLocalPos(vPos + vMovePos);

    //카메라 따라오게 만들기 
    MakeLightFollow(vMovePos);
}

//플레이어 추적해서 따라간다
void CCameraScript::CameraFollowMove()
{
    //이번 프레임에 플레이어가 이동한 량을 알아낸다 
    Vec3 PlayerMovePos = CPlayerScript::GetPlayerMovePos();
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

    Vec3 CameraPos = Transform()->GetLocalPos();

    Vec3 DesirePos = PlayerPos + m_vCameraOffset;
    Vec3 SmoothPos = Vector3::Lerp(CameraPos, DesirePos, m_fSmoothStep * fDT);

    Transform()->SetLocalPos(SmoothPos);

    MakeLightFollow(PlayerMovePos);

    if (KEY_HOLD(KEY_TYPE::RBTN))
    {
        if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
        {
            Vec3 PlayerFront = CPlayerScript::GetPlayerFront();
            Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
            PlayerPos += PlayerFront * 300.0f;

            Vec3 CameraPos = Transform()->GetLocalPos();

            Vec3 Temp = PlayerFront * fDT * 1000.0f;
            CameraPos += Temp;
            Transform()->SetLocalPos(CameraPos);
        }

    }
}


void CCameraScript::LookAtPlayer()
{
    Vec3 CameraRot = Transform()->GetLocalRot();
    Vec3 CameraPos = Transform()->GetLocalPos(); 

    Vec3 PlayerPos = CPlayerScript::GetPlayer()->Transform()->GetLocalPos();

    Vec3 vCameraFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
    Vec3 vCameraUP =Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCameraRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

    Vec3 relativePos = PlayerPos - CameraPos;

    relativePos.y = 0.f;
    relativePos.Normalize();

    vCameraFront.y = 0.f;
    vCameraFront.Normalize();

    Vec3 vCross = relativePos.Cross(vCameraFront);

    float dot = vCross.Dot(vCameraUP);

    float RotAngle = relativePos.Dot(vCameraFront);

    if (RotAngle > 1.f)
        RotAngle = 1.f;
    else if (RotAngle < 0.f)
        RotAngle = 0.f;

    RotAngle = acosf(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        CameraRot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        CameraRot.y += RotAngle;



    relativePos = PlayerPos - CameraPos;
    vCameraFront =Transform()->GetLocalDir(DIR_TYPE::FRONT);

    relativePos.x = 0.f;
    relativePos.Normalize();

    vCameraFront.x = 0.f;
    vCameraFront.Normalize();

    vCross = relativePos.Cross(vCameraFront);

    dot = vCross.Dot(vCameraRight);

    RotAngle = relativePos.Dot(vCameraFront);

    if (RotAngle > 1.f)
        RotAngle = 1.f;
    else if (RotAngle < 0.f)
        RotAngle = 0.f;

    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot >= 0.0)
        CameraRot.x -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        CameraRot.x += RotAngle;

    Transform()->SetLocalRot(CameraRot);
}

void CCameraScript::CutSceneCamera()
{
    if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
    {
        m_OriginCameraPos = Transform()->GetLocalPos();
        m_OriginCameraRot = Transform()->GetLocalRot();
    }

    Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

    // Transform은 바로 적용되지 않기때문에,
    // 카메라가 이동할 위치에서 플레이어를 바라보는 회전값을 구한 뒤 적용시킨다
    CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Cube", 0);
    Vec3 ObjPos = Obj->Transform()->GetLocalPos();

    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
    Vec3 Diff = PlayerPos - ObjPos;

    Vec3 vUP = Obj->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vFront = Obj->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    Vec3 vFrontCross = Diff.Cross(vFront);
    float Frontdot = vFrontCross.Dot(vUP);

    Vec3 Rot = Obj->Transform()->GetLocalRot();
    Diff.Normalize();

    float RotAngle = vFront.Dot(Diff);
    RotAngle = acos(RotAngle);

    // y축 회전
    if (Frontdot > 0.0)
        Rot.y -= RotAngle;
    else if (Frontdot < 0.0)
        Rot.y += RotAngle;

    // x축 회전
    Vec3 Down = -Obj->Transform()->GetLocalDir(DIR_TYPE::UP);
    float Dot = Down.Dot(Diff);
    Rot.x = Dot;

    Transform()->SetLocalPos(ObjPos);
    Transform()->SetLocalRot(Rot);

}

void CCameraScript::ResetOriginCamera()
{
    Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
    Transform()->SetLocalPos(m_OriginCameraPos);
    Transform()->SetLocalRot(m_OriginCameraRot);
}

void CCameraScript::EndingSCeneCamera()
{
}

void CCameraScript::OrthoScaleSmoothTrans(Vec2 _Scale, float _SmoothSpeed)
{
    Vec2 OriginScale = Camera()->GetScale();
    Vec2 DestScale = _Scale;

    Vec2 TransScale = Vec2::Lerp(OriginScale, DestScale, _SmoothSpeed * fDT);

    Vec2 DiffScale = DirectX::XMVectorSubtract(DestScale, TransScale);

    if (DiffScale.x <= 0.0f && DiffScale.y <= 0.0f)
        m_IsOrthoScaleTrans = false;

    Camera()->SetScale(TransScale);
}



void CCameraScript::SaveToScene(FILE* _pFile)
{
    CScript::SaveToScene(_pFile);
}

void CCameraScript::LoadFromScene(FILE* _pFile)
{
    CScript::LoadFromScene(_pFile);
}
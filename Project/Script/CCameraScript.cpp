#include "pch.h"
#include "CCameraScript.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"
#include <Engine/CCamera.h>

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CEventMgr.h>

bool CCameraScript::m_IsCameraShake = false;
float CCameraScript::m_ShakeSpeed = 100.0f;
float CCameraScript::m_ShakePower = 3.0f;

CCameraScript::CCameraScript()
    : CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
    , m_pDirLight(nullptr)
    , m_eMode(CAMERA_MODE::FREE)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::update()
{
    if (m_pDirLight == nullptr)
        m_pDirLight = CSceneMgr::GetInst()->GetCurScene()->FindParentObj(L"Directional Light", 0);

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

    if (KEY_TAP(KEY_TYPE::SPACE))
    {
        m_IsCameraShake = !m_IsCameraShake;
    }


    if (true == m_IsCameraShake)
        CameraShake();
}

void CCameraScript::start()
{
    m_pDirLight = CSceneMgr::GetInst()->GetCurScene()->FindParentObj(L"Directional Light", 0);
}

void CCameraScript::CameraShake()
{
    static float AccTime = 0.0f;
    Vec3 CameraPos = Transform()->GetWorldPos();

    AccTime += fDT * m_ShakeSpeed;
    Vec3 ShakeCameraPos = Vec3{ cos(AccTime) * m_ShakePower, cos(AccTime) * m_ShakePower,cos(AccTime) * m_ShakePower };

    CameraPos = DirectX::XMVectorAdd(CameraPos, ShakeCameraPos);
    DirectX::XMMatrixLookToLH(CameraPos, Transform()->GetWorldDir(DIR_TYPE::FRONT), Transform()->GetWorldDir(DIR_TYPE::UP));

    Transform()->SetLocalPos(CameraPos);
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

    Vec3 CameraPos = Transform()->GetLocalPos();
    CameraPos += PlayerMovePos;

    Transform()->SetLocalPos(CameraPos);

    MakeLightFollow(PlayerMovePos);
}

void CCameraScript::SetCameraPosRot(Vec3 _camPos, Vec3 _camRot)
{
}


void CCameraScript::SaveToScene(FILE* _pFile)
{
    CScript::SaveToScene(_pFile);
}

void CCameraScript::LoadFromScene(FILE* _pFile)
{
    CScript::LoadFromScene(_pFile);
}
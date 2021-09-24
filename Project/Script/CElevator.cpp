#include "pch.h"
#include "CElevator.h"

#include <Engine/CCollider3D.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>

void CElevator::awake()
{
    Vec3 Down = Vec3(0.f, -1.f, 0.f);
    m_DestPos = Transform()->GetLocalPos() + Down * m_Depth;

  
     CreateCol();
     vector<CGameObject*>& Temp = CResMgr::GetInst()->GetNavMeshVec();

     for (int i = 0; i < Temp.size(); ++i)
     {
         if (Temp[i]->GetName() == L"Room_ElevatorRoom2_Nav Child0")
         {
             m_Elevator2_Nav = Temp[i];
             break;
         }
     }

     //   m_Elevator2_Nav = GetObj()->GetChild()[0];

     m_Elevator2_Nav_OriginPos = m_Elevator2_Nav->Transform()->GetLocalPos();

     Vec3 Pos = m_Elevator2_Nav_OriginPos;
     Pos.y += 100000.0f;
     m_Elevator2_Nav->Transform()->SetLocalPos(Pos);
    
}

void CElevator::update()
{
    if (true == m_IsOn)
    {
        Vec3 Pos = Transform()->GetLocalPos();

        if (m_DestPos.y < Pos.y)
        {
            m_ChainSound = Play_RegionLoopSound(L"CageElevatorStart", 40000, 1, false, 0.5f);

            m_SoundTimer += fDT;
            if (4.0f < m_SoundTimer)
            {
                m_SoundTimer = 0.0f;
                m_ChainSound->Stop();
            }

            Pos.y -= fDT * 500.0f;
            Transform()->SetLocalPos(Pos);
        }
        else
        {
            m_ChainSound->Stop();
            Play_Sound(L"CageElevatorEnd", 1, false);
            m_Elevator2_Nav->Transform()->SetLocalPos(Vec3(0,-70.f,110.9f));
            m_IsOn = false;
        }
    }
}

void CElevator::CreateCol()
{
    Vec3 Pos = Transform()->GetLocalPos();

    CGameObject* Obj = new CGameObject;
    Obj->SetName(L"ElevatorCol");

    Obj->AddComponent(new CTransform);
    Obj->AddComponent(new CMeshRender);
    Obj->AddComponent(new CCollider3D);

    Obj->Transform()->SetLocalPos(Vec3(0.0f, -500.0f, 0.0f));
    Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

    Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
    Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
    CurScene->AddObject(Obj, (UINT)LAYER_TYPE::MAP_GIMIC_COL);

    GetObj()->AddChild(Obj);

    Obj->MeshRender()->Activate(false);
}

void CElevator::OnCollisionEnter(CGameObject* _pOther)
{
    CGameObject* Obj = _pOther;

    if ((UINT)LAYER_TYPE::PLAYER_COL == _pOther->GetLayerIndex())
    {
        m_IsOn = true;
        Ptr<CSound> Temp = Play_Sound(L"CageElevatorStart", 1, false);

        vector<CGameObject*> Child = GetGameObject()->GetChild();
        int Size = Child.size();

        for (int i = 0; i < Size; ++i)
        {
            if (Child[i]->Collider3D())
            {
                Child[i]->Collider3D()->Activate(false);
            }
        }
    }
}

void CElevator::OnCollision(CGameObject* _pOther)
{
}

void CElevator::OnCollisionExit(CGameObject* _pOther)
{
}

void CElevator::SaveToScene(FILE* _pFile)
{
    CScript::SaveToScene(_pFile);
}

void CElevator::LoadFromScene(FILE* _pFile)
{
    CScript::LoadFromScene(_pFile);
}

CElevator::CElevator()
    : m_IsOn(false)
    , m_Depth(14434.0f)
    , m_Elevator2_Nav(nullptr)
{
    m_iScriptType = (int)SCRIPT_TYPE::ELEVATOR;
    m_GimicType = GIMICTYPE::ELEVATOR;
}

CElevator::~CElevator()
{
}

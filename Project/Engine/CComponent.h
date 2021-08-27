#pragma once
#include "CEntity.h"
#include "CGameObject.h"

class CTransform;
class CMeshRender;
class CCamera;
class CAnimator2D;
class CAnimator3D;
class CCollider3D;

class CComponent :
    public CEntity
{
private:
    COMPONENT_TYPE  m_eComType;
    CGameObject*    m_pObject;

 protected:
    bool            m_bEnable;

public:
    bool IsEnable() { return m_bEnable; }

    //아래 함수는 최상위 부모가 대상이 아니라 자식에게 직접 명령을 내려야 한다 . 
    void Activate(bool _b) { m_bEnable = _b; }

public:
    virtual void awake() {}
    virtual void start() {}
    virtual void update() {};
    virtual void lateupdate() {};
    virtual void finalupdate() = 0;

public:
    COMPONENT_TYPE GetType() { return m_eComType; }
    CGameObject* GetGameObject() { return m_pObject; };

    CTransform* Transform() { return m_pObject->Transform(); }
    CMeshRender* MeshRender() { return m_pObject->MeshRender(); }
    CCollider2D* Collider2D() { return m_pObject->Collider2D(); }
    CCollider3D* Collider3D() { return m_pObject->Collider3D(); }
    CAnimator2D* Animator2D() { return m_pObject->Animator2D(); }
    CAnimator3D* Animator3D() { return m_pObject->Animator3D(); }
    CCamera* Camera() { return m_pObject->Camera(); }
    CLight2D* Light2D() { return m_pObject->Light2D(); }

    CGameObject* GetObj() { return m_pObject; }

private:
    void SetGameObject(CGameObject* _pObject) { m_pObject = _pObject; }
    
public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);
    virtual CComponent* Clone() = 0;

public:
    CComponent(COMPONENT_TYPE _eType);
    CComponent(const CComponent& _origin);
    ~CComponent();

    friend class CGameObject;
};


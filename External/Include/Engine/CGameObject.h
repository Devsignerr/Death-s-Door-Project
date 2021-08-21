#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CMeshRender;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CCamera;
class CParticleSystem;
class CLight2D;
class CLight3D;
class CTerrain;
class CFrustumSphere;

class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    vector<CScript*>        m_vecScript;
    vector<CGameObject*>    m_vecChild;
    CGameObject*            m_pParentObj;
    int                     m_iLayerIdx;

    bool                    m_bDead;
    bool                    m_bFrustum;
    bool                    m_bDynamicShadow;


public:
    void awake();
    void start();
    
    void update();
    void lateupdate();
    virtual void finalupdate();

    void finalupdate_ex(); // ���̾� ������� �ʴ� finalupdate

public:
    void AddComponent(CComponent* _pComponent);
       
    CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM];}
    CMeshRender* MeshRender() { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }
    CParticleSystem* ParticleSystem(){return (CParticleSystem*)m_arrCom[(UINT)COMPONENT_TYPE::PARTICLE];}
    CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
    CCollider2D* Collider2D() { return (CCollider2D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]; }
    CCollider3D* Collider3D() { return (CCollider3D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER3D]; }
    CLight2D* Light2D() { return (CLight2D*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]; }
    CLight3D* Light3D() { return (CLight3D*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT3D]; }
    CAnimator2D* Animator2D() { return (CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]; }
    CAnimator3D* Animator3D() { return (CAnimator3D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR3D]; }
    CFrustumSphere* FrustumSphere() { return (CFrustumSphere*)m_arrCom[(UINT)COMPONENT_TYPE::FRUSTUMSPHERE]; }

    CTerrain* Terrain() {return (CTerrain*)m_arrCom[(UINT)COMPONENT_TYPE::TERRAIN];}
    CComponent* GetComponent(COMPONENT_TYPE _eType) {return m_arrCom[(UINT)_eType];}
    const vector<CScript*>& GetScripts() { return m_vecScript; }
    CScript* GetScript() { if (m_vecScript.empty()) return nullptr; return m_vecScript[0]; }
    CGameObject* GetParent() { return m_pParentObj; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    void SetDynamicShadow(bool _bTrue);
    bool IsDynamicShdow() { return m_bDynamicShadow; }

    void SetFrustumCheck(bool _b);
    bool IsFrustum() { return m_bFrustum; }

    bool IsDead() { return m_bDead; }
    void SetLayerIndex(int _iIdx) { m_iLayerIdx = _iIdx; }
    int GetLayerIndex() { return m_iLayerIdx; }

private:

    // �θ� ������Ʈ���� ���� ����
    void DisconnectWithParent(); 

    // �Ҽӵ� Layer �� ParentObj �� ���
    void RegisterAsParentObj();

    // �Ҽӵ� Layer ���� �θ�μ� ���� ����    
    void ReleaseAsParentObj();

public:
    // �ڽ� ������Ʈ �߰�
    void AddChild(CGameObject* _pChildObj);

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CGameObject);

public:
    CGameObject();
    CGameObject(CGameObject& _origin);
    ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};


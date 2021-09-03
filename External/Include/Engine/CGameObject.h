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
class CUI;

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
    bool                    m_bAttackColObj;
    bool                    m_bDamageColObj;

public:
    void awake();
    void start();
    
    void update();
    void lateupdate();
    virtual void finalupdate();

    void finalupdate_ex(); // 레이어 등록하지 않는 finalupdate

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
    CUI* UI() { return (CUI*)m_arrCom[(UINT)COMPONENT_TYPE::UI]; }

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

public:
    //아래 두 함수는 최상위 부모 오브젝트에 명령을 내린다.
    //그러면 자식을 탐색하여 해당 컴포넌트를 모두 끄거나 킨다.
    void SetAllColliderActive(bool _b);
    void SetAllMeshrenderActive(bool _b);

public:
    void ChangeLayerIdx(UINT _Idx);

    // 부모 오브젝트와의 연결 해제
    void DisconnectWithParent(); 

    // 소속된 Layer 에 ParentObj 로 등록
    void RegisterAsParentObj();

    // 소속된 Layer 에서 부모로서 연결 해제    
    void ReleaseAsParentObj();

public:
    // 자식 오브젝트 추가
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


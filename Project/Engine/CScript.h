#pragma once
#include "CComponent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CPrefab.h"
#include "CSound.h"
#include "Ptr.h"



enum class SCRIPT_DATA_TYPE
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
    PREFAB,
    STRING,
    MEMFUNC,
};

struct tDataDesc;

class CScript :
    public CComponent
{
protected:
    int                  m_iScriptType;
    vector<tDataDesc>    m_vecDesc;
    //map<wstring, float>  m_mapSound;


public:
    static void Instantiate(Ptr<CPrefab> _pPrefab, Vec3 _vWorldPos, int _iLayerIdx = 0);
    static void DeleteObject(CGameObject* _pTarget);
    static void AddChild(CGameObject* _pParent, CGameObject* _pChild);
    static void DisconnenctWithParent(CGameObject* _pTarget);

public:
    virtual void update();
    virtual void finalupdate() final {}
    virtual CScript* Clone() = 0;

    virtual void OnCollisionEnter(CGameObject* _pOther) {};
    virtual void OnCollision(CGameObject* _pOther) {};
    virtual void OnCollisionExit(CGameObject* _pOther) {};

protected:
    void AddDesc(const tDataDesc& _desc) { m_vecDesc.push_back(_desc); }

public:
    int GetScriptType() { return m_iScriptType; }
    const vector<tDataDesc>& GetDataDesc() { return m_vecDesc; }

    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile) {};

    Ptr<CSound> Play_Sound(wstring _wstr,int _iRoopCount=1, bool _bOverlap = false, float _Volume = 0.5f);
    Ptr<CSound> Play_RegionLoopSound(wstring _wstr, unsigned int StartPoint, int _iRoopCount = 1, bool _bOverlap = false, float _Volume = 0.5f);

public:
    CScript(int _iScriptType);
    CScript(const CScript& _origin);
    virtual ~CScript();

public:
    typedef void(CScript::* SCRIPT_MEMFUNC)(void);
};



struct tDataDesc
{
    SCRIPT_DATA_TYPE            eType;
    string                      strName;
    void* pData;
    CScript::SCRIPT_MEMFUNC     pMemFunc;

    tDataDesc(SCRIPT_DATA_TYPE _eType, const string& _strName, void* _pData, CScript::SCRIPT_MEMFUNC _pMemFunc = nullptr)
        : eType(_eType), strName(_strName), pData(_pData), pMemFunc(_pMemFunc)
    {}
};
#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject*    m_pProtoObj;

public:
    CGameObject*    Instantiate();

public:
    CGameObject* GetProtoObj() { return m_pProtoObj; }

public:
    virtual void Save(const wstring& _strRelativePath);

private:
    virtual void Load(const wstring& _strFilePath);

public:
    CPrefab();
    CPrefab(CGameObject* _pProto);
    ~CPrefab();
};


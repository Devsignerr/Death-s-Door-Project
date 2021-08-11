#pragma once

#include "CFBXLoader.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;

class CMeshData :
	public CRes
{
private:
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;



	//=======================================
	bool					  m_bLoadFail;

public:
	const bool	IsLoadFail() { return m_bLoadFail; }
public:
	static void LoadFromFBX(const wstring& _strFilePath,FBXLOAD_TYPE _LoadType);
	virtual void Save(const wstring& _strRelativePath);
	virtual void Load(const wstring& _strFilePath);

	CGameObject* Instantiate(FBXLOAD_TYPE _Type = FBXLOAD_TYPE::ANIMATION_LOAD);

	CLONE_DISABLE(CMeshData);

public:
	CMeshData();
	virtual ~CMeshData();
};


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

	static vector<CMeshData*> m_pVecMeshData;
	bool					  m_bLoadFail;

public:

	const bool	IsLoadFail() { return m_bLoadFail; }
public:
	static vector<CMeshData*>& LoadFromFBX(const wstring& _strFilePath);

	virtual void Save(const wstring& _strRelativePath);
	virtual void Load(const wstring& _strFilePath);

	CGameObject* Instantiate();

	CLONE_DISABLE(CMeshData);

public:
	CMeshData();
	virtual ~CMeshData();
};


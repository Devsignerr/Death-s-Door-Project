#include "pch.h"
#include "CResMgr.h"

#include "CTexture.h"

CResMgr::CResMgr()
	: m_bEvent(false)
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		m_vecCloneRes[i].reserve(1000);
	}
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapDataTex);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Delete_Vector(m_vecCloneRes[i]);
	}	

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Delete_Map(m_mapRes[i]);
	}
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, UINT _eFlag, DXGI_FORMAT _eFormat, bool _bData)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);
	
	pTex = new CTexture;
	if (FAILED(pTex->Create(_iWidth, _iHeight, _eFlag, _eFormat)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"오류", MB_OK);
		return nullptr;
	}

	pTex->SetKey(_strKey);

	if (_bData)
	{
		m_mapDataTex.insert(make_pair(_strKey, pTex.Get()));
	}
	else
	{
		m_mapRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));
		m_bEvent = true;
	}
	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D, bool _bData)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture;
	if (FAILED(pTex->Create(_pTex2D)))
	{
		MessageBox(nullptr, L"텍스쳐 생성 실패", L"오류", MB_OK);
		return nullptr;
	}

	pTex->SetKey(_strKey);

	if (_bData)
	{
		m_mapDataTex.insert(make_pair(_strKey, pTex.Get()));
	}
	else
	{
		m_mapRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));
		m_bEvent = true;
	}

	
	return pTex;
}


Ptr<CTexture> CResMgr::FindDataTexture(const wstring& _strKey)
{
	map<wstring, CTexture*>::iterator iter = m_mapDataTex.find(_strKey);

	if (iter == m_mapDataTex.end())
		return nullptr;

	return iter->second;	
}

void CResMgr::LoadFBX(const wstring& _strPath, FBXLOAD_TYPE _LoadType)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName;

	vector<CMeshData*>& pMeshDatavec = GetMeshDataVec();
	vector<CMesh*>& pMeshvec = GetMeshVec();

	if (pMeshDatavec.size() > 0) 
	{
		pMeshDatavec.clear();
	}

	if (pMeshvec.size() > 0) 
	{
		pMeshvec.clear();
	}

	CMeshData::LoadFromFBX(_strPath, _LoadType);

	//메쉬데이터는 FBX파일 이름 + 인덱스 순서로 이름붙인다 
	for (UINT i = 0; i < m_pVecMeshData.size(); ++i)
	{
		wstring Name = strName;

		Name += std::to_wstring(i);

		Name += L".mdat";

		m_pVecMeshData[i]->SetKey(Name);
		m_pVecMeshData[i]->SetRelativePath(Name);
		m_mapRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(Name, m_pVecMeshData[i]));

		m_pVecMeshData[i]->Save(m_pVecMeshData[i]->GetRelativePath());
	}
}

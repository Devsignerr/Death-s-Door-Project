#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CMesh.h"
#include "CResMgr.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CGameObject.h"
#include "CAnimator3D.h"

vector<CMeshData*> CMeshData::m_pVecMeshData = {};

CMeshData::CMeshData()
{
}

CMeshData::~CMeshData()
{
}


CGameObject* CMeshData::Instantiate()
{
	// Mesh
	// Material

	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);

	pNewObj->MeshRender()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetMaterial(m_vecMtrl[i], i);
	}

	if (false == m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip((vector<tMTAnimClip>*)m_pMesh->GetAnimClip());

	return pNewObj;
}

vector<CMeshData*>& CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	int ContainerCnt = loader.GetContainerCount();

	// �޽� ��������
	vector<CMesh*> pVecMesh = CMesh::CreateFromContainer(loader, ContainerCnt);

	for (UINT j = 0; j < pVecMesh.size(); ++j) {
		// ResMgr �� �޽� ���
		wstring strMeshName = L"mesh\\";
		strMeshName += path(strFullPath).stem();
		strMeshName += std::to_wstring(j);
		strMeshName += L".mesh";

		pVecMesh[j]->SetName(strMeshName);
		pVecMesh[j]->SetRelativePath(strMeshName);

		CResMgr::GetInst()->AddRes<CMesh>(pVecMesh[j]->GetName(), pVecMesh[j]);

		vector<Ptr<CMaterial>> vecMtrl;

		// ���׸��� ��������
		for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
		{
			// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
			vecMtrl.push_back(pMtrl);
		}

		CMeshData* pMeshData = new CMeshData;
		pMeshData->m_pMesh = pVecMesh[j];
		pMeshData->m_vecMtrl = vecMtrl;

		m_pVecMeshData.push_back(pMeshData);
	}

	return m_pVecMeshData;
}

void CMeshData::Save(const wstring& _strFilePath)
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += _strFilePath;
	SetRelativePath(_strFilePath);

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFileName.c_str(), L"wb");

	// Mesh �� ���Ϸ� ����
	m_pMesh->Save(m_pMesh->GetRelativePath());

	// Mesh Key ����	
	// Mesh Data ����
	SaveWString(m_pMesh->GetKey(), pFile);
	SaveWString(m_pMesh->GetRelativePath(), pFile);

	// material ���� ����
	UINT iMtrlCount = (UINT)m_vecMtrl.size();
	fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

	UINT i = 0;
	for (; i < iMtrlCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
			continue;

		// Material �� ���Ϸ� ����
		m_vecMtrl[i]->Save(m_vecMtrl[i]->GetRelativePath());

		// Material �ε���, Key, Path ����
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveWString(m_vecMtrl[i]->GetKey(), pFile);
		SaveWString(m_vecMtrl[i]->GetRelativePath(), pFile);
	}

	i = -1; // ���� ��
	fwrite(&i, sizeof(UINT), 1, pFile);

	fclose(pFile);
}



void CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (NULL == pFile)
	{
		m_bLoadFail = true;
		//fclose(pFile);
		return;
	}
	

	// Mesh Load
	wstring strMeshKey, strMeshPath;
	LoadWString(strMeshKey, pFile);
	LoadWString(strMeshPath, pFile);
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey);
	if (nullptr == m_pMesh)
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(strMeshKey, strMeshPath);

	// material ���� �б�
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, pFile);

	m_vecMtrl.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		fread(&idx, 4, 1, pFile);
		if (idx == -1)
			break;

		wstring strKey, strPath;
		LoadWString(strKey, pFile);
		LoadWString(strPath, pFile);

		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strKey);
		if (nullptr == pMtrl)
			pMtrl = CResMgr::GetInst()->Load<CMaterial>(strKey, strPath);

		m_vecMtrl[i] = pMtrl;
	}

	fclose(pFile);
}


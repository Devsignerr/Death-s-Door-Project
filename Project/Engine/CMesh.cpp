#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CStructuredBuffer.h"
#include "CInstancingBuffer.h"
#include "CResMgr.h"

#include <algorithm>

CMesh::CMesh()
	: m_tVBDesc{}
	, m_iVtxCount(0)
	, m_pVtxSysMem(nullptr)
	, m_pBoneFrameData(nullptr)
	, m_pBoneOffset(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_pVtxSysMem)
		delete m_pVtxSysMem;

	for (int i = 0; i < (int)m_vecIdxInfo.size(); ++i)
	{
		if (nullptr != m_vecIdxInfo[i].pIdxSysMem)
		{
			delete m_vecIdxInfo[i].pIdxSysMem;
		}
	}

	SAFE_DELETE(m_pBoneFrameData);
	SAFE_DELETE(m_pBoneOffset);
}

void CMesh::Create(VTX* _pVTXSysmem, UINT _iVtxCount, UINT* _IdxSysmem, UINT _iIdxCount)
{
	m_iVtxCount = _iVtxCount;

	// GPU 메모리에 Vertex Buffer 만들기
	m_tVBDesc.ByteWidth = sizeof(VTX) * m_iVtxCount;	// GPU 에 생성할 버퍼 메모리 크기
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;				// 수정 불가능
	m_tVBDesc.CPUAccessFlags = 0;						// 수정 불가능
	m_tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// Vertex Buffer 용도

	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = _pVTXSysmem;							// 초기 데이터

	HRESULT hr = DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_pVB.GetAddressOf());

	// GPU 메모리에 Index Buffer 만들기
	tIndexInfo info = {};
	info.iIdxCount = _iIdxCount;
	info.tIBDesc.ByteWidth = sizeof(UINT) * info.iIdxCount;	// GPU 에 생성할 버퍼 메모리 크기
	info.tIBDesc.Usage = D3D11_USAGE_DEFAULT;				// 수정 불가능 버퍼
	info.tIBDesc.CPUAccessFlags = 0;						// 수정 불가능 버퍼
	info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;		// Index Buffer 용도

	tSubDesc = {};
	tSubDesc.pSysMem = _IdxSysmem;						// 초기 데이터

	hr = DEVICE->CreateBuffer(&info.tIBDesc, &tSubDesc, info.pIB.GetAddressOf());


	m_vecIdxInfo.push_back(info);
}

void CMesh::Reset(VTX* _pVTXSysmem, UINT _iVtxCount, UINT* _IdxSysmem, UINT _iIdxCount)
{
	m_pVB = nullptr;
	m_tVBDesc = {};
	m_iVtxCount = 0;

	m_vecIdxInfo.clear();

	Create(_pVTXSysmem, _iVtxCount, _IdxSysmem, _iIdxCount);
}


void CMesh::CreateFromContainer(CFBXLoader& _loader, UINT _ContainerCnt)
{
	UINT ContainerIdx = _ContainerCnt;

	for (UINT k = 0; k < ContainerIdx; ++k) {
		const tContainer* container = &_loader.GetContainer(k);

		UINT iVtxCount = (UINT)container->vecPos.size();

		D3D11_BUFFER_DESC tVtxDesc = {};

		tVtxDesc.ByteWidth = sizeof(VTX) * iVtxCount;
		tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
			tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
		VTX* pSys = (VTX*)tSub.pSysMem;
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			pSys[i].vPos = container->vecPos[i];
			pSys[i].vUV = container->vecUV[i];
			pSys[i].vColor = Vec4(0.1f, 0.1f, 0.8f, 1.f);
			pSys[i].vNormal = container->vecNormal[i];
			pSys[i].vTangent = container->vecTangent[i];
			pSys[i].vBinormal = container->vecBinormal[i];
			pSys[i].vWeights = container->vecWeights[i];
			pSys[i].vIndices = container->vecIndices[i];
		}

		ComPtr<ID3D11Buffer> pVB = NULL;
		if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
		{
			assert(false);
		}

		//글로벌 ID (entity ID) 가 증가하기 때문에 인스턴싱으로 인정이 안됨 
		//메쉬를 검색해서 같은 메쉬라면 재활용해줄수 있는 구조로 가야함 


		CMesh* pMesh = new CMesh;
		pMesh->m_pVB = pVB;
		pMesh->m_iVtxCount = iVtxCount;
		pMesh->m_tVBDesc = tVtxDesc;
		pMesh->m_pVtxSysMem = pSys;

		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)container->vecIdx.size();
		D3D11_BUFFER_DESC tIdxDesc = {};

		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
			tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
			if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
				tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			void* pSysMem = malloc(tIdxDesc.ByteWidth);
			memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
			tSub.pSysMem = pSysMem;

			ComPtr<ID3D11Buffer> pIB = nullptr;
			if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
			{
				assert(false);
			}

			tIndexInfo info = {};
			info.tIBDesc = tIdxDesc;
			info.iIdxCount = (UINT)container->vecIdx[i].size();
			info.pIdxSysMem = pSysMem;
			info.pIB = pIB;

			pMesh->m_vecIdxInfo.push_back(info);
		}

		// Animation3D
		if (!container->bAnimation)
			CResMgr::GetInst()->GetMeshVec().push_back(pMesh);

		vector<tBone*>& vecBone = _loader.GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			tMTBone bone = {};
			bone.iDepth = vecBone[i]->iDepth;
			bone.iParentIndx = vecBone[i]->iParentIndx;
			bone.matBone = GetMatrixFromFbxMatrix(vecBone[i]->matBone);
			bone.matOffset = GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
			bone.strBoneName = vecBone[i]->strBoneName;

			for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
			{
				tMTKeyFrame tKeyframe = {};
				tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
				tKeyframe.iFrame = j;
				tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
				tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
				tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

				tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
				tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
				tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

				tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
				tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
				tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
				tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

				bone.vecKeyFrame.push_back(tKeyframe);
			}

			iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

			pMesh->m_vecBones.push_back(bone);
		}

		vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			tMTAnimClip tClip = {};

			tClip.strAnimName = vecAnimClip[i]->strName;
			tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
			tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
			tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

			tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
			tClip.bFinish = false;
			tClip.bStay = true;
			tClip.eMode = vecAnimClip[i]->eMode;

			pMesh->m_vecAnimClip.push_back(tClip);
		}

		// Animation 이 있는 Mesh 경우 BoneTexture 만들어두기
		if (pMesh->IsAnimMesh())
		{
			// BoneOffet 행렬
			vector<Matrix> vecOffset;
			vector<tFrameTrans> vecFrameTrans;
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				vecOffset.push_back(pMesh->m_vecBones[i].matOffset);

				for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
				{
					vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
						= tFrameTrans{ Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate)
						, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale)
						, pMesh->m_vecBones[i].vecKeyFrame[j].qRot };
				}
			}
			pMesh->m_pBoneOffset = new CStructuredBuffer;
			pMesh->m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), vecOffset.data());

			pMesh->m_pBoneFrameData = new CStructuredBuffer;
			pMesh->m_pBoneFrameData->Create(sizeof(tFrameTrans)
				, (UINT)vecOffset.size() * iFrameCount
				, vecFrameTrans.data());

			CResMgr::GetInst()->GetMeshVec().push_back(pMesh);
		}
	}
}

void CMesh::UpdateData(UINT _iSubset)
{
	UINT iStride = sizeof(VTX);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render(UINT _iSubset)
{
	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubset].iIdxCount, 0, 0);
}

void CMesh::render_particle(UINT _iParticleCount)
{
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _iParticleCount, 0, 0, 0);
}

void CMesh::render_instancing(UINT _iSubset)
{
	if (_iSubset >= m_vecIdxInfo.size())
		assert(nullptr);

	ID3D11Buffer* arrBuffer[2] = { m_pVB.Get(), CInstancingBuffer::GetInst()->GetBuffer().Get() };
	UINT		  iStride[2] = { sizeof(VTX), sizeof(tInstancingData) };
	UINT		  iOffset[2] = { 0, 0 };

	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);

	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_iSubset].iIdxCount
		, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
}

void CMesh::Save(const wstring& _strRelativePath)
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += _strRelativePath;
	SetRelativePath(_strRelativePath);

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFileName.c_str(), L"wb");

	assert(pFile);

	// 키값
	SaveWString(GetName(), pFile);

	// 상대 경로
	SaveWString(GetRelativePath(), pFile);

	// 정점 데이터 저장			
	fwrite(&m_iVtxCount, sizeof(int), 1, pFile);

	int iByteSize = sizeof(VTX) * m_iVtxCount;
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	fwrite(m_pVtxSysMem, iByteSize, 1, pFile);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(int), 1, pFile);

	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		fwrite(&m_vecIdxInfo[i], sizeof(tIndexInfo), 1, pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem
			, m_vecIdxInfo[i].iIdxCount * GetSizeofFormat(DXGI_FORMAT_R32_UINT)
			, 1, pFile);
	}

	// Animation3D 정보 
	UINT iCount = (UINT)m_vecAnimClip.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecAnimClip[i].strAnimName, pFile);
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, pFile);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].bStay, sizeof(bool), 1, pFile);
	}

	iCount = (UINT)m_vecBones.size();
	fwrite(&iCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecBones[i].strBoneName, pFile);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	fwrite(&m_bNavMesh, sizeof(bool), 1, pFile);

	if (m_bNavMesh)
	{
		UINT NodeCnt = m_vecMeshNode.size();

		fwrite(&NodeCnt, sizeof(int), 1, pFile);

		for (UINT i = 0; i < NodeCnt; ++i)
		{
			fwrite(&m_vecMeshNode[i].NodeIdx, sizeof(int), 1, pFile); 

			fwrite(&m_vecMeshNode[i].VertexPosition[0], sizeof(Vec3), 1, pFile);
			fwrite(&m_vecMeshNode[i].VertexPosition[1], sizeof(Vec3), 1, pFile);
			fwrite(&m_vecMeshNode[i].VertexPosition[2], sizeof(Vec3), 1, pFile);

			UINT NearNodeCnt = m_vecMeshNode[i].VecNearNodeIdx.size();

			fwrite(&NearNodeCnt, sizeof(int), 1, pFile);

			for (UINT j = 0; j < NearNodeCnt; ++j)
			{
				fwrite(&m_vecMeshNode[i].VecNearNodeIdx[j], sizeof(int), 1, pFile);
			}		
		}	
	}


	fclose(pFile);
}

void CMesh::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strKey, strRelativePath;
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetKey(strKey);
	SetRelativePath(strRelativePath);

	// 정점데이터	
	fread(&m_iVtxCount, sizeof(int), 1, pFile);

	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);

	m_pVtxSysMem = (VTX*)malloc(iByteSize);
	fread(m_pVtxSysMem, 1, iByteSize, pFile);


	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = sizeof(VTX) * m_iVtxCount;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = m_pVtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_pVB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 인덱스 정보
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, pFile);

		UINT iByteWidth = info.iIdxCount * GetSizeofFormat(DXGI_FORMAT_R32_UINT);

		void* pSysMem = malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);

		tSubData.pSysMem = info.pIdxSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
		{
			assert(nullptr);
		}

		m_vecIdxInfo.push_back(info);
	}

	// Animation3D 정보 읽기

	int iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		LoadWString(tClip.strAnimName, pFile);
		fread(&tClip.dStartTime, sizeof(double), 1, pFile);
		fread(&tClip.dEndTime, sizeof(double), 1, pFile);
		fread(&tClip.dTimeLength, sizeof(double), 1, pFile);
		fread(&tClip.eMode, sizeof(int), 1, pFile);
		fread(&tClip.fUpdateTime, sizeof(float), 1, pFile);
		fread(&tClip.iStartFrame, sizeof(int), 1, pFile);
		fread(&tClip.iEndFrame, sizeof(int), 1, pFile);
		fread(&tClip.iFrameLength, sizeof(int), 1, pFile);
		fread(&tClip. bStay, sizeof(bool), 1, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i)
	{
		LoadWString(m_vecBones[i].strBoneName, pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		wstring strBone = GetName();

		// BoneOffet 행렬
		vector<Matrix> vecOffset;
		vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);

			for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTrans[(UINT)m_vecBones.size() * j + i]
					= tFrameTrans{ Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate)
					, Vec4(m_vecBones[i].vecKeyFrame[j].vScale)
					, Vec4(m_vecBones[i].vecKeyFrame[j].qRot) };
			}
		}

		m_pBoneOffset = new CStructuredBuffer;
		m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), vecOffset.data());

		m_pBoneFrameData = new CStructuredBuffer;
		m_pBoneFrameData->Create(sizeof(tFrameTrans)
			, (UINT)vecOffset.size() * (UINT)_iFrameCount
			, vecFrameTrans.data());
	}

	fread(&m_bNavMesh, sizeof(bool), 1, pFile);

	if (m_bNavMesh)
	{
		UINT NodeCnt = 0;

		fread(&NodeCnt, sizeof(int), 1, pFile);

		m_vecMeshNode.resize(NodeCnt);

		for (UINT i = 0; i < NodeCnt; ++i)
		{
			fread(&m_vecMeshNode[i].NodeIdx, sizeof(int), 1, pFile);
			
			fread(&m_vecMeshNode[i].VertexPosition[0], sizeof(Vec3), 1, pFile);
			fread(&m_vecMeshNode[i].VertexPosition[1], sizeof(Vec3), 1, pFile);
			fread(&m_vecMeshNode[i].VertexPosition[2], sizeof(Vec3), 1, pFile);

			UINT NearNodeCnt = 0;

			fread(&NearNodeCnt, sizeof(int), 1, pFile);

			m_vecMeshNode[i].VecNearNodeIdx.resize(NearNodeCnt);

			for (UINT j = 0; j < NearNodeCnt; ++j)
			{
				fread(&m_vecMeshNode[i].VecNearNodeIdx[j], sizeof(int), 1, pFile);
			}
		}
	}


	fclose(pFile);
}
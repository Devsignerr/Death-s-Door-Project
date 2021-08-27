#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

void CResMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultShader();

	CreateComputeShader();

	CreateDefaultMaterial();

	// FMOD �ʱ�ȭ
	InitSound();

	LoadPrefab();
}

void CResMgr::ClearCloneRes()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		vector<CRes*>::iterator iter = m_vecCloneRes[i].begin();
		for (iter; iter != m_vecCloneRes[i].end();)
		{
			if (0 == (*iter)->GetRefCount())
				iter = m_vecCloneRes[i].erase(iter);			
			else
				++iter;
		}
	}
}


void CResMgr::CreateDefaultMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	vector<VTX> vecVtx;
	vector<UINT> vecIdx;
	VTX v = {};

	// ==========
	// Point Mesh
	// ==========
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	vecVtx.push_back(v);
	vecIdx.push_back(0);

	// ����
	pMesh = new CMesh;	
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"PointMesh", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// =========
	// Rect Mesh
	// =========	
	// 0 - 1
	// | \ |
	// 3 - 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);	
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(2);
	vecIdx.push_back(0);
	vecIdx.push_back(1);

	// RectMesh ����
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	
	
	vecIdx.clear();

	// ====================
	// Rect Collider2D Mesh
	// ====================
	// 0 - 1
	// |   |
	// 3 - 2
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);	
		
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());	
	AddRes<CMesh>(L"RectMesh_C2D", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ============
	// Circle Mesh
	// ============
	float fRadius = 0.5f;
	UINT  iSliceCount = 40;
	float fTheta = 0.f;

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);

	// �߽� ��
	vecVtx.push_back(v);

	// �׵θ� ��
	for (int i = 0; i < (int)iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);		
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		v.vNormal = Vec3(0.f, 0.f, -1.f);
		vecVtx.push_back(v);

		fTheta += XM_2PI / iSliceCount;
	}

	// �ε���
	for (int i = 0; i < (int)iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);	
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);
	
	vecIdx.clear();

	// =====================
	// Collider2D CircleMesh
	// =====================
	vecVtx.erase(vecVtx.begin());

	// �ε���
	for (int i = 0; i < (int)iSliceCount; ++i)
	{
		vecIdx.push_back(i);		
	}
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	
	AddRes<CMesh>(L"CircleMesh_C2D", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// =========
	// Cube Mesh
	// =========
	VTX arrCube[24] = {};

	// ����
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// �Ʒ� ��	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// ���� ��
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// ������ ��
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// �� ��
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// �� ��
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// �ε���
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh;
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CubeMesh", pMesh);
	vecIdx.clear();

	{
		// =============
		// CubeMesh_C3D
		// =============

		VTX arrCube[24] = {};

		// ����
		arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
		arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
		arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
		arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
		arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);


		// �Ʒ� ��	
		arrCube[4].vPos = Vec3(0.5f, -0.5f, 0.5f);
		arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

		arrCube[5].vPos = Vec3(-0.5f, -0.5f, 0.5f);
		arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

		arrCube[6].vPos = Vec3(0.5f, -0.5f, -0.5f);
		arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

		arrCube[7].vPos = Vec3(-0.5f, -0.5f, -0.5f);
		arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

		// ���� ��
		arrCube[8].vPos = Vec3(-0.5f, 0.5f, -0.5f);
		arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

		arrCube[9].vPos = Vec3(-0.5f, 0.5f, 0.5f);
		arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

		arrCube[10].vPos = Vec3(-0.5f, -0.5f, 0.5f);
		arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

		arrCube[11].vPos = Vec3(-0.5f, -0.5f, -0.5f);
		arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

		// ������ ��
		arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
		arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

		arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
		arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

		arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
		arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

		arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
		arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

		// �� ��
		arrCube[16].vPos = Vec3(-0.5f, 0.5f, 0.5f);
		arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);

		arrCube[17].vPos = Vec3(0.5f, 0.5f, 0.5f);
		arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);

		arrCube[18].vPos = Vec3(0.5f, -0.5f, 0.5f);
		arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);

		arrCube[19].vPos = Vec3(-0.5f, -0.5f, 0.5f);
		arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);

		// �� ��
		arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
		arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

		arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
		arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

		arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
		arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

		arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
		arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

		////����
		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(2);
		vecIdx.push_back(3);
		vecIdx.push_back(0);

		////�Ʒ���
		vecIdx.push_back(5);
		vecIdx.push_back(4);
		vecIdx.push_back(6);
		vecIdx.push_back(7);
		vecIdx.push_back(5);

		//����
		vecIdx.push_back(0);
		vecIdx.push_back(3);
		vecIdx.push_back(0);
		vecIdx.push_back(5);
		vecIdx.push_back(7);
		vecIdx.push_back(3);

		//������
		vecIdx.push_back(2);
		vecIdx.push_back(1);
		vecIdx.push_back(4);
		vecIdx.push_back(6);
		vecIdx.push_back(2);

		//��
		vecIdx.push_back(1);
		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(4);
		vecIdx.push_back(5);
		vecIdx.push_back(0);

		//��
		vecIdx.push_back(3);
		vecIdx.push_back(2);
		vecIdx.push_back(6);
		vecIdx.push_back(7);
		vecIdx.push_back(3);


		pMesh = new CMesh;
		pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
		AddRes(L"CubeMesh_C3D", pMesh);
		vecIdx.clear();


	}

	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 1.f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // ���� ���� ����
	iSliceCount = 40; // ���� ���� ����

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();
			
			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// �ε���
	// �ϱ���
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ����
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ������
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetName(L"SphereMesh");
	AddRes(pMesh->GetName(), pMesh);

	vecVtx.clear();
	vecIdx.clear();


	{
		fRadius = 1.f;

		// Top
		v.vPos = Vec3(0.f, fRadius, 0.f);
		v.vUV = Vec2(0.5f, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();
		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vBinormal = Vec3(0.f, 0.f, 1.f);
		vecVtx.push_back(v);

		// Body
		UINT iStackCount = 40; // ���� ���� ����
		iSliceCount = 40; // ���� ���� ����

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
				v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
				v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
				v.vNormal = v.vPos;
				v.vNormal.Normalize();

				v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.vTangent.y = 0.f;
				v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
				v.vTangent.Normalize();

				v.vTangent.Cross(v.vNormal, v.vBinormal);
				v.vBinormal.Normalize();

				vecVtx.push_back(v);
			}
		}

		// Bottom
		v.vPos = Vec3(0.f, -fRadius, 0.f);
		v.vUV = Vec2(0.5f, 1.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();

		v.vTangent = Vec3(1.f, 0.f, 0.f);
		v.vBinormal = Vec3(0.f, 0.f, -1.f);
		vecVtx.push_back(v);

		// �ε���
		// �ϱ���
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}

		// ����
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// ������
		UINT iBottomIdx = (UINT)vecVtx.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(iBottomIdx);
			vecIdx.push_back(iBottomIdx - (i + 2));
			vecIdx.push_back(iBottomIdx - (i + 1));
		}

		pMesh = new CMesh;
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		pMesh->SetName(L"SphereMesh_C3D");
		AddRes(pMesh->GetName(), pMesh);

		vecVtx.clear();
		vecIdx.clear();
	}

}

void CResMgr::CreateDefaultShader()
{
	// InputLayout �ۼ�
	CGraphicsShader::AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, false);
	CGraphicsShader::AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, false);
	CGraphicsShader::AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, false);

	CGraphicsShader::AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, false);
	CGraphicsShader::AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, false);
	CGraphicsShader::AddInputLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, false);

	CGraphicsShader::AddInputLayout("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, false);
	CGraphicsShader::AddInputLayout("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, false);

	/*�ν��Ͻ� �����ʹ� ������ �ٸ���*/
	CGraphicsShader::AddInputLayout("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, true);

	CGraphicsShader::AddInputLayout("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, true);

	CGraphicsShader::AddInputLayout("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, true);
	CGraphicsShader::AddInputLayout("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, true);

	CGraphicsShader::AddInputLayout("ROWINDEX", 0, DXGI_FORMAT_R32_SINT, true);

	
	Ptr<CGraphicsShader> pShader;

	// ==============
	// Std3D Shader
	// ==============
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");
	

	// Shader Param		
	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"Albedo Map");
	pShader->AddShaderParam(SHADER_PARAM::TEX_1, L"Normal Map");

	pShader->AddShaderParam(SHADER_PARAM::TEXCUBE_0, L"CubeMap");

	AddRes<CGraphicsShader>(L"Std3DShader", pShader);

	// =====================
	// Std3D Deferred Shader
	// =====================
	pShader = new CGraphicsShader(SHADER_POV::DEFERRED);
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D_Deferred");


	// Shader Param		
	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"Albedo Map");
	pShader->AddShaderParam(SHADER_PARAM::TEX_1, L"Normal Map");
	pShader->AddShaderParam(SHADER_PARAM::TEX_2, L"Specular Map");
	pShader->AddShaderParam(SHADER_PARAM::TEX_3, L"Emissive Map");
	pShader->AddShaderParam(SHADER_PARAM::TEX_4, L"PaperBurn Map");

	pShader->AddShaderParam(SHADER_PARAM::TEXCUBE_0, L"CubeMap");

	AddRes<CGraphicsShader>(L"Std3D_DeferredShader", pShader);

	// =================
	// Fire Shader
	// =================
	pShader = new CGraphicsShader(SHADER_POV::DEFERRED);
	pShader->CreateVertexShader(L"Shader\\Fire.fx", "VS_Fire");
	pShader->CreatePixelShader(L"Shader\\Fire.fx", "PS_Fire");

	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"Noise Tex");
	pShader->AddShaderParam(SHADER_PARAM::TEX_1, L"Gradient Tex");

	AddRes(L"FireShader", pShader);

	// ==============
	// SkyBox Shader
	// ==============
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_SkyBox");

	// RS
	pShader->SetRSType(RS_TYPE::CULL_FRONT);

	// OM
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);

	// Shader Param	
	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"SkyBox Texture");
	pShader->AddShaderParam(SHADER_PARAM::TEXCUBE_0, L"SkyBox Cube Texture");

	AddRes<CGraphicsShader>(L"SkyBoxShader", pShader);

	// ================
	// Dir Light Shader
	// ================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_DirLight");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_DirLight");

	// RS
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	// BS
	pShader->SetBlendType(BLEND_TYPE::ONE_ONE);
	// DS
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
			
	AddRes<CGraphicsShader>(L"DirLightShader", pShader);

	// ==================
	// Point Light Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_PointLight");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_PointLight");

	// RS
	pShader->SetRSType(RS_TYPE::CULL_FRONT); // �ȼ� ���̴� 1ȸ ȣ��, ���� ���� ������ ī�޶� ���� �� ��� ���
	// BS
	pShader->SetBlendType(BLEND_TYPE::ONE_ONE);
	// DS
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	AddRes<CGraphicsShader>(L"PointLightShader", pShader);

	// ==================
	// Bloom Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\Bloom.fx", "VS_BLOOM");
	pShader->CreatePixelShader(L"shader\\Bloom.fx", "PS_BLOOM");

	// DS
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBlendType(BLEND_TYPE::ONE_ONE);

	AddRes<CGraphicsShader>(L"BloomShader", pShader);



	// ==================
	// Light Merge Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_LightMerge");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_LightMerge");
		
	// DS
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	AddRes<CGraphicsShader>(L"LightMergeShader", pShader);


	// ==============
	// Std2D Shader
	// ==============
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	// OM
	pShader->SetBlendType(BLEND_TYPE::ALPHABLEND);
	//pShader->SetDepthStencilType();

	// Shader Param	
	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"Output Texture");

	AddRes<CGraphicsShader>(L"Std2DShader", pShader);

	// ==================
	// Collider2D Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Collider2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Collider2D");
	pShader->SetDSType(DS_TYPE::NO_TEST);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddRes<CGraphicsShader>(L"Collider2DShader", pShader);

	// ==================
	// Collider3D Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Collider3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Collider3D");
	pShader->SetDSType(DS_TYPE::NO_TEST);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);

	AddRes<CGraphicsShader>(L"Collider3DShader", pShader);

	// ======================
	// Particle Render Shader
	// ======================
	pShader = new CGraphicsShader(SHADER_POV::PARTICLE);	

	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\std2d.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Particle");
	pShader->SetBlendType(BLEND_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::LESS_NO_WRITE);	
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->AddShaderParam(SHADER_PARAM::TEX_0, L"Fire Texture");

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);

	// ===============================
	// Deffered Particle Render Shader
	// ===============================
	pShader = new CGraphicsShader(SHADER_POV::DEFERRED_PARTICLE);

	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\std2d.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Deffered_Particle");
	pShader->SetBlendType(BLEND_TYPE::ONE_ONE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	AddRes<CGraphicsShader>(L"DefferedParticleRenderShader", pShader);

	// ==================
	// PostEffect Shader
	// ==================
	pShader = new CGraphicsShader(SHADER_POV::POSTEFFECT);
	pShader->CreateVertexShader(L"shader\\posteffect.fx", "VS_PostEffect");
	pShader->CreatePixelShader(L"shader\\posteffect.fx", "PS_PostEffect");
	pShader->SetDSType(DS_TYPE::NO_TEST);

	AddRes<CGraphicsShader>(L"PostEffectShader", pShader);

	// ========================
	// Tessellation Test Shader
	// ========================
	pShader = new CGraphicsShader(SHADER_POV::FORWARD);
	pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

	pShader->SetRSType(RS_TYPE::WIRE_FRAME);	
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	AddRes<CGraphicsShader>(L"TessTestShader", pShader);

	// ==============
	// Terrain Shader
	// ==============
	pShader = new CGraphicsShader(SHADER_POV::DEFERRED);
	pShader->CreateVertexShader(L"shader\\terrain.fx", "VS_Terrain");
	pShader->CreateHullShader(L"shader\\terrain.fx", "HS_Terrain");
	pShader->CreateDomainShader(L"shader\\terrain.fx", "DS_Terrain");
	pShader->CreatePixelShader(L"shader\\terrain.fx", "PS_Terrain");

	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	AddRes<CGraphicsShader>(L"TerrainShader", pShader);


	// =================
	// ShadowMap Shader
	// =================
	pShader = new CGraphicsShader(SHADER_POV::SHADOWMAP);
	pShader->CreateVertexShader(L"Shader\\shadowmap.fx", "VS_ShadowMap");
	pShader->CreatePixelShader(L"Shader\\shadowmap.fx", "PS_ShadowMap");

	AddRes(L"ShadowMapShader", pShader);
}

#include "CCopyShaderCS.h"
#include "CHorizontalBlur.h"
#include "CVerticalBlur.h"
#include "CParticleUpdateShader.h"
#include "CHeightMapShader.h"
#include "CWeightShader.h"
#include "CPickingShader.h"
#include "CCopyBoneMatrixShader.h"
#include "CAnimation3DShader.h"

void CResMgr::CreateComputeShader()
{
	Ptr<CComputeShader> pCS = nullptr;
	
	// CopyTextureShader
	pCS = new CCopyShaderCS;
	pCS->CreateComputeShader(L"shader\\CopyTexture.fx", "CS_CopyTex");
	AddRes<CComputeShader>(L"CopyTextureShader", pCS);

	// HoizontalBlurShader
	pCS = new CHorizontalBlur;
	pCS->CreateComputeShader(L"shader\\HorizontalBlur.fx", "CS_HorizontalBlur");
	AddRes<CComputeShader>(L"HorizontalBlurShader", pCS);

	// CopyTextureShader
	pCS = new CVerticalBlur;
	pCS->CreateComputeShader(L"shader\\VerticalBlur.fx", "CS_VerticalBlur");
	AddRes<CComputeShader>(L"VerticalBlurShader", pCS);

	// Particle Update Shader
	pCS = new CParticleUpdateShader;
	pCS->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pCS);

	// HeightMap Shader
	pCS = new CHeightMapShader;
	pCS->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
	AddRes<CComputeShader>(L"HeightMapShader", pCS);

	// WeightMap Shader
	pCS = new CWeightShader;
	pCS->CreateComputeShader(L"shader\\weightmap.fx", "CS_WeightMap");
	AddRes<CComputeShader>(L"WeightMapShader", pCS);

	// Picking Shader
	pCS = new CPickingShader;
	pCS->CreateComputeShader(L"shader\\picking.fx", "CS_Picking");
	AddRes<CComputeShader>(L"PickingShader", pCS);

	// Animation Update Shader
	pCS = new CAnimation3DShader;
	pCS->CreateComputeShader(L"shader\\animation.fx", "CS_Animation3D");
	AddRes<CComputeShader>(L"CAnimation3DUpdateShader", pCS);

	// CopyBoneMatrix Shader
	pCS = new CCopyBoneMatrixShader;
	pCS->CreateComputeShader(L"shader\\bonecopy.fx", "CS_CopyBoneMatrix");
	AddRes<CComputeShader>(L"CopyBoneMatrixShader", pCS);
}


void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// ===============
	// Std2D Material
	// ===============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	// ===============
	// Std3D Material
	// ===============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"Std3DMtrl", pMtrl);

	// ===============
	// Std3D Deferred Material
	// ===============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"Std3D_DeferredMtrl", pMtrl);
	
	// ==============
	// DirLight Mtrl
	// ==============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DirLightShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"DirLightMtrl", pMtrl);

	// ===============
	// PointLight Mtrl
	// ===============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PointLightShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"PointLightMtrl", pMtrl);

		
	// ==============
	// LightMerge Mtrl
	// ==============
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LightMergeShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"LightMergeMtrl", pMtrl);

	// ===============
	// SkyBox Shader
	// ===============
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));	
	AddRes<CMaterial>(L"SkyBoxMtrl", pMtrl);	

	// ===================
	// Collider2D Material
	// ===================
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Collider2DShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"Collider2DMtrl", pMtrl);

	// ===================
	// Collider3D Material
	// ===================
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Collider3DShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"Collider3DMtrl", pMtrl);



	// ========================
	// Particle Render Material
	// ========================
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);



	// =================================
	// Deffered Particle Render Material
	// =================================
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DefferedParticleRenderShader"));
	pMtrl->m_bDefault = true;
	AddRes<CMaterial>(L"DefferedParticleRenderMtrl", pMtrl);

	// ===================
	// PostEffect Material
	// ===================
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostEffectShader"));
	AddRes<CMaterial>(L"PostEffectMtrl", pMtrl);

	// ===================
	// TessTest Material
	// ===================
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TessTestShader"));
	AddRes<CMaterial>(L"TessTestMtrl", pMtrl);

	// ================
	// Terrain Material
	// ================
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TerrainShader"));
	AddRes<CMaterial>(L"TerrainMtrl", pMtrl);


	// ShadowmapMtrl
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ShadowMapShader"));
	AddRes<CMaterial>(L"ShadowMapMtrl", pMtrl);


	//FireMtrl
	pMtrl = new CMaterial;
	pMtrl->m_bDefault = true;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"FireShader"));
	AddRes<CMaterial>(L"FireMtrl", pMtrl);
}


void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32�� ä�� ����
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::LoadPrefab()
{
	std::wstring PrefabPath = CPathMgr::GetResPath();
	PrefabPath += L"prefab\\";
	CPathMgr::GetAllFilePath(PrefabPath, L"*.pref");
	CPathMgr::GetAllFileName(PrefabPath, L"*.pref");

	Ptr<CPrefab> Prefab = {};
	for (size_t i = 0; i < CPathMgr::GetVecFileName().size(); ++i)
	{
		std::wstring Path = L"prefab\\";
		Path += CPathMgr::GetVecFilePath()[i];
		Load<CPrefab>(CPathMgr::GetVecFileName()[i], Path);
	}
}

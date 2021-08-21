#pragma once
#include "global.h"


//===============
// Struct of FBX 
//===============
struct tFbxMaterial
{
	tMtrlInfo	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
};

struct tWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
};

struct tContainer
{
	wstring								strName;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation 관련 정보
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}
};

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring				strBoneName;
	int					iDepth;			// 계층구조 깊이
	int					iParentIndx;	// 부모 Bone 의 인덱스
	FbxAMatrix			matOffset;		// Offset 행렬( -> 뿌리 -> Local)
	FbxAMatrix			matBone;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};

//네비메쉬 정보 구조체
struct tNavMesh 
{
	//네비메쉬 정점 3개의 값 
	Vec3			  ArrPos[3];
	//로드된 순서 
	UINT			  iIdx;
	//인접 네비메쉬 벡터 
	vector<tNavMesh*> vecNearMesh;
};



class CMesh;

class CFBXLoader
{
private:
	FbxManager*						m_pManager;
	FbxScene*						m_pScene;
	FbxImporter*					m_pImporter;

	vector<tContainer>				m_vecContainer;	

	// Animation
	vector<tBone*>					m_vecBone;
	FbxArray<FbxString*>			m_arrAnimName;
	vector<tAnimClip*>				m_vecAnimClip;
	vector<tNavMesh>				m_vecNavMesh;

public:
	void CreateNavMesh(int _iIdx);

public:
	void init();
	void LoadFbx(const wstring& _strPath,FBXLOAD_TYPE _LoadType);

public:
	int GetContainerCount() { return (int)m_vecContainer.size(); }	
	const tContainer& GetContainer(int _iIdx) { return m_vecContainer[_iIdx]; }
	vector<tBone*>& GetBones() {return m_vecBone;}
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }
	vector<tNavMesh>& GetNevMeshVector() { return m_vecNavMesh; }
	
private:
	void LoadMeshDataFromNode(FbxNode* _pRoot, FBXLOAD_TYPE _LoadType);

	//네비메쉬 로드용 전용 함수 
	void LoadNavMesh(FbxMesh* _FbxMesh ,FbxString _MeshName);
	//애니메이션 로드용 전용 함수 
	void LoadMesh(FbxMesh* _pFbxMesh, FbxString _MeshName);
	//맵 로드용 전용 함수 
	void LoadMesh(FbxMesh* _pFbxMesh, FbxAMatrix _Mat, FbxString _MeshName);


	void LoadMaterial(FbxSurfaceMaterial* _pMtrlSur);

	void GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);

	Vec4 GetMtrlData(FbxSurfaceMaterial * _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
	wstring GetMtrlTextureName(FbxSurfaceMaterial * _pSurface, const char* _pMtrlProperty);

	void LoadTexture();
	void CreateMaterial();

	// Animation
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void Triangulate(FbxNode* _pNode);

	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform
		, int _iBoneIdx, tContainer* _pContainer);

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransform(FbxNode* _pNode);

	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer * _pContainer);
	
public:
	Vec3 GetRotation(FbxVector4 _vec1, FbxVector4 _vec2 , FbxVector4 _vec3);


public:
	CFBXLoader();
	~CFBXLoader();
};


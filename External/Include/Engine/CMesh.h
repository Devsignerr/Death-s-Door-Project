#pragma once
#include "CRes.h"

#include "CFBXLoader.h"

class CStructuredBuffer;

struct tNavMeshNode 
{
    //네비메쉬 노드가 가지는 정점 3개의 위치 
    Vec3 VertexPosition[3] = {};
    //인접한 노드의 인덱스  
    vector<int> VecNearNodeIdx = {};
    //내 인덱스 
    UINT  NodeIdx=0;
};

struct tIndexInfo
{
    ComPtr<ID3D11Buffer> pIB;
    D3D11_BUFFER_DESC    tIBDesc;
    UINT				 iIdxCount;       
    void*                pIdxSysMem;
};


class CMesh :
    public CRes
{
private:  
    ComPtr<ID3D11Buffer>    m_pVB;
    D3D11_BUFFER_DESC       m_tVBDesc;
    UINT                    m_iVtxCount;
    void*                   m_pVtxSysMem;

    // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
    vector<tIndexInfo>		m_vecIdxInfo;
   
    // Animation3D 정보
    vector<tMTAnimClip>		m_vecAnimClip;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;   // 전체 본 프레임 정보
    CStructuredBuffer*      m_pBoneOffset;	    // 각 뼈의 offset 행렬

    //==========================
    
    //네비메쉬 속성의 메쉬인가 
    bool                     m_bNavMesh;

    //한개의 네비메쉬가 가지는 네비노드 벡터 .폴리곤의 갯수가 100개라면 , vector의 크기도 100이다 . 
    vector<tNavMeshNode>     m_vecMeshNode;

public:
    void SetNavMesh(bool _b) { m_bNavMesh = _b; }
    bool IsNavMesh() { return m_bNavMesh; }
public:
    static void CreateFromContainer(CFBXLoader& _loader, UINT _ContainerCnt);

    void Create(VTX* _pVTXSysmem, UINT _iVtxCount, UINT* _IdxSysmem, UINT _iIdxCount);    
    void Reset(VTX* _pVTXSysmem, UINT _iVtxCount, UINT* _IdxSysmem, UINT _iIdxCount);
    void UpdateData(UINT _iSubset);
    void render(UINT _iSubset);
    void render_particle(UINT _iParticleCount);
    void render_instancing(UINT _iSubset);

public:
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
    CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬	

    vector<tNavMeshNode>& GetMeshNodeVec() { return m_vecMeshNode; }

public:
    virtual void Save(const wstring& _strRelativePath);
    virtual void Load(const wstring& _strFilePath);

public:
    CMesh();
    ~CMesh();
};


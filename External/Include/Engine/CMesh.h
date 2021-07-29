#pragma once
#include "CRes.h"

#include "CFBXLoader.h"

class CStructuredBuffer;

struct NavMeshNode 
{
    Vec3 VertexPosition[3];
    list<NavMeshNode> NearNode;
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

    static vector<CMesh*>   m_pVecMesh;
    NavMeshNode             m_tMeshNode;

public:
    static vector<CMesh*>& CreateFromContainer(CFBXLoader& _loader, UINT _ContainerCnt);

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

public:
    virtual void Save(const wstring& _strRelativePath);
    virtual void Load(const wstring& _strFilePath);

public:
    CMesh();
    ~CMesh();
};


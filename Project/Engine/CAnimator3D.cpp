#include "pch.h"
#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CKeyMgr.h"

CAnimator3D::CAnimator3D()
	: m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iClipCount(0)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr) // UpdateData 출력용 버퍼 . 저장 x 
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_iChangeFrameIdx(-1)
	, m_fLerpTime(0.f)
	, m_fLerpUpdateTime(0.f)
	, m_iChangeClipIdx(0)
	, m_bLerping(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
}


void CAnimator3D::start()
{
	
}

void CAnimator3D::update()
{
}

void CAnimator3D::lateupdate()
{
	
}

void CAnimator3D::finalupdate()
{
	if (m_pVecClip == nullptr)
		m_pVecClip = (vector<tMTAnimClip>*)MeshRender()->GetMesh()->GetAnimClip();

	if (m_pVecBones == nullptr)
		m_pVecBones = MeshRender()->GetMesh()->GetBones();

	m_dCurTime = 0.f;
	
	if(CSceneMgr::GetInst()->GetCurScene()->GetState()==SCENE_STATE::PLAY)
	{				
		/*if (m_bLerping)
			ChangeAnimupdate();
		else*/
			Normalupdate();		
	}		
}

void CAnimator3D::Normalupdate()
{
	m_vecClipUpdateTime[m_iCurClip] += fDT;

	if (m_vecClipUpdateTime[m_iCurClip] > m_pVecClip->at(m_iCurClip).dTimeLength)
	{
		m_pVecClip->at(m_iCurClip).bFinish = true;
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}

	//at은 범위를 검사하여 범위 밖의 요소에 접근 시 예외처리를 발생
	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);	

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnimator3D::ChangeAnimupdate()
{
	m_vecClipUpdateTime[m_iChangeClipIdx] += fDT;

	if (m_vecClipUpdateTime[m_iChangeClipIdx] > m_pVecClip->at(m_iChangeClipIdx).dTimeLength)
	{
		m_pVecClip->at(m_iChangeClipIdx).bFinish = true;
		m_vecClipUpdateTime[m_iChangeClipIdx] = 0.f;
	}

	//at은 범위를 검사하여 범위 밖의 요소에 접근 시 예외처리를 발생
	m_dCurTime = m_pVecClip->at(m_iChangeClipIdx).dStartTime + m_vecClipUpdateTime[m_iChangeClipIdx];

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iChangeFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iChangeFrameIdx >= m_pVecClip->at(m_iChangeClipIdx).iFrameLength - 1)
		m_iNextFrameIdx = m_iChangeFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
	{
		m_iNextFrameIdx = m_iChangeFrameIdx + 1;
	}

	// 프레임간의 시간에 따른 비율을 구해준다.
	//m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;

	UpdateChangeAnimation();
}

void CAnimator3D::SetFrame(int iFrame)
{
	int iStartFrame = m_pVecClip->at(m_iCurClip).iStartFrame;

	int Frame = iFrame - iStartFrame;

	double updateTime = (double)Frame /(double)m_iFrameCount;

	m_vecClipUpdateTime[m_iCurClip] = updateTime;
}

void CAnimator3D::SetAnimClip(vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"CAnimation3DUpdateShader").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh); //m_pBoneFinalMatBuffer 의 카운트와 메쉬의 Bone 카운트가 다르다면 m_pBoneFinalMatBuffer를 본 카운트에 맞게 재생성

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer()); //전체 본프레임 정보 세팅 
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);	//g_int_0: BonCount	
		pUpdateShader->SetFrameIndex(m_iFrameIdx);  //g_int_1 : Frame Index
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		pUpdateShader->SetChangeFrameIndex(m_iChangeFrameIdx); //바꿀 애니메이션의 진행 프레임

		pUpdateShader->SetLerpTime(m_fLerpTime); //몇초에 걸쳐 선형보간 할지 

		// 업데이트 쉐이더 실행
		pUpdateShader->Excute(); // animation.fx 실행 

		if(m_fLerpUpdateTime<=0)
			m_iChangeFrameIdx = -1;

		m_bFinalMatUpdate = true;
	}

	// t12 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
	m_pBoneFinalMatBuffer->UpdateData(12);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;
		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::ChangeAnimation(int _iNextAnimIdx, float _fLerpTime)
{
	if (_iNextAnimIdx >= m_pVecClip->size())
		return;

	m_vecClipUpdateTime.at(m_iCurClip) = 0.f;
	m_pVecClip->at(m_iCurClip).bFinish = false;

	m_bLerping = true;
	m_fLerpTime = _fLerpTime;
	m_fLerpUpdateTime = 0.f;

	m_iChangeClipIdx = _iNextAnimIdx;
	m_iChangeFrameIdx = m_pVecClip->at(_iNextAnimIdx).dStartTime* m_iFrameCount;
}

void CAnimator3D::UpdateChangeAnimation()
{
	m_fLerpUpdateTime += fDT;

	if (m_fLerpUpdateTime >= m_fLerpTime)
	{
		m_bLerping = false;

		m_iCurClip = m_iChangeClipIdx;
		m_vecClipUpdateTime[m_iCurClip] = m_fLerpUpdateTime;

		m_iChangeClipIdx = -1;
		m_iChangeFrameIdx = -1;
		m_fLerpUpdateTime = 0.f;
		m_fLerpTime = 0.f;		
		
	}

	m_fRatio = 1.f- (m_fLerpTime - m_fLerpUpdateTime)/ m_fLerpTime; //0~1 값인데 시간 증가할수록 뒤 프레임이 더 영향받음 
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->CreateRW(sizeof(Matrix), iBoneCount, nullptr);
	}
}

void CAnimator3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	
	m_iClipCount = m_vecClipUpdateTime.size();

	fwrite(&m_iClipCount, sizeof(UINT), 1, _pFile);

	vector<float>::iterator iter = m_vecClipUpdateTime.begin();

	for (; iter != m_vecClipUpdateTime.end(); ++iter)
	{
		float fTime = (*iter);

		fwrite(&fTime, sizeof(float), 1, _pFile);
	}
	fwrite(&m_iCurClip, sizeof(UINT), 1, _pFile);
}

void CAnimator3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_iClipCount, sizeof(UINT), 1, _pFile);

	m_vecClipUpdateTime.resize(m_iClipCount);

	for (int i = 0; i < m_iClipCount; i++) 
	{
		float fTime = 0;

		fread(&fTime, sizeof(float), 1, _pFile);
		m_vecClipUpdateTime[i] = fTime;
	}
	fread(&m_iCurClip, sizeof(UINT), 1, _pFile);

}
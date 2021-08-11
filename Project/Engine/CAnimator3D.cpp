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
	, m_iFrameCount(24)
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
	, m_bParentObj(false)
	, m_bStaying(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(CAnimator3D& _origin) :
	CComponent(COMPONENT_TYPE::ANIMATOR3D),
	m_pVecBones(_origin.m_pVecBones), // 뼈 정보는 원본을 그대로 따라감 
	m_iClipCount(_origin.m_iClipCount), // 클립 카운트 정보도 원본 그대로 따라감 
	m_vecClipUpdateTime{}, //0부터 시작 
	m_vecFinalBoneMat{}, // 딱히 쓰이는데가 없는듯 
	m_iFrameCount(_origin.m_iFrameCount),
	m_iCurClip(0),
	m_iFrameIdx(0),
	m_iNextFrameIdx(0),
	m_fRatio(0),
	m_bFinalMatUpdate(false),
	m_bParentObj(_origin.m_bParentObj),
	m_bLerping(false),
	m_iChangeClipIdx(0),
	m_iChangeFrameIdx(-1),
	m_fLerpTime(0.f),
	m_fLerpUpdateTime(0.f)
{
	CAnimator3D* OriginAnim = (CAnimator3D*)&_origin;
	CMeshRender* pMeshRender = OriginAnim->MeshRender();

	//부모는 메쉬렌더가 없으니 자식만 진행함 
	if (!_origin.m_bParentObj)
	{
		Ptr<CMesh> pMesh = pMeshRender->GetMesh();
		SetBones(pMesh->GetBones());
		SetAnimClip((vector<tMTAnimClip>*)pMesh->GetAnimClip());
	}
	
	//만약 부모였다면 
	if (_origin.m_bParentObj)
	{
		vector< tMTAnimClip>* pVecAnimClip = _origin.GetMTAnimClip();

		UINT ClipSize = pVecAnimClip->size();

		m_vecClipUpdateTime.resize(ClipSize);

		for (UINT i =0; i<ClipSize;++i)
		{
			tMTAnimClip newClip = {};

			m_vecParentClip.push_back(newClip);

			m_vecParentClip[i].strAnimName = (*pVecAnimClip)[i].strAnimName;
			m_vecParentClip[i].iStartFrame = (*pVecAnimClip)[i].iStartFrame;
			m_vecParentClip[i].iEndFrame = (*pVecAnimClip)[i].iEndFrame;
			m_vecParentClip[i].iFrameLength = (*pVecAnimClip)[i].iFrameLength;

			m_vecParentClip[i].dStartTime = (*pVecAnimClip)[i].dStartTime;
			m_vecParentClip[i].dEndTime = (*pVecAnimClip)[i].dEndTime;
			m_vecParentClip[i].dTimeLength = (*pVecAnimClip)[i].dTimeLength;
			m_vecParentClip[i].fUpdateTime = (*pVecAnimClip)[i].fUpdateTime;

			m_vecParentClip[i].bFinish = (*pVecAnimClip)[i].bFinish;
			m_vecParentClip[i].bStay = (*pVecAnimClip)[i].bStay;

			m_vecParentClip[i].eMode = (*pVecAnimClip)[i].eMode;

		}
	}

	m_pBoneFinalMatBuffer= new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
}


void CAnimator3D::start()
{
	if (m_bParentObj) 
	{
		if (0 >= m_vecParentClip.size()) {
			Ptr<CMesh> mesh = GetObj()->GetChild()[0]->MeshRender()->GetMesh();
			GetObj()->Animator3D()->CopyAnimClip((vector<tMTAnimClip>*)mesh->GetAnimClip());
		}
	}
}

void CAnimator3D::update()
{
}

void CAnimator3D::lateupdate()
{
	
}

void CAnimator3D::finalupdate()
{
	if (!m_bParentObj)
	{
		if (m_pVecClip == nullptr)
			m_pVecClip = (vector<tMTAnimClip>*)MeshRender()->GetMesh()->GetAnimClip();

		if (m_pVecBones == nullptr)
			m_pVecBones = MeshRender()->GetMesh()->GetBones();
	}
	else 
	{	
		m_dCurTime = 0.f;

		if (CSceneMgr::GetInst()->GetCurScene()->GetState() == SCENE_STATE::PLAY)
		{					
			//만약 애니메이션 블렌중이면 여기로 들어온다 
			if (m_bLerping)
				ChangeAnimupdate();			
			else
			{
				//애니메이션이 끝 프레임에 도달했을시 유지할것으로 세팅했다면 여기로 들어온다
			if (m_bStaying)
				Stayingupdate();
			else 
				//그 외에 상황에서는 여기로 들어온다 
				Normalupdate();
			
			}			
		}
	}
}

void CAnimator3D::Normalupdate()
{	
	m_vecClipUpdateTime[m_iCurClip] += fDT;
	
	if (m_vecParentClip.at(m_iCurClip).bFinish == true)
	{
		m_vecParentClip.at(m_iCurClip).bFinish = false;		
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}

	else if (m_vecClipUpdateTime[m_iCurClip] > m_vecParentClip.at(m_iCurClip).dTimeLength)
	{
		m_vecParentClip.at(m_iCurClip).bFinish = true;
	}

	//at은 범위를 검사하여 범위 밖의 요소에 접근 시 예외처리를 발생
	m_dCurTime = m_vecParentClip.at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_vecParentClip.at(m_iCurClip).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);	

	
	vector<CGameObject*> vecChild = GetObj()->GetChild();

	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (nullptr==vecChild[i]->Animator3D())
			continue;

		vecChild[i]->Animator3D()->SetCurFrameIdx(m_iFrameIdx);
		vecChild[i]->Animator3D()->SetNextFrameIdx(m_iNextFrameIdx);
		vecChild[i]->Animator3D()->SetRatio(m_fRatio);
		vecChild[i]->Animator3D()->CheckFinalMatUpdate();
	}
	
}

void CAnimator3D::ChangeAnimupdate()
{
	m_vecClipUpdateTime[m_iChangeClipIdx] += fDT;

	if (m_vecClipUpdateTime[m_iChangeClipIdx] > m_vecParentClip.at(m_iChangeClipIdx).dTimeLength)
	{
		m_vecParentClip.at(m_iChangeClipIdx).bFinish = true;
		m_vecClipUpdateTime[m_iChangeClipIdx] = 0.f;
	}

	//at은 범위를 검사하여 범위 밖의 요소에 접근 시 예외처리를 발생
	m_dCurTime = m_vecParentClip.at(m_iChangeClipIdx).dStartTime + m_vecClipUpdateTime[m_iChangeClipIdx];

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iChangeFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iChangeFrameIdx >= m_vecParentClip.at(m_iChangeClipIdx).iFrameLength -1)
		m_iNextFrameIdx = m_iChangeFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
	{
		m_iNextFrameIdx = m_iChangeFrameIdx + 1;
	}

	vector<CGameObject*> vecChild = GetObj()->GetChild();
	
	for (int i = 0; i < vecChild.size(); i++)
	{
		if (nullptr == vecChild[i]->Animator3D())
			continue;

		vecChild[i]->Animator3D()->SetCurFrameIdx(m_iFrameIdx);
		//vecChild[i]->Animator3D()->SetCurFrameIdx(m_iNextFrameIdx);
		vecChild[i]->Animator3D()->SetChangeFrameIdx(m_iChangeFrameIdx);
		vecChild[i]->Animator3D()->CheckFinalMatUpdate();
	}

	UpdateChangeAnimation();
}

void CAnimator3D::Stayingupdate()
{
	if (m_vecClipUpdateTime[m_iCurClip] < m_vecParentClip.at(m_iCurClip).dTimeLength)
	{
		m_vecClipUpdateTime[m_iCurClip] += fDT;
	}

	//at은 범위를 검사하여 범위 밖의 요소에 접근 시 예외처리를 발생
	m_dCurTime = m_vecParentClip.at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_vecParentClip.at(m_iCurClip).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);


	vector<CGameObject*> vecChild = GetObj()->GetChild();

	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (nullptr == vecChild[i]->Animator3D())
			continue;

		vecChild[i]->Animator3D()->SetCurFrameIdx(m_iFrameIdx);
		vecChild[i]->Animator3D()->SetNextFrameIdx(m_iNextFrameIdx);
		vecChild[i]->Animator3D()->SetRatio(m_fRatio);
		vecChild[i]->Animator3D()->CheckFinalMatUpdate();
	}


}

void CAnimator3D::CopyAnimClip(vector<tMTAnimClip>* _vec)
{
	UINT ClipSize = _vec->size();

	m_vecParentClip.clear();
	m_vecClipUpdateTime.clear();

	m_vecClipUpdateTime.resize(ClipSize);

	for (UINT i = 0; i < ClipSize; ++i)
	{
		tMTAnimClip Clip = {};

		m_vecParentClip.push_back(Clip);

		m_vecParentClip[i].strAnimName = (*_vec)[i].strAnimName;
		m_vecParentClip[i].iStartFrame = (*_vec)[i].iStartFrame;
		m_vecParentClip[i].iEndFrame = (*_vec)[i].iEndFrame;
		m_vecParentClip[i].iFrameLength = (*_vec)[i].iFrameLength;

		m_vecParentClip[i].dStartTime = (*_vec)[i].dStartTime;
		m_vecParentClip[i].dEndTime = (*_vec)[i].dEndTime;
		m_vecParentClip[i].dTimeLength = (*_vec)[i].dTimeLength;
		m_vecParentClip[i].fUpdateTime = (*_vec)[i].fUpdateTime;

		m_vecParentClip[i].bFinish = (*_vec)[i].bFinish;
		m_vecParentClip[i].bStay = (*_vec)[i].bStay;

		m_vecParentClip[i].eMode = (*_vec)[i].eMode;
	}

}

void CAnimator3D::SetFrame(int iFrame)
{
	int iStartFrame = m_vecParentClip.at(m_iCurClip).iStartFrame;

	int Frame = iFrame - iStartFrame;

	double updateTime = (double)Frame /(double)m_iFrameCount;

	m_vecClipUpdateTime[m_iCurClip] = updateTime;
}

void CAnimator3D::SetAnimClip(vector<tMTAnimClip>* _vecAnimClip)
{
	if (m_bParentObj) 
	{
		CopyAnimClip(_vecAnimClip);
		ResizeClipUpdateVec(_vecAnimClip->size());
	}
	else 
	{
		m_pVecClip = _vecAnimClip;
		m_vecClipUpdateTime.resize(m_pVecClip->size());
	}
}

void CAnimator3D::UpdateData()
{		
	//애니메이션 객체 하위 메쉬 업데이트 
	
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

		// 업데이트 쉐이더 실행
		pUpdateShader->Excute(); // animation.fx 실행 	

		m_bFinalMatUpdate = true;
	}

	// t12 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
	m_pBoneFinalMatBuffer->UpdateData(12);
	
}

void CAnimator3D::SetClipTime(int _iClipIdx, float _fTime)
{
	 m_vecClipUpdateTime[_iClipIdx] = _fTime; 

	vector<CGameObject*> vecChild = GetObj()->GetChild();

	for (int i = 0; i < vecChild.size(); i++)
	{
		vecChild[i]->Animator3D()->GetvecClipUpdateTime()[_iClipIdx] = _fTime;
	}
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


void CAnimator3D::ChangeAnimation(int _iNextAnimIdx, float _fLerpTime, bool _Stay)
{
	if (_iNextAnimIdx >= m_vecParentClip.size())
		return;

	m_bStaying = _Stay;

	m_vecClipUpdateTime.at(m_iCurClip) = 0.f;
	m_vecParentClip.at(m_iCurClip).bFinish = false;

	m_bLerping = true;
	m_fLerpTime = _fLerpTime;
	m_fLerpUpdateTime = 0.f;

	m_iChangeClipIdx = _iNextAnimIdx;
	m_iChangeFrameIdx = m_vecParentClip.at(_iNextAnimIdx).dStartTime* m_iFrameCount;
}

void CAnimator3D::UpdateChangeAnimation()
{
	m_fLerpUpdateTime += fDT;

	vector<CGameObject*> vecChild = GetObj()->GetChild();

	if (m_fLerpUpdateTime >= m_fLerpTime)
	{
		m_bLerping = false;

		m_iCurClip = m_iChangeClipIdx;
		m_vecClipUpdateTime[m_iCurClip] = m_fLerpUpdateTime;

		m_iChangeClipIdx = -1;
		m_iChangeFrameIdx = -1;
		m_fLerpUpdateTime = 0.f;
		m_fLerpTime = 0.f;				
		
		for (int i = 0; i < vecChild.size(); i++)
		{
			if (nullptr == vecChild[i]->Animator3D())
				continue;

			vecChild[i]->Animator3D()->SetChangeFrameIdx(m_iChangeFrameIdx);
			vecChild[i]->Animator3D()->CheckFinalMatUpdate(true);
		}	
	}

	m_fRatio = 1.f- (m_fLerpTime - m_fLerpUpdateTime)/ m_fLerpTime; //0~1 값인데 시간 증가할수록 뒤 프레임이 더 영향받음 

	
	for (int i = 0; i < vecChild.size(); i++)
	{
		if (nullptr == vecChild[i]->Animator3D())
			continue;

		vecChild[i]->Animator3D()->SetRatio(m_fRatio);
	}
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
	fwrite(&m_bParentObj, sizeof(bool), 1, _pFile);
	fwrite(&m_iFrameCount, sizeof(int), 1, _pFile);

	if (m_bParentObj)
	{
		size_t Size = m_vecParentClip.size();

		fwrite(&Size, sizeof(size_t), 1, _pFile);

		for (size_t i = 0; i < Size; ++i)
		{
			SaveWString(m_vecParentClip[i].strAnimName, _pFile);
			fwrite(&m_vecParentClip[i].iStartFrame, sizeof(int), 1, _pFile);
			fwrite(&m_vecParentClip[i].iEndFrame, sizeof(int), 1, _pFile);
			fwrite(&m_vecParentClip[i].iFrameLength, sizeof(int), 1, _pFile);

			fwrite(&m_vecParentClip[i].dEndTime, sizeof(double), 1, _pFile);
			fwrite(&m_vecParentClip[i].dStartTime, sizeof(double), 1, _pFile);
			fwrite(&m_vecParentClip[i].dTimeLength, sizeof(double), 1, _pFile);

			fwrite(&m_vecParentClip[i].eMode, sizeof(FbxTime::EMode), 1, _pFile);
		}

	}
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
	fread(&m_bParentObj, sizeof(bool), 1, _pFile);
	fread(&m_iFrameCount, sizeof(int), 1, _pFile);

	if (m_bParentObj)
	{
		size_t Size = 0;
		fread(&Size, sizeof(size_t), 1, _pFile);

		m_vecParentClip.clear();

		for (size_t i = 0; i < Size; ++i)
		{
			std::wstring strAniName = {};

			int StartFrame = 0;
			int EndFrame = 0;
			int FrameLength = 0;

			float UpdateTime = 0.0f;
			double EndTime = 0.0;
			double StartTime = 0.0;
			double TimeLength = 0.0;

			FbxTime::EMode Mode = {};

			LoadWString(strAniName, _pFile);

			fread(&StartFrame, sizeof(int), 1, _pFile);
			fread(&EndFrame, sizeof(int), 1, _pFile);
			fread(&FrameLength, sizeof(int), 1, _pFile);

			fread(&EndTime, sizeof(double), 1, _pFile);
			fread(&StartTime, sizeof(double), 1, _pFile);
			fread(&TimeLength, sizeof(double), 1, _pFile);

			fread(&Mode, sizeof(FbxTime::EMode), 1, _pFile);



			tMTAnimClip TempClip = { };
			TempClip.iEndFrame = EndFrame;
			TempClip.dEndTime = EndTime;
			TempClip.iFrameLength = FrameLength;
			TempClip.eMode = Mode;
			TempClip.iStartFrame = StartFrame;
			TempClip.dStartTime = StartTime;
			TempClip.strAnimName = strAniName;
			TempClip.dTimeLength = TimeLength;

			m_vecParentClip.push_back(TempClip);
		}
	}
}
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
	, m_pBoneFinalMatBuffer(nullptr) // UpdateData ��¿� ���� . ���� x 
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

	//at�� ������ �˻��Ͽ� ���� ���� ��ҿ� ���� �� ����ó���� �߻�
	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);	

	// ��ǻƮ ���̴� ���꿩��
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

	//at�� ������ �˻��Ͽ� ���� ���� ��ҿ� ���� �� ����ó���� �߻�
	m_dCurTime = m_pVecClip->at(m_iChangeClipIdx).dStartTime + m_vecClipUpdateTime[m_iChangeClipIdx];

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iChangeFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iChangeFrameIdx >= m_pVecClip->at(m_iChangeClipIdx).iFrameLength - 1)
		m_iNextFrameIdx = m_iChangeFrameIdx;	// ���̸� ���� �ε����� ����
	else
	{
		m_iNextFrameIdx = m_iChangeFrameIdx + 1;
	}

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	//m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	// ��ǻƮ ���̴� ���꿩��
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
		check_mesh(pMesh); //m_pBoneFinalMatBuffer �� ī��Ʈ�� �޽��� Bone ī��Ʈ�� �ٸ��ٸ� m_pBoneFinalMatBuffer�� �� ī��Ʈ�� �°� �����

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer()); //��ü �������� ���� ���� 
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);	//g_int_0: BonCount	
		pUpdateShader->SetFrameIndex(m_iFrameIdx);  //g_int_1 : Frame Index
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		pUpdateShader->SetChangeFrameIndex(m_iChangeFrameIdx); //�ٲ� �ִϸ��̼��� ���� ������

		pUpdateShader->SetLerpTime(m_fLerpTime); //���ʿ� ���� �������� ���� 

		// ������Ʈ ���̴� ����
		pUpdateShader->Excute(); // animation.fx ���� 

		if(m_fLerpUpdateTime<=0)
			m_iChangeFrameIdx = -1;

		m_bFinalMatUpdate = true;
	}

	// t12 �������Ϳ� ������� ������(��������) ���ε�	
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
		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
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

	m_fRatio = 1.f- (m_fLerpTime - m_fLerpUpdateTime)/ m_fLerpTime; //0~1 ���ε� �ð� �����Ҽ��� �� �������� �� ������� 
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
#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

struct Anim 
{
	wstring AnimName;
	int iStartFrame; 
	int iEndFrame;
};

class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*	    m_pVecBones;
	vector<tMTAnimClip>*        m_pVecClip;
	int							m_iClipCount;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // 클립 인덱스	

	int							m_iFrameIdx; // 클립의 현재 프레임
	int							m_iNextFrameIdx; // 클립의 다음 프레임
	float						m_fRatio;	// 프레임 사이 비율
		
	CStructuredBuffer*			m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
	bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

	//========aninmtion Lerp ==========
	bool						m_bLerping;
	int							m_iChangeClipIdx;
	int							m_iChangeFrameIdx;
	float						m_fLerpTime;
	float						m_fLerpUpdateTime;

	//=================================


public:
	int GetRatio() { return m_fRatio;}
	void SetCurFrameIdx(int _idx) { m_iFrameIdx = _idx; m_iNextFrameIdx =m_iFrameIdx+1; }
	void CheckFinalMatUpdate() { m_bFinalMatUpdate = false; }

	//=====================================

public:
	void SetFrame(int iFrame);
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
	void ClearData();
	void SetCurAnimIdx(int _idx) { m_iCurClip = _idx; }
public: 
	double GetCurTime() { return m_dCurTime; }
	int GetClipIdx() { return m_iCurClip; }
	int GetFrameIdx() { return m_iFrameIdx; }
	int GetFrameCount() { return m_iFrameCount; }

	vector<tMTAnimClip>* GetMTAnimClip() { return m_pVecClip; }
	const vector<tMTBone>* GetVecBones() { return m_pVecBones; }
	vector<float>& GetvecClipUpdateTime() { return m_vecClipUpdateTime; }

public : 
	void ChangeAnimation(int _iNextAnimIdx , float _fLerpTime);
	void UpdateChangeAnimation();
private:
	void check_mesh(Ptr<CMesh> _pMesh);

public:
	virtual void awake() {};
	virtual void start() ;
	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();

private:
	void Normalupdate();
	void ChangeAnimupdate();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CAnimator3D* Clone() { return new CAnimator3D(*this); }

public:
	CAnimator3D();
	virtual ~CAnimator3D();
};

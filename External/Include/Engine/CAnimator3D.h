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
	vector<tMTAnimClip>			m_vecParentClip;
	vector<tMTAnimClip>*        m_pVecClip;
	int							m_iClipCount;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // Ŭ�� �ε���	

	int							m_iFrameIdx; // Ŭ���� ���� ������
	int							m_iNextFrameIdx; // Ŭ���� ���� ������
	float						m_fRatio;	// ������ ���� ����
		
	CStructuredBuffer*			m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
	bool						m_bFinalMatUpdate; // ������� ���� ���࿩��

	//========aninmtion Lerp ==========
	bool						m_bParentObj;
	bool						m_bLerping;
	int							m_iChangeClipIdx;
	int							m_iChangeFrameIdx;
	float						m_fLerpTime;
	float						m_fLerpUpdateTime;

	//=================================
	bool						m_bStaying;

public:
	bool IsParent() { return m_bParentObj; }
	bool IsStaying() { return m_bStaying; }
	void SetStay(bool _b) { m_bStaying = _b; }
public:
	int GetRatio() { return m_fRatio;}
	void SetRatio(float Ratio) { m_fRatio = Ratio; }
	void SetChangeFrameIdx(int _idx) {m_iChangeFrameIdx = _idx;}
	void SetParent() { m_bParentObj = true; }
	void SetCurFrameIdx(int _idx) { m_iFrameIdx = _idx; }
	void SetNextFrameIdx(int _idx) { m_iNextFrameIdx = _idx; }
	void SetFrameCount(int _count) { m_iFrameCount = _count; }
	void CheckFinalMatUpdate(bool _b = false) { m_bFinalMatUpdate = _b; }

	void CopyAnimClip(vector<tMTAnimClip>* _vec);
	//=====================================

public:
	void SetFrame(int iFrame);
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();

	void SetClipTime(int _iClipIdx, float _fTime);

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
	void ClearData();
	void SetCurAnimIdx(int _idx){ m_iCurClip = _idx; }
public: 
	double GetCurTime() { return m_dCurTime; }
	int GetClipIdx() { return m_iCurClip; }
	int GetFrameIdx() { return m_iFrameIdx; }
	int GetFrameCount() { return m_iFrameCount; }

	vector<tMTAnimClip>* GetMTAnimClip() {return  &m_vecParentClip; }
	const vector<tMTBone>* GetVecBones() { return m_pVecBones; }
	vector<float>& GetvecClipUpdateTime() { return m_vecClipUpdateTime; }
	void ResizeClipUpdateVec(int _size){ m_vecClipUpdateTime.resize(_size); }
public : 
	void ChangeAnimation(int _iNextAnimIdx , float _fLerpTime , bool _Stay=false);
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
	void Stayingupdate();
public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CLONE(CAnimator3D);

public:
	CAnimator3D();
	CAnimator3D(CAnimator3D& _origin);
	virtual ~CAnimator3D();
};

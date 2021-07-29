#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader :
    public CComputeShader
{

private:
	tMtrlInfo			m_tInfo;			// g_int_0 : BonCount, g_int_1 : Frame Index
	CStructuredBuffer*	m_pFrameDataBuffer;	// t13
	CStructuredBuffer*	m_pOffsetMatBuffer;	// t14 
	CStructuredBuffer*	m_pOutputBuffer;	// u0

	/*ChangeFrame   g_int_3
	LerpTime      g_float_2*/
public:
	void SetBoneCount(int _iBoneCount) {       m_tInfo.arrInt[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) {       m_tInfo.arrInt[1] = _iFrameIdx; }	
	void SetNextFrameIdx(int _iFrameIdx) {     m_tInfo.arrInt[2] = _iFrameIdx; }
	void SetChangeFrameIndex(int _iFrameIdx) { m_tInfo.arrInt[3] = _iFrameIdx; }
	void SetFrameRatio(float _fFrameRatio) {   m_tInfo.arrFloat[0] = _fFrameRatio; }
	void SetLerpTime(float _fLerpTime) { m_tInfo.arrFloat[1] = _fLerpTime; }
	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();

public:
	virtual void Excute();

public:
	CAnimation3DShader();
	virtual ~CAnimation3DShader();
};


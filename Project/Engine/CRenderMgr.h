#pragma once

#include "CSingleton.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CLight2D.h"
#include "CLight3D.h"

class CStructuredBuffer;
class CCamera;
class CMRT;


class CRenderMgr
	: public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr)
private:
	vector<Ptr<CTexture>>	m_vecNoiseTex;
	int						m_iCurNoise;
	float					m_fAccTime;

	vector<CLight2D*>		m_vecLight2D;
	CStructuredBuffer*		m_pLight2DBuffer;

	vector<CLight3D*>		m_vecLight3D;
	CStructuredBuffer*		m_pLight3DBuffer;

	Ptr<CTexture>			m_pCopyTarget;		// swapchain backbuffer copy
	Ptr<CTexture>			m_pSwapChainTarget;     // swapchain 블러용 다운샘플 패스 타겟 
	vector<CCamera*>		m_vecCam;
	CCamera*				m_UICam;  // UI렌더 전용 카메라 
	CCamera*				m_pToolCam;			// Client 에서 셋팅

	CStructuredBuffer* m_pAnim2DBuffer;	// 2D Animation 정보 전달


	CMRT* m_arrMRT[(UINT)MRT_TYPE::END];




public:
	void init();
	void update();

	void render();

private:
	// Scene 이 재생
	void render_play();

	// 정지, 일시정지
	void render_tool();

	void render_shadowmap();

public:
	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D); }
	int RegisterLight3D(CLight3D* _pLight3D) { m_vecLight3D.push_back(_pLight3D);  return (int)m_vecLight3D.size() - 1; }
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterUICamera(CCamera* _pCam) { m_UICam = _pCam; }
	void RegisterToolCamera(CCamera* _pCam) { m_pToolCam = _pCam; }
	void CopyTarget(); // SwapChain backbuffer 를 후처리용 리소스 텍스쳐에 복사한다.

	CStructuredBuffer* GetAnim2DBuffer() { return m_pAnim2DBuffer; }
	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }

	CCamera* GetCurCam();
	CCamera* GetUICam() { return m_UICam; }

	void CustomCopy(Ptr<CTexture> _SrcTex, Ptr<CTexture> _DstTex , COPY_TYPE _eType);

private:
	void LightMerge();
	void UpdateLights(vector<tLightInfo>& _vec, CStructuredBuffer* _buffer, UINT _iRegisterNum);
	void CreateMRT();
	void ClearMRT();
};


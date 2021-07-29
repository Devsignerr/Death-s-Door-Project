#include "pch.h"
#include "Animator3DGUI.h"
#include "ListGUI.h"
#include "CImGUIMgr.h"

#include <Engine/CAnimator3D.h>
#include <Engine\CKeyMgr.h>
#include <Engine/CCore.h>
#include <Engine/CPathMgr.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <tchar.h>

#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTimeMgr.h>


#pragma region OriginRegion



//Animator3DGUI::Animator3DGUI():
//	ComponentGUI(COMPONENT_TYPE::ANIMATOR3D)
//{
//}
//
//Animator3DGUI::~Animator3DGUI()
//{
//}
//
//
//void Animator3DGUI::init()
//{
//}
//
//void Animator3DGUI::lateupdate()
//{
//	if (IsActive() && KEY_TAP(KEY_TYPE::ENTER))
//	{
//		ImGui::SetWindowFocus(nullptr); // 모든 ImGui window focus 해제
//	}
//}
//
//void Animator3DGUI::render()
//{
//	CAnimator3D* pCAnimator3D  = GetTargetObj()->Animator3D();
//
//	float dCurTime = pCAnimator3D->GetCurTime();
//	int iCurClipIdx = pCAnimator3D->GetClipIdx();
//
//	int iFrameCount = pCAnimator3D->GetFrameCount();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//	//const vector<tMTBone>* pVecBones = pCAnimator3D->GetVecBones();
//	vector<float>& vecClipUpdateTime = pCAnimator3D->GetvecClipUpdateTime();
//	float updateTime = vecClipUpdateTime[iCurClipIdx];
//
//	wstring			strAnimName = (*VecClip)[iCurClipIdx].strAnimName;
//	string strName = GetString(strAnimName);
//
//	int				iStartFrame = (*VecClip)[iCurClipIdx].iStartFrame;
//	int				iEndFrame = (*VecClip)[iCurClipIdx].iEndFrame;
//	int				iFrameLength = (*VecClip)[iCurClipIdx].iFrameLength;
//
//	float			dStartTime = (*VecClip)[iCurClipIdx].dStartTime;
//	float			dEndTime = (*VecClip)[iCurClipIdx].dEndTime;
//	float			dTimeLength = (*VecClip)[iCurClipIdx].dTimeLength;
//
//	static bool	    bRepeat = (*VecClip)[iCurClipIdx].bRepeat;
//
//	FbxTime::EMode	eMode = (*VecClip)[iCurClipIdx].eMode;
//
//
//
//
//	Start();
//
//	enum ContentsType { CT_Text, CT_FillButton };
//
//	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersH |
//		ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersInnerV
//		| ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner;
//
//	static bool display_headers = false;
//	static int contents_type = CT_Text;
//	
//	ImGui::TextColored(ImVec4(0.6f, 0.2f, 0.2f, 1.0f), "Animation Clip Info"); ImGui::NewLine();
//
//	if (ImGui::BeginTable("Animation Clip Info", 2, flags))
//	{
//
//		//anim Name ==============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("AnimName    \t"); ImGui::TableSetColumnIndex(1);
//		ImGui::TextUnformatted((char*)strName.c_str());
//		//========================================================================
//
//
//		//FrameLength  ===========================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("FrameLength \t"); ImGui::TableSetColumnIndex(1);
//
//		string str;
//		str = std::to_string(iFrameLength);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//		//TimeLength  ============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("TimeLength \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(dTimeLength);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//		//FrameCount  ============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("FrameCount \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(iFrameCount);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//
//		//StartFrame  ============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("StartFrame \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(iStartFrame);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//
//		//EndFrame   =============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("EndFrame \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(iEndFrame);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//		//StartTime  ============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("StartTime \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(dStartTime);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//
//
//		//EndTime   =============================================================
//		ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
//		ImGui::TextUnformatted("EndTime \t"); ImGui::TableSetColumnIndex(1);
//
//		str = std::to_string(dEndTime);
//		ImGui::TextUnformatted((char*)str.c_str());
//		//========================================================================
//
//		ImGui::EndTable();
//	}
//
//	ImGui::NewLine();
//
//	static bool CanModify = false;
//	ImGui::Checkbox("Modify on PlayMode", &CanModify);
//
//	ImGui::SameLine();
//
//	ImGui::Checkbox("Is Repeat", &bRepeat);
//	(*VecClip).at(iCurClipIdx).bRepeat = bRepeat;
//
//	ImGui::NewLine();
//	
//	if (CSceneMgr::GetInst()->GetCurScene()->GetState() != SCENE_STATE::PLAY) {
//		static int iFrameIdx = pCAnimator3D->GetFrameIdx();
//		ImGui::Text("CurFrameIdx \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::SliderInt("##iFrameIdx", &iFrameIdx, iStartFrame, iEndFrame);
//
//		if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { iFrameIdx--; }
//		ImGui::SameLine();
//		if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { iFrameIdx++; }
//
//		pCAnimator3D->SetFrame(iFrameIdx);
//	}
//	else 
//	{		
//		if (CanModify)
//		{
//			static int iFrameIdx = pCAnimator3D->GetFrameIdx();
//			ImGui::Text("CurFrameIdx \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::SliderInt("##iFrameIdx", &iFrameIdx, iStartFrame, iEndFrame);
//
//			if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { iFrameIdx--; }
//			ImGui::SameLine();
//			if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { iFrameIdx++; }
//
//			pCAnimator3D->SetFrame(iFrameIdx);
//		}
//		else 
//		{
//			int iFrameIdx = pCAnimator3D->GetFrameIdx();
//			ImGui::Text("CurFrameIdx \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::SliderInt("##iFrameIdx", &iFrameIdx, iStartFrame, iEndFrame);
//
//		}
//	}
//	
//	
//
//	ImGui::Text("CurClipIdx  \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::DragInt("##iCurClipIdx", &iCurClipIdx,0, VecClip->size());
//
//	ImGui::NewLine();
//
//	ImGui::Text("CurTime	 \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::DragFloat("##dCurTime", &dCurTime);								 						  													 						 								
//	ImGui::Text("updateTime  \t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::DragFloat("##updateTime", &updateTime);
//
//	pCAnimator3D->SetCurAnimIdx(iCurClipIdx);
//	ImGui::NewLine();
//
//
//	// ============= 애니메이션 선택 콤보박스 =================================
//
//	wstring wstrName = pCAnimator3D->GetMTAnimClip()->at(iCurClipIdx).strAnimName;
//	strName = string(wstrName.begin(), wstrName.end());
//
//	char szBuffer[100] = "";
//	strcpy_s(szBuffer, 100, strName.c_str());
//
//	ImGui::PushItemWidth(100);
//	ImGui::TextColored(ImVec4(0.6f, 0.2f, 0.2f, 1.0f), "Current Animation Name"); ImGui::NewLine();
//	ImGui::PushItemWidth(200);
//	ImGui::InputText("##Current Animation Name", szBuffer, 100, ImGuiInputTextFlags_ReadOnly);
//
//	if (ImGui::BeginDragDropTarget())
//	{
//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BeginDrag"))
//		{
//			// 드랍됨
//			int a = 0;
//		}
//		ImGui::EndDragDropTarget();
//	}
//
//
//	if (ImGui::Button("Select##1"))
//	{
//		ListGUI* pGUI = (ListGUI*)PopupListGUI();
//
//		pGUI->SetCaption("Animation");
//
//		vector<tMTAnimClip>::const_iterator iter = (*VecClip).begin();
//		for (; iter != (*VecClip).end(); ++iter)
//		{
//			pGUI->AddItem(string((*iter).strAnimName.begin(), (*iter).strAnimName.end()));
//		}
//	} 
//
//	// =========================================================================
//
//
//
//
//	ImGui::NewLine();
//
//	static char cAnimName[64];
//	static int iStartFrameIdx = 0;
//	static int iEndFrameIdx = 0;
//
//	ImGui::TextColored(ImVec4(0.6f, 0.2f, 0.2f, 1.0f), "Create New Anim"); ImGui::NewLine();
//
//	ImGui::Text("AnimationName\t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::InputText("##AnimationName", cAnimName,64);
//
//	wstring wAnimName(cAnimName,&cAnimName[64]);
//
//	ImGui::Text("StartFrameIdx\t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::InputInt("##StartFrameIdx", &iStartFrameIdx);
//	ImGui::Text("EndFrameIdx  \t");    ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::InputInt("##EndFrameIdx", &iEndFrameIdx);
//
//	ImGui::NewLine();
//
//	ImGui::PushItemWidth(150);
//
//	if (ImGui::Button("Create New Aniamtion")) 
//	{
//		CreateNewAnimation(wAnimName, iStartFrameIdx, iEndFrameIdx);
//	}
//
//	ImGui::SameLine();
//	ImGui::PushItemWidth(150);
//
//	if (ImGui::Button("Modify Aniamtion"))
//	{
//		ModifyAnimation(wAnimName, iStartFrameIdx, iEndFrameIdx);
//	}
//
//	ImGui::NewLine();
//
//	if (ImGui::Button("Save Aniamtion"))
//	{
//		SaveAnimation();
//	}
//
//	ImGui::SameLine();
//
//	if (ImGui::Button("Load Aniamtion"))
//	{
//		LoadAnimation();
//	}
//
//	ImGui::NewLine();
//
//	static char EraseName[64];
//	ImGui::Text("Animation Name\t");  ImGui::SameLine(); ImGui::PushItemWidth(150); ImGui::InputText("#EraseName", EraseName, 64);
//
//	if (ImGui::Button("Erase Aniamtion"))
//	{
//		
//		//EraseAnimation();
//	}
//
//	End();
//
//}
//
//void Animator3DGUI::CreateNewAnimation(wstring _AnimName , int StartFrameIdx, int EndFrameIdx)
//{
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	//0번째는 오리지널 애니메이션 클립 
//
//	//타임 = 프레임 / 프레임 카운트(30) 
//
//	int   FrameCount = pCAnimator3D->GetFrameCount(); //30 
//
//	int	  iStartFrame = (*VecClip)[0].iStartFrame; //애니메이션의 시작 프레임
//	int	  iEndFrame = (*VecClip)[0].iEndFrame; //애니메이션의 종료 프레임 
//	int	  iFrameLength = (*VecClip)[0].iFrameLength; //총 프레임 갯수 (endframe과 다를수 있음!)
//		  
//	float dStartTime = (*VecClip)[0].dStartTime; //해당 클립 애니메이션 재생 시간 
//	float dEndTime = (*VecClip)[0].dEndTime; //해당 클립 애니메이션 종료 시간 
//	float dTimeLength = (*VecClip)[0].dTimeLength; //애니메이션 총 재생 시간 
//
//	FbxTime::EMode	eMode = (*VecClip)[0].eMode;
//
//
//	//새로운 애니메이션 정보 
//
//	tMTAnimClip animclip = {};
//
//	animclip.strAnimName = _AnimName;
//
//	animclip.iStartFrame = StartFrameIdx;
//	animclip.iEndFrame = EndFrameIdx;
//	animclip.iFrameLength = EndFrameIdx - StartFrameIdx;
//
//	animclip.dStartTime = (double)StartFrameIdx / (double)FrameCount;
//	animclip.dEndTime = (double)EndFrameIdx / (double)FrameCount;
//	animclip.dTimeLength = (double)(EndFrameIdx - StartFrameIdx)/ (double)FrameCount;	
//
//	animclip.eMode = eMode;
//
//	VecClip->push_back(animclip);
//
//	pCAnimator3D->SetAnimClip(VecClip);
//}
//
//void Animator3DGUI::ModifyAnimation(wstring _AnimName, int StartFrameIdx, int EndFrameIdx)
//{
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	int iCurClipIdx = pCAnimator3D->GetClipIdx();
//
//	//0번째는 오리지널 애니메이션 클립 
//
//	//타임 = 프레임 / 프레임 카운트(30) 
//
//	int   FrameCount = pCAnimator3D->GetFrameCount(); //30 
//
//	(*VecClip)[iCurClipIdx].iStartFrame= StartFrameIdx; //애니메이션의 시작 프레임
//	(*VecClip)[iCurClipIdx].iEndFrame= EndFrameIdx; //애니메이션의 종료 프레임 
//	(*VecClip)[iCurClipIdx].iFrameLength= EndFrameIdx - StartFrameIdx; ; //총 프레임 갯수 (endframe과 다를수 있음!)
//
//	(*VecClip)[iCurClipIdx].dStartTime = (double)StartFrameIdx / (double)FrameCount; //해당 클립 애니메이션 재생 시간 
//	(*VecClip)[iCurClipIdx].dEndTime = (double)EndFrameIdx / (double)FrameCount; //해당 클립 애니메이션 종료 시간 
//	(*VecClip)[iCurClipIdx].dTimeLength = (double)(EndFrameIdx - StartFrameIdx) / (double)FrameCount; //애니메이션 총 재생 시간 
//}
//
//void Animator3DGUI::EraseAnimation(wstring _AnimName)
//{
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	vector<tMTAnimClip> ::iterator iter = (*VecClip).begin();
//
//	for (; iter != (*VecClip).end(); iter++) 
//	{
//		if (iter->strAnimName == _AnimName) 
//		{
//			(*VecClip).erase(iter);
//			pCAnimator3D->SetCurAnimIdx(0);
//			break;
//		}
//	}
//}
//
//GUI* Animator3DGUI::PopupListGUI()
//{
//	ListGUI* pListGUI = (ListGUI*)CImGUIMgr::GetInst()->FindGUI(L"ListGUI");
//	pListGUI->SetSelectCallBack(this, (LIST_SELECT_ITEM)&Animator3DGUI::SelectListItem);
//	pListGUI->Activate();
//
//	return pListGUI;
//}
//
//void Animator3DGUI::SelectListItem(ListGUI* _pListGUI, const char* _pSelectName)
//{
//	string strSel = _pListGUI->GetSelect();
//
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//	
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	int Idx = 0;
//
//	for (; Idx<(*VecClip).size(); Idx++)
//	{
//		if ((*VecClip).at(Idx).strAnimName == wstring(strSel.begin(), strSel.end()))
//		{
//			break;
//		}
//	}
//
//	pCAnimator3D->SetCurAnimIdx(Idx);
//
//	ImGui::SetWindowFocus(nullptr); // 모든 ImGui window focus 해제
//	
//}
//
//void Animator3DGUI::SaveAnimation()
//{
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	OPENFILENAME ofn = {};       // common dialog box structure
//	wchar_t szFile[260] = { 0 };       // if using TCHAR macros
//
//	wstring strMapFilePath = CPathMgr::GetResPath();
//	strMapFilePath += L"3DAnimation\\";
//
//	// Initialize OPENFILENAME   
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
//	ofn.lpstrFile = szFile;
//	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = _T("All\0*.*\0이미지\0*.bmp\0");
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	GetSaveFileName(&ofn);
//	wstring strOutput = ofn.lpstrFile;
//
//	// 파일 저장
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, strOutput.c_str(), L"wb");
//
//	if (!pFile)
//		return;
//
//	int ClipCount = (*VecClip).size();
//
//	fwrite(&ClipCount, sizeof(int), 1, pFile);
//
//	for (int idx = 0; idx < ClipCount; ++idx)
//	{		
//		wstring	strAnimName = (*VecClip)[idx].strAnimName;
//
//		int		iStartFrame = (*VecClip)[idx].iStartFrame;
//		int		iEndFrame = (*VecClip)[idx].iEndFrame;
//		int		iFrameLength = (*VecClip)[idx].iFrameLength;
//
//		float	dStartTime = (*VecClip)[idx].dStartTime;
//		float	dEndTime = (*VecClip)[idx].dEndTime;
//		float	dTimeLength = (*VecClip)[idx].dTimeLength;
//
//		bool	bRepeat = (*VecClip)[idx].bRepeat;
//
//		FbxTime::EMode	eMode = (*VecClip)[idx].eMode;
//
//		SaveWString(strAnimName, pFile);
//
//		fwrite(&iStartFrame, sizeof(int), 1, pFile);
//		fwrite(&iEndFrame, sizeof(int), 1, pFile);
//		fwrite(&iFrameLength, sizeof(int), 1, pFile);
//
//		fwrite(&dStartTime, sizeof(float), 1, pFile);
//		fwrite(&dEndTime, sizeof(float), 1, pFile);
//		fwrite(&dTimeLength, sizeof(float), 1, pFile);
//		fwrite(&bRepeat, sizeof(bool), 1, pFile);
//
//		fwrite(&eMode, sizeof(int), 1, pFile);
//	}
//
//	
//	fclose(pFile);
//}
//
//void Animator3DGUI::LoadAnimation()
//{
//	CAnimator3D* pCAnimator3D = GetTargetObj()->Animator3D();
//
//	vector<tMTAnimClip>* VecClip = (vector<tMTAnimClip>*)pCAnimator3D->GetMTAnimClip();
//
//	VecClip->clear();
//
//	OPENFILENAME ofn = {};       // common dialog box structure
//	wchar_t szFile[260] = { 0 };       // if using TCHAR macros
//
//	wstring strMapFilePath = CPathMgr::GetResPath();
//	strMapFilePath += L"3DAnimation\\";
//
//	// Initialize OPENFILENAME   
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
//	ofn.lpstrFile = szFile;
//	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = _T("All\0*.*\0이미지\0*.bmp\0");
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	GetOpenFileName(&ofn);
//	wstring strOutput = ofn.lpstrFile;
//
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, strOutput.c_str(), L"rb");
//	if (!pFile)
//		return;
//
//	int ClipCount = 0;
//
//	fread(&ClipCount, sizeof(int), 1, pFile);
//
//	if (ClipCount <= 1) 
//	{
//		fclose(pFile);
//		return;
//	}
//
//	for (int idx = 0; idx < ClipCount; ++idx)
//	{
//		wstring	strAnimName;
//
//		int		iStartFrame = 0;
//		int		iEndFrame = 0;
//		int		iFrameLength = 0;
//
//		float	dStartTime = 0;
//		float	dEndTime = 0;
//		float	dTimeLength = 0;
//
//		bool	bRepeat = false;
//
//		FbxTime::EMode	eMode;
//
//		LoadWString(strAnimName, pFile);
//
//		fread(&iStartFrame, sizeof(int), 1, pFile);
//		fread(&iEndFrame, sizeof(int), 1, pFile);
//		fread(&iFrameLength, sizeof(int), 1, pFile);
//		
//		fread(&dStartTime, sizeof(float), 1, pFile);
//		fread(&dEndTime, sizeof(float), 1, pFile);
//		fread(&dTimeLength, sizeof(float), 1, pFile);
//		fread(&bRepeat, sizeof(bool), 1, pFile);
//		fread(&eMode, sizeof(int), 1, pFile);
//
//		tMTAnimClip animclip = {};
//
//		animclip.strAnimName = strAnimName;
//
//		animclip.iStartFrame = iStartFrame;
//		animclip.iEndFrame = iEndFrame;
//		animclip.iFrameLength = iFrameLength;
//
//		animclip.dStartTime = dStartTime;
//		animclip.dEndTime = dEndTime;
//		animclip.dTimeLength = dTimeLength;
//		animclip.bRepeat = bRepeat;
//
//		animclip.eMode = eMode;
//
//		VecClip->push_back(animclip);
//	}
//
//	pCAnimator3D->SetAnimClip(VecClip);
//
//	fclose(pFile);
//}

#pragma endregion
#pragma region MyRegion

#define ANISTOP 0
#define ANIPLAY 1

#define STOPANI 0
#define PLAYANI 1
#define PAUSEANI 2


void Animator3DGUI::init()
{
}

void Animator3DGUI::lateupdate()
{

}

void Animator3DGUI::render()
{
	CAnimator3D* Animator3D = GetTargetObj()->Animator3D();
	Start();

	ImGui::TextColored(Vec4(0.0f, 1.0f, 0.0f, 1.0f), "Animation3D_Info");
	Render_Animation3DInfo(Animator3D);

	if (SCENE_STATE::STOP == CSceneMgr::GetInst()->GetCurScene()->GetState())
	{
		if (ImGui::Button("Frame Edit##Animation3DFrameEdit"))
			m_Open = !m_Open;

		if (true == m_Open)
			FrameEdit(Animator3D);
	}


	//if (true == m_Open)
	//{
	//	static int lines = 7;
	//	ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
	//	ImGui::Begin("Test");
	//	ImGui::BeginChild("scrolling", ImVec2(0.f,50.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	//	static int Length = Animator3D->GetVecClip()[0].data()->FrameLength;



	//	for (size_t i = 0; i < Length; ++i)
	//	{
	//		char LH[256] = { 0, };
	//		sprintf_s(LH, "Frame_%d", i);
	//		ImGui::SameLine();
	//		ImGui::Text(LH);
	//	}
	///*	for (int line = 0; line < lines; line++)
	//	{
	//		int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
	//		for (int n = 0; n < num_buttons; n++)
	//		{
	//			if (n > 0) ImGui::SameLine();
	//			ImGui::PushID(n + line * 1000);
	//			char num_buf[16];
	//			sprintf_s(num_buf, "%d", n);
	//			const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
	//			float hue = n * 0.05f;
	//			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
	//			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
	//			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
	//			ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
	//			ImGui::PopStyleColor(3);
	//			ImGui::PopID();
	//		}
	//	}*/
	//	//float scroll_x = ImGui::GetScrollX();
	//	//float scroll_max_x = ImGui::GetScrollMaxX();
	//	//ImGui::EndChild();
	//	//ImGui::PopStyleVar(2);
	//	//float scroll_x_delta = 0.0f;
	//	//ImGui::SmallButton("<<");
	//	//if (ImGui::IsItemActive())
	//	//	scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f;
	//	//ImGui::SameLine();
	//	//ImGui::Text("Scroll from code"); ImGui::SameLine();
	//	//ImGui::SmallButton(">>");
	//	//if (ImGui::IsItemActive())
	//	//	scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f;
	//	//ImGui::SameLine();
	//	//ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
	//	//if (scroll_x_delta != 0.0f)
	//	//{
	//	//	// Demonstrate a trick: you can use Begin to set yourself in the context of another window
	//	//	// (here we are already out of your child window)
	//	//	ImGui::BeginChild("scrolling");
	//	//	ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
	//		ImGui::EndChild();
	//		ImGui::End();
	//	//}
	//	//ImGui::Spacing();
	//}
	//

	End();
}

void Animator3DGUI::Render_Animation3DInfo(CAnimator3D* _TargetAni)
{
	//for (size_t i = 0; i < _TargetAni->GetVecClip()->size(); ++i)
	{
		int i = _TargetAni->GetClipIdx();
		if (ImGui::BeginTable("Animation3D_Info", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH))
		{
			std::wstring wName = _TargetAni->GetMTAnimClip()[i].data()->strAnimName;
			std::string Name = GetString(wName.c_str());

			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Clip Name");
			ImGui::TableNextColumn(); ImGui::Text(Name.c_str());

			std::string ModeName = ClipMode(_TargetAni->GetMTAnimClip()[i].data()->eMode);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Mode Name");
			ImGui::TableNextColumn(); ImGui::Text(ModeName.c_str());

			int CurClip = _TargetAni->GetClipIdx();
			char CC[256] = { 0, };
			sprintf_s(CC, "%d", CurClip);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Cur Clip Index");
			ImGui::TableNextColumn(); ImGui::Text(CC);

			int CurFrame = _TargetAni->GetFrameIdx();
			char CF[256] = { 0, };
			sprintf_s(CF, "%d", CurFrame);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Cur Frame");
			ImGui::TableNextColumn(); ImGui::Text(CF);

			double CurTime = _TargetAni->GetCurTime();
			char CT[256] = { 0, };
			sprintf_s(CT, "%lf", CurTime);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Cur Time");
			ImGui::TableNextColumn(); ImGui::Text(CT);

			float Ratio = _TargetAni->GetRatio();
			char Ra[256] = { 0, };
			sprintf_s(Ra, "%f", Ratio);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Ratio");
			ImGui::TableNextColumn(); ImGui::Text(Ra);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Animation3D_Info2", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH))
		{
			static UINT BoneCount = _TargetAni->GetBoneCount();
			char Count[256] = { 0, };
			sprintf_s(Count, "%d", BoneCount);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Bone Count");
			ImGui::TableNextColumn(); ImGui::Text(Count);

			static int FrameCount = _TargetAni->GetFrameCount();
			char FC[256] = { 0, };
			sprintf_s(FC, "%d", FrameCount);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Frame Count");
			ImGui::TableNextColumn(); ImGui::Text(FC);

			int ClipSize = (int)_TargetAni->GetMTAnimClip()->size();
			char CS[256] = { 0, };
			sprintf_s(CS, "%d", ClipSize);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Clip Size");
			ImGui::TableNextColumn(); ImGui::Text(CS);

			static int FrameLength = _TargetAni->GetMTAnimClip()[i].data()->iFrameLength;
			char FL[256] = { 0, };
			sprintf_s(FL, "%d", FrameLength);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Frame Length");
			ImGui::TableNextColumn(); ImGui::Text(FL);

			static double TimeLength = _TargetAni->GetMTAnimClip()[i].data()->dTimeLength;
			char TL[256] = { 0, };
			sprintf_s(TL, "%lf", TimeLength);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Time Length");
			ImGui::TableNextColumn(); ImGui::Text(TL);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Animation3D_Info3", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH))
		{
			static int StartFrame = _TargetAni->GetMTAnimClip()[i].data()->iStartFrame;
			char SF[256] = { 0, };
			sprintf_s(SF, "%d", StartFrame);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Start Frame");
			ImGui::TableNextColumn(); ImGui::Text(SF);

			static int EndFrame = _TargetAni->GetMTAnimClip()[i].data()->iEndFrame;
			char EF[256] = { 0, };
			sprintf_s(EF, "%d", EndFrame);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "End Frame");
			ImGui::TableNextColumn(); ImGui::Text(EF);

			static double StartTime = _TargetAni->GetMTAnimClip()[i].data()->dStartTime;
			char ST[256] = { 0, };
			sprintf_s(ST, "%lf", StartTime);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Start Time");
			ImGui::TableNextColumn(); ImGui::Text(ST);

			static double EndTime = _TargetAni->GetMTAnimClip()[i].data()->dEndTime;
			char ET[256] = { 0, };
			sprintf_s(ET, "%lf", EndTime);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "End Time");
			ImGui::TableNextColumn(); ImGui::Text(ET);

			ImGui::EndTable();
		}


		ImGui::NewLine();
	}

}

std::string Animator3DGUI::ClipMode(FbxTime::EMode _Mode)
{
	std::string ModeName = {};
	switch (_Mode)
	{
	case FbxTime::EMode::eDefaultMode: ModeName = "eDefaultMode"; break;
	case FbxTime::EMode::eFrames120: ModeName = "eFrames120"; break;
	case FbxTime::EMode::eFrames100: ModeName = "eFrames100"; break;
	case FbxTime::EMode::eFrames60: ModeName = "eFrames60"; break;
	case FbxTime::EMode::eFrames50: ModeName = "eFrames50"; break;
	case FbxTime::EMode::eFrames48: ModeName = "eFrames48"; break;
	case FbxTime::EMode::eFrames30: ModeName = "eFrames30"; break;
	case FbxTime::EMode::eFrames30Drop: ModeName = "eFrames30Drop"; break;
	case FbxTime::EMode::eNTSCDropFrame: ModeName = "eNTSCDropFrame"; break;
	case FbxTime::EMode::eNTSCFullFrame: ModeName = "eNTSCFullFrame"; break;
	case FbxTime::EMode::ePAL: ModeName = "ePAL"; break;
	case FbxTime::EMode::eFrames24: ModeName = "eFrames24"; break;
	case FbxTime::EMode::eFrames1000: ModeName = "eFrames1000"; break;
	case FbxTime::EMode::eFilmFullFrame: ModeName = "eFilmFullFrame"; break;
	case FbxTime::EMode::eCustom: ModeName = "eCustom"; break;
	case FbxTime::EMode::eFrames96: ModeName = "eFrames96"; break;
	case FbxTime::EMode::eFrames72: ModeName = "eFrames72"; break;
	case FbxTime::EMode::eFrames59dot94: ModeName = "eFrames59dot94"; break;
	case FbxTime::EMode::eModesCount: ModeName = "eModesCount"; break;
	}
	return ModeName;
}

void Animator3DGUI::FrameEdit(CAnimator3D* _TargetAni)
{
	static int CurPhase = 1;
	static int FrameSize = (100 * CurPhase);
	static int FirstFrame = FrameSize * (CurPhase - 1);
	static int Section[2] = { 0,_TargetAni->GetMTAnimClip()[0].data()->iFrameLength };
	if (_TargetAni->GetMTAnimClip()[0].data()->iFrameLength <= FrameSize)
	{
		FrameSize = _TargetAni->GetMTAnimClip()[0].data()->iFrameLength;
	}

	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Animation Modify", &m_Open, ImGuiWindowFlags_MenuBar))
	{
		//if (ImGui::BeginMenuBar())
		//{
		//	if (ImGui::BeginMenu("Frame Cutting"))
		//	{ 
		//		//if (ImGui::MenuItem("Close")) *&m_Open = false;
		//		ImGui::EndMenu();
		//	}
		//
		//	ImGui::EndMenuBar();
		//}

		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for (int i = FirstFrame; i < FrameSize; ++i)
			{
				char label[128];
				sprintf_s(label, "Frame %d", i);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();

		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Text("Frame: %d", selected);
			ImGui::Separator();

			{

				static int ModeIdx = 0;
				static bool Mode = false;
				static std::string CurMode = {};
				static bool ModeCheck = false;
				ImGui::Text("Cur State :");
				if (false == Mode)
				{
					if (ModeIdx == STOPANI)
						CurMode = "Stop";
					else if (ModeIdx == PAUSEANI)
						CurMode = "Puase";

				}
				else
				{
					CurMode = "Play";
				}

				ImGui::SameLine();
				ImGui::Text(CurMode.c_str());
				ImGui::SameLine(130);


				if (ModeIdx == PAUSEANI || ModeIdx == STOPANI)
				{
					if (ImGui::Button("Play"))
					{
						if (ModeIdx == STOPANI)
							selected = Section[0];
						Mode = true;

						if (ModeIdx == PLAYANI)
							ModeIdx = PAUSEANI;
						else
							ModeIdx = PLAYANI;

					}

				}
				else if (ModeIdx == PLAYANI)
				{
					if (ImGui::Button("Pause"))
					{
						Mode = false;
						if (ModeIdx == PAUSEANI)
							ModeIdx = PLAYANI;
						else
							ModeIdx = PAUSEANI;
					}
				}


				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					Mode = false;
					ModeIdx = 0;
				}

				ImGui::NewLine();
				static float OriginTime = 0.05f;
				static float Time = 0.05f;
				static int LastSelIdx = 0;
				static int RepeatOnOff = 0;
				static ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
				static bool SectionCheck = false;
				if (false == Mode)
				{
					_TargetAni->SetCurFrameIdx(selected);
					_TargetAni->UpdateData();
					_TargetAni->CheckFinalMatUpdate();
					_TargetAni->ClearData();

					ImGui::Text("Repeat Mode");
					ImGui::RadioButton("RepeatOff", &RepeatOnOff, 0);
					ImGui::SameLine();
					ImGui::RadioButton("RepeatOn", &RepeatOnOff, 1);
					ImGui::NewLine();

					ImGui::PushItemWidth(100);
					ImGui::Text("Section");
					ImGui::SameLine(100);


					ImGui::InputInt2("##AniSection", Section, flag);

					if (Section[0] < 0)
						Section[0] = 0;
					if (Section[1] > _TargetAni->GetMTAnimClip()[0].data()->iFrameLength)
						Section[1] = _TargetAni->GetMTAnimClip()[0].data()->iFrameLength;
					if (Section[0] >= _TargetAni->GetMTAnimClip()[0].data()->iFrameLength)
						Section[0] = _TargetAni->GetMTAnimClip()[0].data()->iFrameLength - 1;
					if (Section[0] > Section[1])
						Section[1] = Section[0] + 1;

					ImGui::SameLine();
					ImGui::Checkbox("Section Modify", &SectionCheck);
					if (true == SectionCheck)
					{
						flag = ImGuiInputTextFlags_None;
						ModeCheck = false;
					}
					else
					{
						flag = ImGuiInputTextFlags_ReadOnly;
						ModeCheck = true;
					}

					ImGui::PushItemWidth(50);
					ImGui::Text("Frame Step");
					ImGui::SameLine(100);
					ImGui::InputFloat("##AniStepTime", &OriginTime, 0, 0, "%.2f");
					Time = OriginTime;

					if (ModeIdx == PAUSEANI)
					{
						if (false == ModeCheck)
							selected = LastSelIdx;
					}
					else
					{
						if (false == ModeCheck)
							selected = Section[0];
					}

					if (false == ModeCheck)
					{
						static int Temp = -1;
						if (selected >= FrameSize && Temp != selected)
						{
							CurPhase = (selected / 100) + 1;
							FirstFrame = (CurPhase - 1) * 100;
							FrameSize = CurPhase * 100;
						}
						Temp = selected;


					}




				}
				else
				{
					ImGui::Text("Repeat Mode");
					ImGui::RadioButton("RepeatOff", &RepeatOnOff, 0);
					ImGui::SameLine();
					ImGui::RadioButton("RepeatOn", &RepeatOnOff, 1);
					ImGui::NewLine();

					ImGui::PushItemWidth(100);
					ImGui::Text("Section");
					ImGui::SameLine(100);
					ImGui::InputInt2("##AniSection", Section, ImGuiInputTextFlags_ReadOnly);

					ImGui::PushItemWidth(50);
					ImGui::Text("Frame Step");
					ImGui::SameLine(100);
					ImGui::InputFloat("##AniStepTime", &Time, 0, 0, "%.2f", ImGuiInputTextFlags_ReadOnly);


					Time -= CTimeMgr::GetInst()->GetfDT();

					if (0.0f > Time)
					{
						Time = OriginTime;
						++selected;

						if (RepeatOnOff == 0)
						{
							if (selected > Section[1])
								selected = Section[1];
						}
						else
						{
							if (selected > Section[1])
								selected = Section[0];
						}


						_TargetAni->SetCurFrameIdx(selected);
						_TargetAni->UpdateData();
						_TargetAni->CheckFinalMatUpdate();
						_TargetAni->ClearData();
						LastSelIdx = selected;
					}


					if (selected >= FrameSize || selected == Section[0])
					{
						CurPhase = (selected / 100) + 1;
						FirstFrame = (CurPhase - 1) * 100;
						FrameSize = CurPhase * 100;
					}
					ModeCheck = false;
				}

			}

			static int SelectedAni = -1;
			static std::string SelectAniName = {};
			static bool OpenName = false;


			if (ImGui::Button("Save"))
			{
				SaveAniFile();
			}

			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				LoadAniFile();
			}

			ImGui::SameLine();
			if (ImGui::Button("Register"))
			{
				OpenName = !OpenName;

			}
			if (true == OpenName)
			{
				ImGui::PushItemWidth(100);
				char T[256] = { 0, };
				if (ImGui::InputText("##SetAniName", T, IM_ARRAYSIZE(T), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					std::string TempName = T;
					std::wstring Name = std::wstring(TempName.begin(), TempName.end());
					CreateAnimation(Name, Section[0], Section[1]);
				}


				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					if (-1 != SelectedAni)
					{

						std::vector<tMTAnimClip>* VecClip = GetTargetObj()->Animator3D()->GetMTAnimClip();
						std::vector<tMTAnimClip>::iterator Iter = VecClip->begin();

						for (size_t i = 0; i < SelectedAni; ++i)
							++Iter;

						VecClip->erase(Iter);
					}
				}

			}







			// child begin
			{
				//ImGui::BeginGroup();
				ImGui::Separator();
				ImGui::BeginChild("Animation List", ImVec2(0, 100));
				std::vector<tMTAnimClip>* VecClip = GetTargetObj()->Animator3D()->GetMTAnimClip();
				std::vector<tMTAnimClip>::iterator Iter = VecClip->begin();

				for (int i = 0; Iter != VecClip->end(); ++Iter, ++i)
				{
					if (ImGui::Selectable(GetString(Iter->strAnimName), SelectedAni == i))
					{
						SelectedAni = i;
						SelectAniName = GetString(Iter->strAnimName);
						Section[0] = Iter->iStartFrame;
						Section[1] = Iter->iEndFrame;
						selected = Section[0];
					}

					ImGui::SameLine(200);
					char StartSection[256] = { 0, };
					sprintf_s(StartSection, "%d", Iter->iStartFrame);
					ImGui::Text(StartSection);
					ImGui::SameLine();
					ImGui::Text(" ~ ");
					ImGui::SameLine();
					char EndSection[256] = { 0, };
					sprintf_s(EndSection, "%d", Iter->iEndFrame);
					ImGui::Text(EndSection);


					//ImGui::
				}


				ImGui::EndChild();
				ImGui::Separator();
				//ImGui::EndGroup();

			}


			ImGui::EndChild();
			if (ImGui::Button("<"))
			{
				if (0 >= FirstFrame)
				{
					FirstFrame = 0;
					FrameSize = 100;
				}
				else if (FrameSize == _TargetAni->GetMTAnimClip()[0].data()->iFrameLength)
				{
					CurPhase -= 1;
					int LastPhase = (_TargetAni->GetMTAnimClip()[0].data()->iFrameLength / 100);
					FirstFrame = (LastPhase - 1) * 100;
					FrameSize = LastPhase * 100;
				}
				else
				{
					CurPhase -= 1;
					FirstFrame -= 100;
					FrameSize -= 100;
				}
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(30);
			int PrevPhase = CurPhase;
			ImGui::InputInt("##CurPhase", &CurPhase, 0, 0);

			ImGui::SameLine();
			ImGui::Text("/");

			ImGui::SameLine();
			char LP[256] = { 0, };
			static int LastPhase = (_TargetAni->GetMTAnimClip()[0].data()->iFrameLength / 100) + 1;
			sprintf_s(LP, "%d", LastPhase);
			ImGui::Text(LP);

			if (CurPhase != PrevPhase)
			{
				if (CurPhase <= 0)
				{
					CurPhase = 1;
					FirstFrame = (CurPhase - 1) * 100;
					FrameSize = CurPhase * 100;
				}
				else if (CurPhase > LastPhase)
				{
					CurPhase = LastPhase;
					FirstFrame = (CurPhase - 1) * 100;
					FrameSize = _TargetAni->GetMTAnimClip()[0].data()->iFrameLength;
				}
				else
				{
					FirstFrame = (CurPhase - 1) * 100;
					FrameSize = CurPhase * 100;
				}

			}



			/*	ImGui::SameLine();
				static int LastPhase = (_TargetAni->GetVecClip()[0].data()->FrameLength / 100) + 1;
				static int selected_Phase = 0;
				for (int i = 0; i < LastPhase; ++i)
				{
					char label[128];
					sprintf_s(label, "%d", i);
					if (ImGui::Selectable(label, selected_Phase == i, 0, ImVec2(15, 0)))
					{
						selected_Phase = i;
						CurPhase = selected_Phase;
						FrameSize = (100 * selected_Phase);
						FirstFrame = FrameSize * (selected_Phase - 1);
						break;
					}
					ImGui::SameLine();
				}*/

			ImGui::SameLine();

			if (ImGui::Button(">"))
			{
				if (_TargetAni->GetMTAnimClip()[0].data()->iFrameLength <= FrameSize)
				{
					FrameSize = _TargetAni->GetMTAnimClip()[0].data()->iFrameLength;
				}
				else
				{
					CurPhase += 1;
					FirstFrame += 100;
					FrameSize += 100;
				}
			}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}

void Animator3DGUI::SaveAniFile()
{
	std::wstring Path = CPathMgr::GetResPath();
	Path += L"AniData"; //

	OPENFILENAME ofn;
	static wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"All File\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"ani";

	std::wstring SaveFileName = {};
	if (GetSaveFileName(&ofn))
		SaveFileName = ofn.lpstrFile;

	FILE* File = nullptr;
	_wfopen_s(&File, SaveFileName.c_str(), L"wb");

	const std::vector<tMTAnimClip>* TempClip = GetTargetObj()->Animator3D()->GetMTAnimClip();

	size_t Size = TempClip->size();

	fwrite(&Size, sizeof(size_t), 1, File);

	for (size_t i = 0; i < Size; ++i)
	{
		std::wstring strAniName = (*TempClip)[i].strAnimName;

		int StartFrame = (*TempClip)[i].iStartFrame;
		int EndFrame = (*TempClip)[i].iEndFrame;
		int FrameLength = (*TempClip)[i].iFrameLength;

		double EndTime = (*TempClip)[i].dEndTime;
		double StartTime = (*TempClip)[i].dStartTime;
		double TimeLength = (*TempClip)[i].iFrameLength;

		FbxTime::EMode Mode = (*TempClip)[i].eMode;

		SaveWString(strAniName, File);

		fwrite(&StartFrame, sizeof(int), 1, File);
		fwrite(&EndFrame, sizeof(int), 1, File);
		fwrite(&FrameLength, sizeof(int), 1, File);

		fwrite(&EndTime, sizeof(double), 1, File);
		fwrite(&StartTime, sizeof(double), 1, File);
		fwrite(&TimeLength, sizeof(double), 1, File);

		fwrite(&Mode, sizeof(FbxTime::EMode), 1, File);
	}

	fclose(File);
}

void Animator3DGUI::LoadAniFile()
{
	std::wstring Path = CPathMgr::GetResPath();
	Path += L"AniData"; //

	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetWndHwnd();
	ofn.lpstrFilter = L"(*.*)";
	ofn.lpstrFile = fileName;
	ofn.lpstrInitialDir = Path.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = nullptr;

	std::wstring fileNameStr = {};

	std::wstring SaveFileName = {};
	if (GetOpenFileName(&ofn))
		SaveFileName = ofn.lpstrFile;

	FILE* File = nullptr;

	_wfopen_s(&File, SaveFileName.c_str(), L"rb");

	if (!File)
		return;

	size_t Size = 0;
	fread(&Size, sizeof(size_t), 1, File);

	if (Size <= 0)
	{
		fclose(File);
		return;
	}

	std::vector<tMTAnimClip>* TempVecClip = GetTargetObj()->Animator3D()->GetMTAnimClip();
	TempVecClip->clear();

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

		LoadWString(strAniName, File);

		fread(&StartFrame, sizeof(int), 1, File);
		fread(&EndFrame, sizeof(int), 1, File);
		fread(&FrameLength, sizeof(int), 1, File);

		fread(&EndTime, sizeof(double), 1, File);
		fread(&StartTime, sizeof(double), 1, File);
		fread(&TimeLength, sizeof(double), 1, File);

		fread(&Mode, sizeof(FbxTime::EMode), 1, File);



		tMTAnimClip TempClip = { };
		TempClip.iEndFrame = EndFrame;
		TempClip.dEndTime = EndTime;
		TempClip.iFrameLength = FrameLength;
		TempClip.eMode = Mode;
		TempClip.iStartFrame = StartFrame;
		TempClip.dStartTime = StartTime;
		TempClip.strAnimName = strAniName;
		TempClip.dTimeLength = TimeLength;

		TempVecClip->push_back(TempClip);
	}

	GetTargetObj()->Animator3D()->SetAnimClip(TempVecClip);

	fclose(File);


}

void Animator3DGUI::CreateAnimation(std::wstring _AniName, int _StartFrame, int _EndFrame)
{
	std::vector<tMTAnimClip>* VecClip = GetTargetObj()->Animator3D()->GetMTAnimClip();

	int FrameCount = GetTargetObj()->Animator3D()->GetFrameCount();

	FbxTime::EMode Mode = (*VecClip)[0].eMode;

	tMTAnimClip TempClip = { };
	TempClip.strAnimName = _AniName;
	TempClip.iStartFrame = _StartFrame;
	TempClip.iEndFrame = _EndFrame;

	TempClip.iFrameLength = _EndFrame - _StartFrame;
	TempClip.dStartTime = (double)_StartFrame / (double)FrameCount;
	TempClip.dEndTime = (double)_EndFrame / (double)FrameCount;
	TempClip.dTimeLength = (double)(_EndFrame - _StartFrame) / (double)FrameCount;

	TempClip.eMode = Mode;

	VecClip->push_back(TempClip);

	GetTargetObj()->Animator3D()->SetAnimClip(VecClip);
}




Animator3DGUI::Animator3DGUI()
	: ComponentGUI(COMPONENT_TYPE::ANIMATOR3D)
	, m_Open(false)
{
}

Animator3DGUI::~Animator3DGUI()
{
}


#pragma endregion

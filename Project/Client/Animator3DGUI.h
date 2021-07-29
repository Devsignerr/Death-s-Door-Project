
#pragma once
#include "ComponentGUI.h"

class ListGUI;

#pragma region OriginRegion

//class Animator3DGUI :
//    public ComponentGUI
//{
//private :
//
//
//public:
//    virtual void init();
//    virtual void lateupdate();
//    virtual void render();
//
//private:
//    void CreateNewAnimation(wstring _AnimName, int StartFrameIdx, int EndFrameIdx);
//    void ModifyAnimation(wstring _AnimName, int StartFrameIdx, int EndFrameIdx);
//    void EraseAnimation(wstring _AnimName);
//
//
//    GUI* PopupListGUI();
//    void SelectListItem(ListGUI* _pListGUI, const char* _pSelectName);
//
//    void SaveAnimation();
//    void LoadAnimation();
//public:
//    Animator3DGUI();
//    ~Animator3DGUI();
//
//};

#pragma endregion
#pragma region MyRegion
class CAnimator3D;
class Animator3DGUI :
	public ComponentGUI
{
private:
	bool m_Open;
private:
	void Render_Animation3DInfo(CAnimator3D* _TargetAni);
	std::string ClipMode(FbxTime::EMode _Mode);
	void FrameEdit(CAnimator3D* _TargetAni);
	void SaveAniFile();
	void LoadAniFile();
	void CreateAnimation(std::wstring _AniName, int _StartFrame, int _EndFrame);
public:
	virtual void init();
	virtual void lateupdate();
	virtual void render();

public:
	Animator3DGUI();
	~Animator3DGUI();
};
#pragma endregion




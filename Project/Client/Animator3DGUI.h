
#pragma once
#include "ComponentGUI.h"

class ListGUI;


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





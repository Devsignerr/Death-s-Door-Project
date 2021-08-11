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

	if (Animator3D->IsParent())
	{
		ImGui::TextColored(Vec4(0.0f, 1.0f, 0.0f, 1.0f), "Animation3D_Info");

		Render_Animation3DInfo(Animator3D);

		//if (SCENE_STATE::STOP == CSceneMgr::GetInst()->GetCurScene()->GetState())
		{
			if (ImGui::Button("Frame Edit##Animation3DFrameEdit"))
				m_Open = !m_Open;

			if (true == m_Open)
				FrameEdit(Animator3D);
		}
	}
	End();
}

void Animator3DGUI::Render_Animation3DInfo(CAnimator3D* _TargetAni)
{
	{
		int i = _TargetAni->GetClipIdx();
		if (ImGui::BeginTable("Animation3D_Info", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH))
		{
			std::wstring wName = _TargetAni->GetMTAnimClip()->at(i).strAnimName;
			std::string Name = GetString(wName.c_str());

			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Clip Name");
			ImGui::TableNextColumn(); ImGui::Text(Name.c_str());

			std::string ModeName = ClipMode(_TargetAni->GetMTAnimClip()->at(i).eMode);
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
			//static UINT BoneCount = _TargetAni->GetBoneCount();
			//char Count[256] = { 0, };
			//sprintf_s(Count, "%d", BoneCount);
			//ImGui::TableNextRow();
			//ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Bone Count");
			//ImGui::TableNextColumn(); ImGui::Text(Count);

			int FrameCount = _TargetAni->GetFrameCount();
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

			int FrameLength = _TargetAni->GetMTAnimClip()->at(i).iFrameLength;
			char FL[256] = { 0, };
			sprintf_s(FL, "%d", FrameLength);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Frame Length");
			ImGui::TableNextColumn(); ImGui::Text(FL);

			double TimeLength = _TargetAni->GetMTAnimClip()->at(i).dTimeLength;
			char TL[256] = { 0, };
			sprintf_s(TL, "%lf", TimeLength);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Time Length");
			ImGui::TableNextColumn(); ImGui::Text(TL);

			ImGui::EndTable();
		}

		if (ImGui::BeginTable("Animation3D_Info3", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH))
		{
			static int StartFrame = _TargetAni->GetMTAnimClip()->at(i).iStartFrame;
			char SF[256] = { 0, };
			sprintf_s(SF, "%d", StartFrame);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Start Frame");
			ImGui::TableNextColumn(); ImGui::Text(SF);

			static int EndFrame = _TargetAni->GetMTAnimClip()->at(i).iEndFrame;
			char EF[256] = { 0, };
			sprintf_s(EF, "%d", EndFrame);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "End Frame");
			ImGui::TableNextColumn(); ImGui::Text(EF);

			static double StartTime = _TargetAni->GetMTAnimClip()->at(i).dStartTime;
			char ST[256] = { 0, };
			sprintf_s(ST, "%lf", StartTime);
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); 	ImGui::TextColored(Vec4(1.0f, 0.5f, 0.0f, 1.0f), "Start Time");
			ImGui::TableNextColumn(); ImGui::Text(ST);

			static double EndTime = _TargetAni->GetMTAnimClip()->at(i).dEndTime;
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
	//한번에 보여줄 프레임 갯수 
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
				static int iFrameCount = GetTargetObj()->Animator3D()->GetFrameCount();
				static float Time = 0.05f;
				static int LastSelIdx = 0;
				static int RepeatOnOff = 0;
				static ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
				static bool SectionCheck = false;
				if (false == Mode)
				{
					int ChildCount = _TargetAni->GetObj()->GetChild().size();

					for (int i = 0; i < ChildCount; i++) 
					{
						if (nullptr == _TargetAni->GetObj()->GetChild()[i]->Animator3D())
							continue;

						_TargetAni->GetObj()->GetChild()[i]->Animator3D()->SetCurFrameIdx(selected);
						_TargetAni->GetObj()->GetChild()[i]->Animator3D()->SetNextFrameIdx(selected+1);
						_TargetAni->GetObj()->GetChild()[i]->Animator3D()->UpdateData();
						_TargetAni->GetObj()->GetChild()[i]->Animator3D()->CheckFinalMatUpdate();
						_TargetAni->GetObj()->GetChild()[i]->Animator3D()->ClearData();
					}

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

					ImGui::PushItemWidth(150);
					ImGui::Text("Frame Count");
					ImGui::SameLine(100);
					ImGui::InputInt("##AniFrameCount", &iFrameCount);

					GetTargetObj()->Animator3D()->SetFrameCount(iFrameCount);


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

						int ChildCount = _TargetAni->GetObj()->GetChild().size();

						for (int i = 0; i < ChildCount; i++)
						{
							if (nullptr == _TargetAni->GetObj()->GetChild()[i]->Animator3D())
								continue;

							_TargetAni->GetObj()->GetChild()[i]->Animator3D()->SetCurFrameIdx(selected);
							_TargetAni->GetObj()->GetChild()[i]->Animator3D()->SetNextFrameIdx(selected + 1);
							_TargetAni->GetObj()->GetChild()[i]->Animator3D()->UpdateData();
							_TargetAni->GetObj()->GetChild()[i]->Animator3D()->CheckFinalMatUpdate();
							_TargetAni->GetObj()->GetChild()[i]->Animator3D()->ClearData();
						}
			
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

						for (size_t i = 0; Iter != VecClip->end(); ++i)
						{
							if (i == SelectedAni)
							{
								Iter =VecClip->erase(Iter);
								_TargetAni->SetCurAnimIdx(0);
							}
							else
							{
								++Iter;
							}
						}
							

						
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
					
						_TargetAni->GetObj()->Animator3D()->SetCurAnimIdx(SelectedAni);
						
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
		double TimeLength = (*TempClip)[i].dTimeLength;

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

	GetTargetObj()->Animator3D()->GetvecClipUpdateTime().resize(TempVecClip->size());

	vector<CGameObject*> vecChild = GetTargetObj()->GetChild();

	if (vecChild.size() > 0)
	{
		for (int i = 0; i < vecChild.size(); i++)
		{
			vecChild[i]->Animator3D()->GetvecClipUpdateTime().resize(TempVecClip->size());
		}
	}

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

	GetTargetObj()->Animator3D()->ResizeClipUpdateVec(VecClip->size());

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

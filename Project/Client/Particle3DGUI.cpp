#include "pch.h"
#include "Particle3DGUI.h"
#include <Engine/CParticleSystem.h>
#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CCore.h>
#include <tchar.h>

Particle3DGUI::Particle3DGUI():
	ComponentGUI(COMPONENT_TYPE::PARTICLE)
{
}

Particle3DGUI::~Particle3DGUI()
{
}


void Particle3DGUI::update()
{
    ComponentGUI::update();
}

void Particle3DGUI::render()
{
    CTransform* pTransform = GetTargetObj()->Transform();
    CParticleSystem* pPaticle = GetTargetObj()->ParticleSystem();

    int   maxLiveCount = pPaticle->GetMaxLiveCount();
    int   maxcount = pPaticle->GetMaxCount();   
    int   AliveCount= pPaticle->GetAliveCount();
    float MinLifeTime=  pPaticle->GetMinLifeTime();
    float MaxLifeTime=  pPaticle->GetMaxLifeTime();
    float MinSpeed= pPaticle->GetMinSpeed();
    float MaxSpeed= pPaticle->GetMaxSpeed();
    float Frequency= pPaticle->GetFrequency();
    float AccTime= pPaticle->GetAccTime();                                                    
    Vec4  CreateRange= pPaticle->GetCreateRange();
    Vec4  StartColor=  pPaticle->GetStartColor();                             
    Vec4  EndColor= pPaticle->GetEndColor();
    Vec4  StartScale=  Vec4(pPaticle->GetStartScale(),1.f);
    Vec4  EndScale= Vec4(pPaticle->GetEndScale(),1.f);
    PARTICLE_TYPE Type = pPaticle->GetType();

    ImGuiColorEditFlags misc_flags = 0;

    Start();

    if (ImGui::Button("Normal"))
    {
        pPaticle->SetPOV(SHADER_POV::PARTICLE);
    }

    if (ImGui::Button("Deffered"))
    {
        pPaticle->SetPOV(SHADER_POV::DEFERRED_PARTICLE);
    }


    const char* items[] = 
    { 
        //====================
        //위로 올라가는 타입 
        // ======= 0 ==========

        "UP",
        //====================
        //땅으로 넓게 퍼지는 타입 
        // ======= 1 ==========
        "SPREAD_XZ", //색 ,텍스처 ,스피드 등 바꿔가며 재사용. y축 제외 중심으로부터 퍼짐

        //====================
        //사방으로 넓게 퍼지는 타입 
        // ======= 2 ==========
        "SPREAD_XYZ", //파이어볼 피격시 생기는 이펙트,성보스 빙글 돌며 땅 긁을때 나는 효과 

        //====================
        //가운데로 모이는 타입 
        // ======= 3 =========
        "GATHER", // 폭탄 기모으기 

        //====================
        //제자리에서 머무는 타입
        // ======= 4 ==========
        "STAY", };

   
    static int item_current_idx = 0;

    if (m_bItemChanged)
    {
        item_current_idx = (UINT)Type;
        m_bItemChanged = false;
    }

    const char* combo_label = items[item_current_idx];

    ImGui::PushItemWidth(200);

    if (ImGui::BeginCombo("Select Load Type", combo_label))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                item_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    PARTICLE_TYPE parType = (PARTICLE_TYPE)item_current_idx;

    pPaticle->SetType(parType);

    //=======================Particle On Off =============

    if(ImGui::Button("Enable"))
    {
        bool b = GetTargetObj()->ParticleSystem()->IsEnable();
        GetTargetObj()->ParticleSystem()->Activate(!b);
    }

    ImGui::SameLine();

    if (ImGui::Button("Reapet"))
    {
        bool br = GetTargetObj()->ParticleSystem()->IsRepeat();
        GetTargetObj()->ParticleSystem()->SetRepeat(!br);
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        GetTargetObj()->ParticleSystem()->Reset();
    }

    ImGui::NewLine();

    ImGui::Text("Set Texture");

    
    static char szBuffer[100] = "";
    ImGui::InputText("##Texture Name", szBuffer, 100);

    string str(szBuffer);

    wstring wstrTex = wstring(str.begin(), str.end());


    if (ImGui::Button("SetTexture"))
    {
        Ptr<CTexture> ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(wstrTex);

        if (ParticleTex == nullptr)
        {
            ParticleTex = CResMgr::GetInst()->Load<CTexture>(wstrTex, L"texture\\particle\\" + wstrTex+L".png");
        }

        pPaticle->SetTexture(ParticleTex);
    }

    ImGui::NewLine();
    
    ImGui::Text("MaxLiveCount\t");  ImGui::SameLine(); ImGui::DragInt("##MaxLiveCount", &maxLiveCount,1,0);
    ImGui::Text("MaxCount    \t");  ImGui::SameLine(); ImGui::DragInt("##MaxCount    ", &maxcount,1,0);
    ImGui::Text("AliveCount  \t");  ImGui::SameLine(); ImGui::DragInt("##AliveCount  ", &AliveCount);
    ImGui::Text("MinlifeCount\t");  ImGui::SameLine(); ImGui::DragFloat("##MinlifeCount", &MinLifeTime);
    ImGui::Text("MaxLifeTime \t");  ImGui::SameLine(); ImGui::DragFloat("##MaxLifeTime ", &MaxLifeTime);
    ImGui::Text("MinSpeed    \t");  ImGui::SameLine(); ImGui::DragFloat("##MinSpeed    ", &MinSpeed);
    ImGui::Text("MaxSpeed    \t");  ImGui::SameLine(); ImGui::DragFloat("##MaxSpeed    ", &MaxSpeed);
    ImGui::Text("Frequency   \t");  ImGui::SameLine(); ImGui::DragFloat("##Frequency   ", &Frequency);
    ImGui::Text("AccTime     \t");  ImGui::SameLine(); ImGui::DragFloat("##AccTime     ", &AccTime);
    ImGui::Text("CreateRange \t");  ImGui::SameLine(); ImGui::DragFloat4("##CreateRange ", CreateRange);
    ImGui::Text("StartColor  \t");  ImGui::SameLine(); ImGui::ColorEdit3("##vAmb", StartColor,misc_flags);
    ImGui::Text("EndColor    \t");  ImGui::SameLine(); ImGui::ColorEdit3("##vDiff", EndColor, misc_flags);
    ImGui::Text("StartScale  \t");  ImGui::SameLine(); ImGui::DragFloat3("##StartScale  ", StartScale);
    ImGui::Text("EndScale    \t");  ImGui::SameLine(); ImGui::DragFloat3("##EndScale    ", EndScale);


    End();


    if (maxcount < 0)
        maxcount = 0;

     pPaticle->SetMaxLiveCount(maxLiveCount);
     pPaticle->SetMaxParticleCount(maxcount);
     pPaticle->SetMaxCount(maxcount);
     pPaticle->SetMinLifeTime(MinLifeTime);
     pPaticle->SetMaxLifeTime(MaxLifeTime);
     pPaticle->SetMinSpeed(MinSpeed);
     pPaticle->SetMaxSpeed(MaxSpeed);
     pPaticle->SetFrequency(Frequency);
     pPaticle->SetAccTime(AccTime);
     pPaticle->SetCreateRange(CreateRange);
     pPaticle->SetStartColor(StartColor);
     pPaticle->SetEndColor(EndColor);
     pPaticle->SetStartScale(StartScale);
     pPaticle->SetEndScale(EndScale);


     if (ImGui::Button("Save Particle"))
     {
         SaveParticleFile();
     }

     ImGui::SameLine();
     if (ImGui::Button("Load Particle"))
     {
         LoadParticleFile();
     }
}


void Particle3DGUI::SaveParticleFile()
{
    std::wstring Path = CPathMgr::GetResPath();
    Path += L"ParticleData"; //

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
    ofn.lpstrDefExt = L"PTC";

    std::wstring SaveFileName = {};
    if (GetSaveFileName(&ofn))
        SaveFileName = ofn.lpstrFile;

    FILE* File = nullptr;
    _wfopen_s(&File, SaveFileName.c_str(), L"wb");

    CParticleSystem* pParticle = GetTargetObj()->ParticleSystem();

    int   maxcount = pParticle->GetMaxCount();
    float MinLifeTime = pParticle->GetMinLifeTime();
    float MaxLifeTime = pParticle->GetMaxLifeTime();
    float MinSpeed = pParticle->GetMinSpeed();
    float MaxSpeed = pParticle->GetMaxSpeed();
    float Frequency = pParticle->GetFrequency();
    Vec4  CreateRange = pParticle->GetCreateRange();
    Vec4  StartColor = pParticle->GetStartColor();
    Vec4  EndColor = pParticle->GetEndColor();
    Vec4  StartScale = Vec4(pParticle->GetStartScale(), 1.f);
    Vec4  EndScale = Vec4(pParticle->GetEndScale(), 1.f);
    SHADER_POV Pov = pParticle->GetPOV();
    PARTICLE_TYPE Type = pParticle->GetType();

    Ptr<CTexture> Tex = pParticle->GetTexture();

    fwrite(&maxcount, sizeof(int), 1, File);

    fwrite(&MinLifeTime, sizeof(float), 1, File);
    fwrite(&MaxLifeTime, sizeof(float), 1, File);
    fwrite(&MinSpeed, sizeof(float), 1, File);
    fwrite(&MaxSpeed, sizeof(float), 1, File);
    fwrite(&Frequency, sizeof(float), 1, File);
    fwrite(&CreateRange, sizeof(Vec4), 1, File);
    fwrite(&StartColor, sizeof(Vec4), 1, File);
    fwrite(&EndColor, sizeof(Vec4), 1, File);
    fwrite(&StartScale, sizeof(Vec4), 1, File);
    fwrite(&EndScale, sizeof(Vec4), 1, File);
    fwrite(&Pov, sizeof(SHADER_POV), 1, File);
    fwrite(&Type, sizeof(PARTICLE_TYPE), 1, File);

    SaveResRefInfo<CTexture>(Tex, File);
    
    fclose(File);
}

void Particle3DGUI::LoadParticleFile()
{
    std::wstring Path = CPathMgr::GetResPath();
    Path += L"ParticleData"; //

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

    FILE* _pFile = nullptr;

    _wfopen_s(&_pFile, SaveFileName.c_str(), L"rb");

    if (!_pFile)
        return;


    CParticleSystem* pParticle = GetTargetObj()->ParticleSystem();

    int   maxcount = 0;
    float MinLifeTime = 0;
    float MaxLifeTime = 0;
    float MinSpeed = 0;
    float MaxSpeed = 0;
    float Frequency = 0;
    Vec4  CreateRange = {};
    Vec4  StartColor = {};
    Vec4  EndColor = {};
    Vec4  StartScale = {};
    Vec4  EndScale = {};
    SHADER_POV Pov = (SHADER_POV)0;
    PARTICLE_TYPE Type = (PARTICLE_TYPE)0;

    Ptr<CTexture> Tex = nullptr;


    fread(&maxcount, sizeof(int), 1, _pFile); 
    fread(&MinLifeTime, sizeof(float), 1, _pFile);
    fread(&MaxLifeTime, sizeof(float), 1, _pFile);
    fread(&MinSpeed, sizeof(float), 1, _pFile);
    fread(&MaxSpeed, sizeof(float), 1, _pFile);
    fread(&Frequency, sizeof(float), 1, _pFile);
    fread(&CreateRange, sizeof(Vec4), 1, _pFile);

    fread(&StartColor, sizeof(Vec4), 1, _pFile);
    fread(&EndColor, sizeof(Vec4), 1, _pFile);
    fread(&StartScale, sizeof(Vec4), 1, _pFile);
    fread(&EndScale, sizeof(Vec4), 1, _pFile);
    fread(&Pov, sizeof(SHADER_POV), 1, _pFile);
    fread(&Type, sizeof(PARTICLE_TYPE), 1, _pFile);

    LoadResRefInfo<CTexture>(Tex, _pFile);

    pParticle->SetMaxParticleCount(maxcount);
    pParticle->SetMinLifeTime(MinLifeTime);
    pParticle->SetMaxLifeTime(MaxLifeTime);
    pParticle->SetMinSpeed(MinSpeed);
    pParticle->SetMaxSpeed(MaxSpeed);

    pParticle->SetFrequency(Frequency);
    pParticle->SetCreateRange(CreateRange);
    pParticle->SetStartColor(StartColor);         
    pParticle->SetEndColor(EndColor);
    pParticle->SetStartScale(StartScale);
    pParticle->SetEndScale(EndScale);
    
    pParticle->SetPOV(Pov);
    pParticle->SetType(Type);

    pParticle->SetTexture(Tex);

    fclose(_pFile);


}

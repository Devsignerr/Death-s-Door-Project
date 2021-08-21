#include "pch.h"
#include "Particle3DGUI.h"
#include <Engine/CParticleSystem.h>

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

     pPaticle->SetMaxParticleCount(maxcount);
     pPaticle->SetMaxCount(maxcount);
     //pPaticle->SetAliveCount(AliveCount);
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


}

#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CTitle :
    public CScript
{
private:
    CGameObject* m_TitleFont;
    CGameObject* m_Title;

   bool          m_Fade;
   int           m_FadeType;
   float         m_Time;

   
public:
    virtual void awake();
    virtual void update();
public:
    CTitle();
    ~CTitle();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CTitle);
};


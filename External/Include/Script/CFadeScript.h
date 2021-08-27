#pragma once
#include <Engine/CScript.h>

class CFadeScript :public CScript
{
    enum class FADETYPE
    {
        FADE_IN,
        FADE_OUT,
        END
    };

private:
   static FADETYPE m_FadeType;
   static bool m_FadeInOut;
   static float m_Time;
   static float m_FadeTime;
public:
    static void Fade_In();
    static void Fade_Out();
    static void SetFadeTime(float _Time) { m_FadeTime = XM_PI * _Time; }
    static bool GetIsFadeInOut() { return m_FadeInOut; }
private:
    void Fade_in(float _FadeTime);
    void Fade_out(float _FadeTime);

public:
    virtual void awake();
    virtual void update();
public:
    CFadeScript();
    ~CFadeScript();

public:
    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CFadeScript);
};


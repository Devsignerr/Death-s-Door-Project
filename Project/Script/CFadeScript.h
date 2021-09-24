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
    FADETYPE m_FadeType;
    bool m_FadeInOut;
    float m_Time;
    float m_FadeTime;
    bool  m_SceneChangeCheck;
public:
    void Fade_In();
    void Fade_Out();
    void SetFadeTime(float _Time) { m_FadeTime = XM_PI * _Time; }
    bool GetIsFadeInOut() { return m_FadeInOut; }
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

#pragma once
#include <Engine/CScript.h>

class CGameObject;
class CTitle :
    public CScript
{
private:
    CGameObject* m_TitleFont;
    Vec3            m_FontPos;
    Vec3            m_FontHalfScale;
    Vec3            m_FontMinRange;
    Vec3            m_FontMaxRange;
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


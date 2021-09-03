#pragma once
#include <Engine/CScript.h>

class CMapGimic :public CScript
{
public:
    enum class GIMICTYPE
    {
        GRATING,
        SPIN_LASER,
        ANCHOR,
        UPDOWN_SWITCH,
        ELEVATOR,
        LEVER,
        END
    };

protected:
    GIMICTYPE m_GimicType;

public:
    GIMICTYPE GetGimicType() { return m_GimicType; }

public:
    virtual void awake();
    virtual void update();
public:
    CMapGimic();
    virtual ~CMapGimic();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CMapGimic);

};


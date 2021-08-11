#pragma once
#include <Engine/CScript.h>
class CRandomMgrScript : public CScript
{

public:
    virtual void awake();
    virtual void update();

public:
    static int    GetRandomintNumber(int _Start, int _End);
    static double GetRandomDoubleNumber(double _Start, double _End);

public:
    CRandomMgrScript();
    ~CRandomMgrScript();


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CRandomMgrScript);
};


#pragma once
#include <Engine/CScript.h>

class CWeaponType : public CScript
{
private:
    enum class WEAPONTYPE
    {
        ARROW,
        FIRE,
        BOMB,
        HOOK,
        END
    };

private:
    map<WEAPONTYPE, CGameObject*>   m_WeaponIcon;
    WEAPONTYPE                      m_CurType;
    CGameObject* m_PrevIcon;
    CGameObject* m_CurIcon;
    bool                            m_IsChange;

private:
    void FucusOnOff(CGameObject* _PrevIcon, CGameObject* _CurIcon);
    void TypeChange(WEAPONTYPE _Type);

public:
    virtual void awake();
    virtual void update();
public:
    CWeaponType();
    ~CWeaponType();

    void SaveToScene(FILE* _pFile) override;
    void LoadFromScene(FILE* _pFile) override;

    CLONE(CWeaponType);
};

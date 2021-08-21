#pragma once
#include "ComponentGUI.h"

class ListGUI;

class MeshRenderGUI :
    public ComponentGUI
{
private:
    UINT      m_iMaterialIdx;  //현재 선택된 메테리얼 
   

public:    
    virtual void update();
    virtual void render();

private:
    GUI* PopupListGUI();
    void SelectListItem(ListGUI* _pListGUI, const char* _pSelectName);



public:
    MeshRenderGUI();
    ~MeshRenderGUI();
};


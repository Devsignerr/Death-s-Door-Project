#pragma once
#include "GUI.h"

class CScene;

class MenuGUI :
    public GUI
{
public:
    virtual void update();
    virtual void render();

public:
    //작성자 : 허서행 
    //이 함수는 여러개의 오브젝트로 이루어진 애니메이션을 총괄하는 부모를 생성해서 반환하는 함수임 
    //인자 : 부모 오브젝트로 지정할 이름 . 로드할 FBX 의 파일이름을 넣어주면 됨 
    CGameObject* CreateParentObj(wstring _wName,FBXLOAD_TYPE _Type,int LayerIdx);
    void ChangeSceneState(CScene* _pScene, SCENE_STATE _eState);

public:
    MenuGUI();
    ~MenuGUI();
};


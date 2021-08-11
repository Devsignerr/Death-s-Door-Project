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
    //�ۼ��� : �㼭�� 
    //�� �Լ��� �������� ������Ʈ�� �̷���� �ִϸ��̼��� �Ѱ��ϴ� �θ� �����ؼ� ��ȯ�ϴ� �Լ��� 
    //���� : �θ� ������Ʈ�� ������ �̸� . �ε��� FBX �� �����̸��� �־��ָ� �� 
    CGameObject* CreateParentObj(wstring _wName,FBXLOAD_TYPE _Type,int LayerIdx);
    void ChangeSceneState(CScene* _pScene, SCENE_STATE _eState);

public:
    MenuGUI();
    ~MenuGUI();
};


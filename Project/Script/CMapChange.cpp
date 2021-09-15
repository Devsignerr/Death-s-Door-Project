#include "pch.h"
#include "CMapChange.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CFadeScript.h"
#include "CDoorScript.h"

#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

vector<CGameObject*> CMapChange::FindCollNum = {};
bool CMapChange::IsMapChange = false;


void CMapChange::awake()
{
	wstring Name = GetObj()->GetName();

	Name = Name.substr(Name.length() - 2, Name.length());

	m_ThisCollNum = _wtoi(Name.c_str());

	// 짝수는 자신의 다음 넘버 홀수는 자신의 이전 넘버와 연결

	int CollLink = m_ThisCollNum % 2;

	if (0 == CollLink)
		m_ChangeCollNum = m_ThisCollNum + 1;
	else if (1 == CollLink)
		m_ChangeCollNum = m_ThisCollNum - 1;

	FindCollNum.push_back(GetObj());
}

void CMapChange::update()
{
	if (true == m_MapChangeCheck)
	{
		if (true == CFadeScript::GetIsFadeInOut())
		{
			IsMapChange = true;

			vector<CGameObject*>::iterator Iter = FindCollNum.begin();

			for (; Iter != FindCollNum.end(); ++Iter)
			{
				CMapChange* ChangeCol = (CMapChange*)(*Iter)->GetScript();

				if (ChangeCol->GetThisColNum() == m_ChangeCollNum)
				{
					Vec3 Pos = (*Iter)->Transform()->GetLocalPos();
					Vec3 FrontDir = CPlayerScript::GetPlayerFront();

					Pos += FrontDir * 300.0f;

					CGameObject* camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);

					((CCameraScript*)camera->GetScript())->LookAtPlayer(Pos);
										
					CPlayerScript::GetPlayer()->Transform()->SetLocalPos(Pos);	

					if (CDoorScript::m_bActive == true)
					{
						CPlayerScript* Player = CPlayerScript::GetPlayer();

						Player->GetObj()->GetChild()[0]->MeshRender()->Activate(true);
						Player->GetObj()->GetChild()[1]->MeshRender()->Activate(true);

						m_MapChangeCheck = false;
						IsMapChange = false;

						vector<CGameObject*>& Temp =(vector<CGameObject*>&)CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::MAP_GIMIC)->GetObjects();

						vector<CGameObject*>::iterator iter = Temp.begin();

						for (; iter != Temp.end(); ++iter)
						{
							if ((*iter)->GetScript())
							{
								if (((CMapGimic*)(*iter)->GetScript())->GetGimicType() == GIMICTYPE::DOOR)
								{
									((CDoorScript*)(*iter)->GetScript())->Disappear();
								}
							}
						}
					}

					break;
				}
			}
		}
	}
}

CMapChange::CMapChange()
	: CScript((UINT)SCRIPT_TYPE::MAPCHANGE)
	, m_ThisCollNum(-1)
	, m_ChangeCollNum(-1)
	, m_MapChangeCheck(false)
{
}

CMapChange::~CMapChange()
{
	FindCollNum.clear();
}

void CMapChange::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_COL== Obj->GetLayerIndex())
	{
		CFadeScript::Fade_Out();
		m_MapChangeCheck = true;
	}

	CPlayerScript* Player =CPlayerScript::GetPlayer();
	Player->GetObj()->SetAllMeshrenderActive(false);


	vector<CGameObject*>& Temp = (vector<CGameObject*>&)CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::MAP_GIMIC)->GetObjects();

	vector<CGameObject*>::iterator iter = Temp.begin();

	for (; iter != Temp.end(); ++iter)
	{
		if ((*iter)->GetScript())
		{
			if (((CMapGimic*)(*iter)->GetScript())->GetGimicType() == GIMICTYPE::DOOR)
			{
				((CDoorScript*)(*iter)->GetScript())->Spawn();
			}
		}
	}

}

void CMapChange::OnCollision(CGameObject* _pOther)
{
}

void CMapChange::OnCollisionExit(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;
	
	if ((UINT)LAYER_TYPE::PLAYER_COL == Obj->GetLayerIndex())
	{
		CFadeScript::Fade_In();
		//m_MapChangeCheck = false;
	}
}

void CMapChange::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CMapChange::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

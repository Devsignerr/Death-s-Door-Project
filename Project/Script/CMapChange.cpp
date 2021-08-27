#include "pch.h"
#include "CMapChange.h"
#include "CPlayerScript.h"
#include "CFadeScript.h"

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
			vector<CGameObject*>::iterator Iter = FindCollNum.begin();

			for (; Iter != FindCollNum.end(); ++Iter)
			{
				CMapChange* ChangeCol = (CMapChange*)(*Iter)->GetScript();

				if (ChangeCol->GetThisColNum() == m_ChangeCollNum)
				{
					Vec3 Pos = (*Iter)->Transform()->GetLocalPos();
					Vec3 FrontDir = CPlayerScript::GetPlayerFront();;

					Pos += FrontDir * 300.0f;

					CPlayerScript::GetPlayer()->Transform()->SetLocalPos(Pos);
					m_MapChangeCheck = false;
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
}

void CMapChange::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if (10 == Obj->GetLayerIndex())
	{
		CFadeScript::Fade_Out();
		m_MapChangeCheck = true;
		
	}
}

void CMapChange::OnCollision(CGameObject* _pOther)
{
}

void CMapChange::OnCollisionExit(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if (10 == Obj->GetLayerIndex())
	{
		CFadeScript::Fade_In();
		m_MapChangeCheck = false;
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

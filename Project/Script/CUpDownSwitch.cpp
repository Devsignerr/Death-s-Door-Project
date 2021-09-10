#include "pch.h"
#include "CUpDownSwitch.h"
#include "CSpinLaser.h"
#include "CFence.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>

vector<CGameObject*> CUpDownSwitch::m_UpDownSwitchObj = {};



void CUpDownSwitch::awake()
{
	if (0 == m_Test)
	{
		m_Test = 1;
		CMapGimic::awake();

		m_UpDownSwitchObj.push_back(GetGameObject());

		if (-1 == m_ThisNum)
		{
			int UpDownSwitchObjLastIdx = m_UpDownSwitchObj.size() - 1;
			CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[UpDownSwitchObjLastIdx]->GetScript();

			m_ThisNum = UpDownSwitchObjLastIdx;

			if (0 == m_ThisNum)
				Script->SetOnOffCheck(true);

			CreateCol(Vec3(120.0f, 200.0f, 120.0f), Vec3(10.0f, 100.0f, 20.0f), L"UpDownSwitchCol");
		}
	}

	m_OriginPos = Transform()->GetLocalPos();
	Vec3 Up = Transform()->GetLocalDir(DIR_TYPE::UP);
	m_UpSwitchPos = m_OriginPos + Up * 200.0f;


	Ptr<CTexture> EmissiveTex = CResMgr::GetInst()->FindRes<CTexture>(L"Wall_Ruin_Green");
	if (nullptr == EmissiveTex)
		EmissiveTex = CResMgr::GetInst()->Load<CTexture>(L"Wall_Ruin_Green", L"texture\\FBXTexture\\Wall_Ruin_Green.png");

	m_pOriginMtrl = GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0);
	m_pOriginMtrl->SetData(SHADER_PARAM::TEX_3, EmissiveTex.Get());
}

void CUpDownSwitch::update()
{
	CMapGimic::update();
	Vec3 Up = Transform()->GetLocalDir(DIR_TYPE::UP);

	if (true == m_OnOffCheck && false == m_MoveCheck)
	{
		// ºû ÄÑÁü
		Vec3 Pos = Transform()->GetLocalPos();

		if (m_UpSwitchPos.y > Pos.y)
		{
			Pos.y += fDT * 30.0f;
			Transform()->SetLocalPos(Pos);
		}
		else
		{
			m_MoveCheck = true;
			Ptr<CMaterial> Mtrl = GetObj()->GetChild()[0]->MeshRender()->GetCloneMaterial(0);
			Mtrl->SetEmissive(Vec4(0.1f, 0.8f, 0.8f, 0.5f));
		}
	}
	else if (false == m_OnOffCheck && true == m_MoveCheck)
	{
		GetObj()->GetChild()[0]->MeshRender()->SetMaterial(m_pOriginMtrl,0);
		LastSwitchOnCheck();
	}

	//if (KEY_TAP(KEY_TYPE::SPACE))
	//{
	//
	//	//if (true == m_OnOffCheck)
	//	//	m_OnOffCheck = false;
	//
	//	CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[0]->GetScript();
	//	Script->SetOnOffCheck(false);
	//
	//	Script = (CUpDownSwitch*)m_UpDownSwitchObj[1]->GetScript();
	//	Script->SetOnOffCheck(true);
	//	Script->SetMoveCheck(false);
	//}
	//
	//if (KEY_TAP(KEY_TYPE::LSHIFT))
	//{
	//	ResetSwitch();
	//}
}

void CUpDownSwitch::SpinLaserMove()
{

	if (true == CSpinLaser::GetLaserStopCheck())
		CSpinLaser::SetLaserStopCheck(false);
	//vector<CGameObject*> AllMapGimicObj = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::MAP_GIMIC)->GetObjects();
	//
	//int MapGimicSize = AllMapGimicObj.size();
	//
	//for (int i = 0; i < MapGimicSize; ++i)
	//{
	//	CMapGimic* Script = (CMapGimic*)AllMapGimicObj[i]->GetScript();
	//
	//	if (GIMICTYPE::SPIN_LASER == Script->GetGimicType())
	//	{
	//		CSpinLaser* SpinLaserScript = (CSpinLaser*)Script;
	//		if (SpinLaserScript->GetLaserStopCheck())
	//			SpinLaserScript->SetLaserStopCheck(false);
	//
	//		break;
	//	}
	//
	//}
}

void CUpDownSwitch::LastSwitchOnCheck()
{
	int LastIdx = m_UpDownSwitchObj.size() - 1;
	if (LastIdx == m_ThisNum)
	{
		CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[LastIdx]->GetScript();

		if (false == Script->m_OnOffCheck && true == Script->m_MoveCheck)
		{
			CFence::SetOpen();
			CSpinLaser::SetLaserStopCheck(true);
		}

	}

}

void CUpDownSwitch::ResetSwitch()
{
	int Size = m_UpDownSwitchObj.size();

	for (int i = 0; i < Size; ++i)
	{
		m_UpDownSwitchObj[i]->Transform()->SetLocalPos(m_OriginPos);
		m_UpDownSwitchObj[i]->GetChild()[0]->MeshRender()->SetMaterial(m_pOriginMtrl, 0);

		const vector<CGameObject*>& childvec = GetObj()->GetChild();
		int Size = childvec.size();

		for (int j = 0; j < Size; ++j)
		{
			if (childvec[j]->Collider3D())
			{
				childvec[j]->Collider3D()->Activate(true);
				break;
			}
		}
	}

	CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[0]->GetScript();
	Script->SetOnOffCheck(true);
	Script->SetMoveCheck(false);
}


void CUpDownSwitch::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		const vector<CGameObject*>& childvec = GetObj()->GetChild();
		int Size = childvec.size();

		for (int i = 0; i < Size; ++i)
		{
			if (childvec[i]->Collider3D())
			{
				childvec[i]->Collider3D()->Activate(false);
				break;
			}
		}

		if (true == m_OnOffCheck)
			m_OnOffCheck = false;

		int NextIdx = m_ThisNum + 1;

		if (m_UpDownSwitchObj.size() > NextIdx)
		{
			CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[NextIdx]->GetScript();
			Script->SetOnOffCheck(true);
			Script->SetMoveCheck(false);
		}
	
			SpinLaserMove();
	}
}

void CUpDownSwitch::OnCollision(CGameObject* _pOther)
{
}

void CUpDownSwitch::OnCollisionExit(CGameObject* _pOther)
{
}

void CUpDownSwitch::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CUpDownSwitch::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

CUpDownSwitch::CUpDownSwitch()
	: m_OnOffCheck(false)
	, m_ThisNum(-1)
	, m_MoveCheck(false)
	, m_Test(0)
{
	m_iScriptType = (int)SCRIPT_TYPE::UPDOWNSWITCH;
	m_GimicType = GIMICTYPE::UPDOWN_SWITCH;
}

CUpDownSwitch::~CUpDownSwitch()
{
	m_Test = 0;
	m_UpDownSwitchObj.clear();
}

#include "pch.h"
#include "CUpDownSwitch.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>

vector<CGameObject*> CUpDownSwitch::m_UpDownSwitchObj = {};



void CUpDownSwitch::awake()
{
	SCENE_STATE SceneState = CSceneMgr::GetInst()->GetCurScene()->GetState();

	if (SCENE_STATE::STOP == SceneState)
	{
		CMapGimic::awake();

		m_UpDownSwitchObj.push_back(GetGameObject());

		if (-1 == m_ThisNum)
		{
			size_t UpDownSwitchObjLastIdx = m_UpDownSwitchObj.size() - 1;
			CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[UpDownSwitchObjLastIdx]->GetScript();

			int LastNum = Script->GetThisNum();
			m_ThisNum = LastNum + 1;

			if (0 == m_ThisNum)
				Script->SetOnOffCheck(true);
		}

		m_UpSwitchPos = Transform()->GetLocalPos();
		Vec3 Up = Transform()->GetLocalDir(DIR_TYPE::UP);

		m_UpSwitchPos += Up * 100.0f;

		Ptr<CTexture> EmissiveTex = CResMgr::GetInst()->FindRes<CTexture>(L"Wall_Ruin_Green");
		if (nullptr == EmissiveTex)
			EmissiveTex = CResMgr::GetInst()->Load<CTexture>(L"Wall_Ruin_Green", L"texture\\FBXTexture\\Wall_Ruin_Green.png");

		m_pOriginMtrl = GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0);

		m_pOriginMtrl->SetData(SHADER_PARAM::TEX_3, EmissiveTex.Get());
	}
}

void CUpDownSwitch::update()
{
	CMapGimic::update();


	if (true == m_OnOffCheck && false == m_MoveCheck)
	{
		// ºû ÄÑÁü
		Vec3 Pos = Transform()->GetLocalPos();

		if (m_UpSwitchPos.y > Pos.y)
			Pos.y += fDT * 30.0f;
		else
		{
			m_MoveCheck = true;
			Ptr<CMaterial> Mtrl = GetObj()->GetChild()[0]->MeshRender()->GetCloneMaterial(0);
			Mtrl->SetEmissive(Vec4(0.4f, 0.1f, 0.1f, 0.f));
		}

		Transform()->SetLocalPos(Pos);
	}
	else if (false == m_OnOffCheck && true == m_MoveCheck)
	{
		GetObj()->GetChild()[0]->MeshRender()->SetMaterial(m_pOriginMtrl,0);
	}

	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		m_OnOffCheck = false;
	}
}

void CUpDownSwitch::ResetSwitch()
{
	size_t Size = m_UpDownSwitchObj.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_UpDownSwitchObj[i]->Collider3D()->Activate(true);
	}
}


void CUpDownSwitch::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		Collider3D()->Activate(false);

		if (true == m_OnOffCheck)
			m_OnOffCheck = false;

			CUpDownSwitch* Script = (CUpDownSwitch*)m_UpDownSwitchObj[(size_t)(m_ThisNum + 1)]->GetScript();
			Script->SetOnOffCheck(true);
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
{
	m_iScriptType = (int)SCRIPT_TYPE::UPDOWNSWITCH;
	m_GimicType = GIMICTYPE::UPDOWN_SWITCH;
}

CUpDownSwitch::~CUpDownSwitch()
{
	m_UpDownSwitchObj.clear();
}

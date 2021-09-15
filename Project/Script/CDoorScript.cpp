#include "pch.h"
#include "CDoorScript.h"
#include "CMapChange.h"
#include "CSceneChange.h"
#include "CFadeScript.h"

bool CDoorScript::m_bIronDead = false;
bool CDoorScript::m_bCrowDead = false;
bool CDoorScript::m_bActive = false;

CDoorScript::CDoorScript():
	m_fBurn(0.f),
	m_fWaitTime(0.f),
	m_eState(DOOR_STATE::STAY_DEACTIVE)
{
	m_GimicType = GIMICTYPE::DOOR;
	m_iScriptType = (UINT)SCRIPT_TYPE::DOORSCRIPT;
}

CDoorScript::~CDoorScript()
{
}

void CDoorScript::Spawn()
{
	m_fBurn = 4.f;
	m_eState = DOOR_STATE::SPAWNING;
}

void CDoorScript::Disappear()
{
	m_fBurn = 0.f;
	m_eState = DOOR_STATE::DISAPPEARING;
}


void CDoorScript::awake()
{
	m_eState = DOOR_STATE::STAY_DEACTIVE;
}

void CDoorScript::update()
{
	switch (m_eState)
	{
		case DOOR_STATE::SPAWNING:
		{			
				m_fBurn -= fDT * 2.f;

				Vec4 Param = {};
				Param.x = 0.4f;
				Param.y = 0.1f;
				Param.z = 0.4f;
				Param.w = m_fBurn;
				GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Param);

				if (m_fBurn < 0.f)
				{
					m_eState = DOOR_STATE::STAY_ACTIVE;
				}
			
			break;
		}
			
		case DOOR_STATE::DISAPPEARING:
		{
			
			m_fBurn += fDT * 2.f;

			Vec4 Param = {};
			Param.x = 0.4f;
			Param.y = 0.1f;
			Param.z = 0.4f;
			Param.w = m_fBurn;
			GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Param);

			if (m_fBurn > 5.f)
			{
				m_eState = DOOR_STATE::STAY_DEACTIVE;
			}
			
			break;
		}
			
		case DOOR_STATE::STAY_ACTIVE:
		{
			m_fBurn = 0.f;
			m_bActive = true;

			Vec4 Param = {};
			Param.x = 0.4f;
			Param.y = 0.1f;
			Param.z = 0.4f;
			Param.w = m_fBurn;

			GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Param);

			break;
		}
			

		case DOOR_STATE::STAY_DEACTIVE:

		{
			m_fBurn = 10.f;
			m_bActive = false;

			Vec4 Param = {};
			Param.x = 0.4f;
			Param.y = 0.1f;
			Param.z = 0.4f;
			Param.w = m_fBurn;
			GetObj()->GetChild()[0]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &Param);

			break;
		}		
			
	}
}


void CDoorScript::SaveToScene(FILE* _pFile)
{
	CMapGimic::SaveToScene(_pFile);
}

void CDoorScript::LoadFromScene(FILE* _pFile)
{
	CMapGimic::LoadFromScene(_pFile);
}

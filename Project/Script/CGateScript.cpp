#include "pch.h"
#include "CGateScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

CGateScript::CGateScript():
	m_fOpenTime(8.f),
	m_fCurTime(0.f)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::GATESCRIPT;
	m_GimicType = GIMICTYPE::GATE;
	m_eState = GATE_STATE::WAIT;
}

CGateScript::~CGateScript()
{
}

void CGateScript::awake()
{
	
}

void CGateScript::update()
{
	if (m_eState == GATE_STATE::OPEN)
	{
		m_fCurTime += fDT;

		if (m_fOpenTime > m_fCurTime)
		{
			Vec3 LocalPos = Transform()->GetLocalPos();
			LocalPos.y -= 100.f * fDT;
			Transform()->SetLocalPos(LocalPos);
		}
	}
}

void CGateScript::Open()
{
	m_eState = GATE_STATE::OPEN;
}

void CGateScript::SaveToScene(FILE* _pFile)
{
	CMapGimic::SaveToScene(_pFile);
}

void CGateScript::LoadFromScene(FILE* _pFile)
{
	CMapGimic::LoadFromScene(_pFile);
}

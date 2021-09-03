#include "pch.h"
#include "CProjectile.h"
#include "CMemoryPoolScript.h"

#include <Engine/CCollider3D.h>
#include <Engine/CEventMgr.h>


CProjectile::CProjectile():
	m_bMemoryObj(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::PROJECTILE;
}

CProjectile::~CProjectile()
{
}



void CProjectile::ReturnToMemoryPool()
{
	CMemoryPoolScript::ReturnObj(GetObj());
}

void CProjectile::update()
{
	if (m_bActive)
	{
		m_fCurTime += fDT;

		if (m_fCurTime > m_fLifeTime)
		{
			m_fCurTime = 0.f;

			if(m_bMemoryObj)
				ReturnToMemoryPool();
			else
			{
				DeleteObject(GetObj());
			}
		}

		if (m_bDestroyed)
		{
			m_fCurTime = 0.f;

			ActivateExplosionParticle();

			if(m_bMemoryObj)
				ReturnToMemoryPool();

			else 
			{
				DeleteObject(GetObj());
			}
		}
	}
}


void CProjectile::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CProjectile::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}

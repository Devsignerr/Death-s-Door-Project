#include "pch.h"
#include "TCrowDeath.h"
#include "CCrowScript.h"
#include "CMapGimic.h"
#include "CDoorScript.h"


#include <Engine/CAnimator3D.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

void TCrowDeath::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	if (540 == CurAni->GetFrameIdx())
	{
		for (int i = 0; i < childvec.size(); ++i)
		{
			if (childvec[i]->MeshRender())
			{
				UINT Count = childvec[i]->MeshRender()->GetMtrlCount();
				for (UINT j = 0; j < Count; ++j)
				{
					Ptr<CMaterial> mtrl = childvec[i]->MeshRender()->GetSharedMaterial(j);
					mtrl->SetData(SHADER_PARAM::TEX_4, m_PaperBurnTex.Get());
				}
			}
		}

		m_Script->SetbDamaged();
	}

	if (549 <= CurAni->GetFrameIdx())
	{
		m_PaperBurnTime += fDT / 2.0f;

		for (int i = 0; i < childvec.size(); ++i)
		{
			if (childvec[i]->MeshRender())
			{
				Vec4 BurnInfo = Vec4(1.0f, 0.f, 0.f, m_PaperBurnTime / 2.f);
				int BurnType = (UINT)BURN_TYPE::MONSTER_BURN;

				m_Script->EffectParamSetting(Vec4(10.f, 1.f, 1.f, 1.f), Vec4(0.01f, 0.005f, 0.005f, 1.f), m_RedTex);

				childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_1, &BurnType);
				childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::VEC4_0, &BurnInfo);
			}
		}
	}

	if (3.0f < m_PaperBurnTime)
	{
		if (!m_bDead)
		{
			CreateDoor();
			CScript::DeleteObject(GetObj());
			m_bDead = true;
		}
	}
}

void TCrowDeath::CreateDoor()
{
	vector<CGameObject*>& Temp = (vector<CGameObject*>&)CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::MAP_GIMIC)->GetObjects();

	vector<CGameObject*>::iterator iter = Temp.begin();

	int DoorCount = 0;

	for (; iter != Temp.end(); ++iter)
	{
		if ((*iter)->GetScript())
		{
			if (((CMapGimic*)(*iter)->GetScript())->GetGimicType() == GIMICTYPE::DOOR)
			{			
				((CDoorScript*)(*iter)->GetScript())->Spawn();
				((CDoorScript*)(*iter)->GetScript())->CreateCol(L"Stage_Room");
			}
		}
	}
}

void TCrowDeath::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCrowScript*)GetObj()->GetScript();

	m_Script->OnOffAttackCol(false);
	CCrowScript::SetCrowDeath();

	if (nullptr == m_RedTex)
		m_RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"RedTex");

	if (nullptr == m_PaperBurnTex)
		m_PaperBurnTex = CResMgr::GetInst()->FindRes<CTexture>(L"PaperBurnTexture");

	const vector<CGameObject*>& childvec = GetObj()->GetChild();
	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->Collider3D())
			childvec[i]->Collider3D()->Activate(false);
	}

}

void TCrowDeath::Exit()
{
}

TCrowDeath::TCrowDeath()
	: m_PaperBurnTime(0.0f)
	, m_Script(nullptr)
{
}

TCrowDeath::~TCrowDeath()
{
}

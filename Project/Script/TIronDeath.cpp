#include "pch.h"
#include "TIronDeath.h"
#include "CIronmaceScript.h"
#include "CDoorScript.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

#include <Engine/CAnimator3D.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider3D.h>


void TIronDeath::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	if (380 == CurAni->GetFrameIdx())
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

	if (382 <= CurAni->GetFrameIdx())
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

		for (int i = 0; i < childvec.size(); ++i)
		{
			if (childvec[i]->Collider3D())
				childvec[i]->Collider3D()->Activate(false);
		}

		if (2.0f < m_PaperBurnTime)
		{
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

			CScript::DeleteObject(GetObj());
		}
	}

}

void TIronDeath::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetObj()->GetScript();

	if (nullptr == m_RedTex)
		m_RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"RedTex");

	if (nullptr == m_PaperBurnTex)
		m_PaperBurnTex = CResMgr::GetInst()->FindRes<CTexture>(L"PaperBurnTexture");

	((CIronmaceScript*)GetScript())->PlaySound(L"KnightDeath", true, 0.5f);



}

void TIronDeath::Exit()
{
	
}

TIronDeath::TIronDeath()
	: m_PaperBurnTime(0.0f)
	, m_Script(nullptr)
{
}

TIronDeath::~TIronDeath()
{
}

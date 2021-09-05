#include "pch.h"
#include "TCastleDeath.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CCollider3D.h>

void TCastleDeath::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	vector<CGameObject*> childvec = GetObj()->GetChild();

	if (75 == CurAni->GetFrameIdx())
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

	if (77 <= CurAni->GetFrameIdx())
	{
		m_Death += fDT;

		if (1.0f < m_Death)
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

			if (3.0f < m_PaperBurnTime)
			{
				DeleteObject(GetObj());
			}
		}

	}
}

void TCastleDeath::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetObj()->GetScript();

	if (nullptr == m_RedTex)
		m_RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"RedTex");

	if (nullptr == m_PaperBurnTex)
		m_PaperBurnTex = CResMgr::GetInst()->FindRes<CTexture>(L"PaperBurnTexture");
}

void TCastleDeath::Exit()
{
}

TCastleDeath::TCastleDeath()
	: m_PaperBurnTime(0.0f)
	, m_Script(nullptr)
	, m_Death(0.0f)
{
}

TCastleDeath::~TCastleDeath()
{
}

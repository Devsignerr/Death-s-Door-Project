#include "pch.h"
#include "TIronDeath.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider3D.h>


void TIronDeath::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (370 <= CurAni->GetFrameIdx())
	{
		m_PaperBurnTime += fDT / 4.0f;

		vector<CGameObject*> childvec = GetObj()->GetChild();

		for (int i = 0; i < childvec.size(); ++i)
		{
			if (childvec[i]->MeshRender())
				childvec[i]->MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::FLOAT_0, &m_PaperBurnTime);

			if (childvec[i]->Collider3D())
				childvec[i]->Collider3D()->Activate(false);
		}

		if (4.0f < m_PaperBurnTime)
		{
			DeleteObject(GetObj());
		}
	}
}

void TIronDeath::Enter()
{
}

void TIronDeath::Exit()
{
}

TIronDeath::TIronDeath()
	: m_PaperBurnTime(0.0f)
{
}

TIronDeath::~TIronDeath()
{
}

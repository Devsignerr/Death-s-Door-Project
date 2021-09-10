#include "pch.h"
#include "CFogScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CRes.h>

void CFogScript::awake()
{
}

void CFogScript::update()
{
	int a = 1;

	const vector<CGameObject*>& vecChild = GetObj()->GetChild();

	UINT ChildCount = vecChild.size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		if (vecChild[i]->MeshRender())
		{
			int MtrlCount = vecChild[i]->MeshRender()->GetMtrlCount();

			for (int j = 0; j < MtrlCount; ++j)
			{
				vecChild[i]->MeshRender()->GetSharedMaterial(j)->SetData(SHADER_PARAM::INT_2, &a);
				vecChild[i]->MeshRender()->GetSharedMaterial(j)->SetData(SHADER_PARAM::FLOAT_2, &m_fBlendingRatio);
				vecChild[i]->MeshRender()->GetSharedMaterial(j)->SetData(SHADER_PARAM::FLOAT_3, &m_fMaxHeight);
			}
		}
	}

}

CFogScript::CFogScript()
	: CScript((UINT)SCRIPT_TYPE::FOGSCRIPT)
	, m_fBlendingRatio(0.0f)
	, m_fMaxHeight(0.0f)
{
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "Max_Height", &m_fMaxHeight));
	AddDesc(tDataDesc(SCRIPT_DATA_TYPE::FLOAT, "Blending_Ratio", &m_fBlendingRatio));
}

CFogScript::~CFogScript()
{
}

void CFogScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CFogScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}


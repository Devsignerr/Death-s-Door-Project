#include "pch.h"
#include "CCameraEx.h"

#include <Engine\CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>

#include "CImGUIMgr.h"
#include "InspectorGUI.h"

void CCameraEx::finalupdate()
{
	CalViewMat();
	CalProjMat();
	CalRay();

	m_frustum.finalupdate();
		
	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		ObjectSelectToRay();
	}
}

void CCameraEx::ObjectSelectToRay()
{
	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

	for (size_t i = 10; i < 20; ++i)
	{
		const std::vector<CGameObject*>& VecObj = CurScene->GetLayer(i)->GetObjects();

		if (true == VecObj.empty())
			break;

		for (size_t i = 0; i < VecObj.size(); ++i)
		{
			Vec3 TempPos = VecObj[i]->Transform()->GetWorldPos();

			Vec4 Pos = DirectX::XMVector4Transform(Vec4(TempPos.x, TempPos.y, TempPos.z, 1.0f), Camera()->GetViewMat());
			Pos = DirectX::XMVector4Transform(Pos, Camera()->GetProjMat());
			Pos.x /= Pos.w;
			Pos.y /= Pos.w;
			Pos.z /= Pos.w;
			Pos.w /= Pos.w;


			Vec4 Pos2 = DirectX::XMVector4Transform(Vec4(TempPos.x + VecObj[i]->Transform()->GetWorldScale().x / 2.f, TempPos.y, TempPos.z, 1.0f), Camera()->GetViewMat());
			Pos2 = DirectX::XMVector4Transform(Pos2, Camera()->GetProjMat());
			Pos2.x /= Pos2.w;
			Pos2.y /= Pos2.w;
			Pos2.z /= Pos2.w;
			Pos2.w /= Pos2.w;

			Vec2 TransPT = Vec2((Pos.x + 1) * 1600.f / 2.f, ((Pos.y - 1) * 900.f / -2.f));
			Vec2 TransPT2 = Vec2((Pos2.x + 1) * 1600.f / 2.f, ((Pos2.y - 1) * 900.f / -2.f));

			float fTrnaslength = TransPT.Distance(TransPT, TransPT2);

			if (fTrnaslength > Vec2::Distance(TransPT, CKeyMgr::GetInst()->GetMousePos()))
			{
				InspectorGUI* pInspector = (InspectorGUI*)CImGUIMgr::GetInst()->FindGUI(L"Inspector");
				pInspector->SetTargetObject(VecObj[i]);
				break;
			}
		}
	}
}

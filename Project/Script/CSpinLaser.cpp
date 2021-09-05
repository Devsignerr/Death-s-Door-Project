#include "pch.h"
#include "CSpinLaser.h"

#include <Engine/CCollider3D.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

#define MAX_RANGE 10000000.0f
bool CSpinLaser::m_StopCheck = false;
bool CSpinLaser::m_PrevStopState = false;

void CSpinLaser::awake()
{
	LaserCreate();
	SetCollideRectInfo();
	CreateRectToCollide();

}

void CSpinLaser::update()
{

	if (false == m_StopCheck)
	{
		Vec3 Rot = Transform()->GetLocalRot();
		Rot.y -= fDT / 3.0f;

		Transform()->SetLocalRot(Rot);
		CalLaserLength(true, m_Dist[0], m_Laser[0], m_LaserCol[0]);
		CalLaserLength(false, m_Dist[1], m_Laser[1], m_LaserCol[1]);
		m_StopTime = 0.0f;
		ReActivateLaser(m_StopCheck, 1.0f);
	}
	else
	{
		m_StopTime += fDT;

		if (m_StopTime < 2.0f)
		{
			Vec3 Rot = Transform()->GetLocalRot();
			Rot.y -= fDT / (3.0f + m_StopTime);
			Transform()->SetLocalRot(Rot);
		}


		m_Laser[0]->Transform()->SetLocalScale(Vec3(0.0f, 0.0f, 0.0f));
		m_Laser[1]->Transform()->SetLocalScale(Vec3(0.0f, 0.0f, 0.0f));

		m_LaserCol[0]->Transform()->SetLocalScale(Vec3(0.0f, 0.0f, 0.0f));
		m_LaserCol[1]->Transform()->SetLocalScale(Vec3(0.0f, 0.0f, 0.0f));
	}

	if (KEY_TAP(KEY_TYPE::KEY_E))
	{
		m_StopCheck = true;
		m_PrevStopState = true;
	}
}


void CSpinLaser::LaserCreate()
{
	if (nullptr == m_Laser[0])
	{
		for (int i = 0; i < 2; ++i)
		{
			m_Laser[i] = new CGameObject;

			wchar_t Name[256] = { 0, };
			swprintf_s(Name, L"Laser%d", i);

			m_Laser[i]->SetName(Name);

			m_Laser[i]->AddComponent(new CTransform);
			m_Laser[i]->AddComponent(new CMeshRender);

			m_Laser[i]->Transform()->SetLocalPos(Transform()->GetLocalPos());
			m_Laser[i]->Transform()->SetLocalScale(Vec3(1.0f, 40.0f, 40.0f));

			m_Laser[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Low_SphereMesh"));
			m_Laser[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_Laser[i], (UINT)LAYER_TYPE::MAP_GIMIC);
			AddChild(GetObj(), m_Laser[i]);
		}

	}

	if (nullptr == m_LaserCol[0])
	{
		for (int i = 0; i < 2; ++i)
		{
			m_LaserCol[i] = new CGameObject;

			wchar_t Name[256] = { 0, };
			swprintf_s(Name, L"LaserCol%d", i);

			m_LaserCol[i]->SetName(Name);

			m_LaserCol[i]->AddComponent(new CTransform);
			m_LaserCol[i]->AddComponent(new CMeshRender);
			m_LaserCol[i]->AddComponent(new CCollider3D);

			m_LaserCol[i]->Transform()->SetLocalPos(Transform()->GetLocalPos());
			m_LaserCol[i]->Transform()->SetLocalScale(Vec3(1.0f, 40.0f, 40.0f));


			m_LaserCol[i]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
			m_LaserCol[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(m_LaserCol[i], (UINT)LAYER_TYPE::MONSTER_ATTACK_COL);
			AddChild(GetObj(), m_LaserCol[i]);
		}

	}


}

void CSpinLaser::CalLaserLength(bool _bRight, float _Dist, CGameObject* _Laser, CGameObject* _LaserCol)
{
	_Dist = MAX_RANGE;
	float Minimum = 0.f;

	const Matrix& ObjWorld = Transform()->GetWorldMat();
	Vec3 ObjectPos = Transform()->GetLocalPos();

	Vec3 LayDir = {};

	if (_bRight)
		LayDir = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	else
		LayDir = -Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	Vec3 Point1 = {};
	Vec3 Point2 = {};
	Vec3 Point3 = {};
	Vec3 Point4 = {};

	int Size = m_AllRect.size();

	for (int i = 0; i < Size; ++i)
	{
		Matrix Mat = m_AllRect[i]->Transform()->GetWorldMat();

		Point1 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.0f), Mat);
		Point2 = DirectX::XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.0f), Mat);
		Point3 = DirectX::XMVector3TransformCoord(Vec3(0.5f, -0.5f, 0.0f), Mat);
		Point4 = DirectX::XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.0f), Mat);

		if (false == DirectX::TriangleTests::Intersects(ObjectPos, LayDir, Point1, Point2, Point3, Minimum))
			DirectX::TriangleTests::Intersects(ObjectPos, LayDir, Point1, Point3, Point4, Minimum);

		if (Minimum != 0.0f)
			_Dist = min(_Dist, Minimum);
	}

	if (_Dist != MAX_RANGE)
	{
		Vec3 Pos = Transform()->GetWorldPos();

		if (_bRight)
			Pos.x = (_Dist / 2.0f);
		else
			Pos.x = -(_Dist / 2.0f);

		Pos.y = 135.0f;
		Pos.z = 0.0f;

		_Laser->Transform()->SetLocalPos(Pos);
		_LaserCol->Transform()->SetLocalPos(Pos);

		Vec3 Scale = _Laser->Transform()->GetLocalScale();

		Scale.x = _Dist / 2.f;

		_Laser->Transform()->SetLocalScale(Scale);

		Scale.x = _Dist;
		_LaserCol->Transform()->SetLocalScale(Scale);
	}

}

void CSpinLaser::CreateRectToCollide()
{
	static int a = 0;

	if (0 == a)
	{
		++a;

		for (int i = 0; i < 19; ++i)
		{
			CGameObject* pObj = new CGameObject;
			wchar_t Name[256] = { 0, };
			swprintf_s(Name, L"Rect%02d", i);

			pObj->SetName(Name);

			pObj->AddComponent(new CTransform);
			pObj->AddComponent(new CMeshRender);

			pObj->Transform()->SetLocalPos(m_CollideRectInfo.front().Pos);
			pObj->Transform()->SetLocalScale(m_CollideRectInfo.front().Scale);
			pObj->Transform()->SetLocalRot(m_CollideRectInfo.front().Rot);

			pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);

			pObj->MeshRender()->Activate(false);
			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
			CurScene->AddObject(pObj, (UINT)LAYER_TYPE::MAP_GIMIC_COL);

			m_AllRect.push_back(pObj);
			m_CollideRectInfo.pop();
		}

	}
}

void CSpinLaser::SetCollideRectInfo()
{
	Vec3 SpinLaserPos = Transform()->GetLocalPos();

	COLLIDERECTINFO Temp = { };

	Temp.Pos = Vec3(SpinLaserPos.x - 4000.0f, SpinLaserPos.y + 327.0f, SpinLaserPos.z - 559.0f);
	Temp.Scale = Vec3(6915.0f, 2400.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, -(XM_PI / 2.0f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 4051.0f, SpinLaserPos.y + 327.0f, SpinLaserPos.z - 559.0f);
	Temp.Scale = Vec3(6915.0f, 2400.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 2.0f), 0.0f);
	m_CollideRectInfo.push(Temp);


	Temp.Pos = Vec3(SpinLaserPos.x + 400.0f, SpinLaserPos.y + 327.0f, SpinLaserPos.z + 2315.0f);
	Temp.Scale = Vec3(9000.0f, 2400.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, 0.0f, 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 400.0f, SpinLaserPos.y + 327.0f, SpinLaserPos.z - 3735.0f);
	Temp.Scale = Vec3(9000.0f, 2400.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, XM_PI, 0.0f);
	m_CollideRectInfo.push(Temp);


	Temp.Pos = Vec3(SpinLaserPos.x - 3127.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 655.0f);
	Temp.Scale = Vec3(2550.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, -(XM_PI / 2.0f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1944.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 1213);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(XM_PI, -(XM_PI / 3.6f), XM_PI);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1183.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 1986.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(XM_PI, -(XM_PI / 6.0f), XM_PI);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1939.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1168.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(XM_PI, -(XM_PI / 1.5f), XM_PI);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1202.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1932.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(XM_PI, -(XM_PI / 1.39f), XM_PI);
	m_CollideRectInfo.push(Temp);


	Temp.Pos = Vec3(SpinLaserPos.x + 1155.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1888.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 4.5f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 1954.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1118.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 4.5f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 1849.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 1189.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 1.39f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 1177.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 2023.0f);
	Temp.Scale = Vec3(94.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 1.29f), 0.0f);
	m_CollideRectInfo.push(Temp);


	Temp.Pos = Vec3(SpinLaserPos.x - 9.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 769.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, 0.0f, 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 9.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 821.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, XM_PI, 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1153.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 1139.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, -(XM_PI / 1.29f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x - 1117.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1099.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, -(XM_PI / 3.6f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 1126.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z + 1115.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 3.6f), 0.0f);
	m_CollideRectInfo.push(Temp);

	Temp.Pos = Vec3(SpinLaserPos.x + 1118.0f, SpinLaserPos.y + 233.0f, SpinLaserPos.z - 1157.0f);
	Temp.Scale = Vec3(166.0f, 546.0f, 1.0f);
	Temp.Rot = Vec3(0.0f, (XM_PI / 1.29f), 0.0f);
	m_CollideRectInfo.push(Temp);
}

void CSpinLaser::ReActivateLaser(bool _StopState, float _ReActivateTime)
{

	if (true == m_PrevStopState && false == _StopState)
	{
		for (int i = 0; i < 2; ++i)
		{
			//m_LaserCol[i];
			Vec3 OriginScale = m_Laser[i]->Transform()->GetLocalScale();
			Vec3 DestScale = Vec3(OriginScale.x, 40.0f, 40.0f);

			Vec3 TransScale = Vec3::Lerp(OriginScale, DestScale, _ReActivateTime * fDT);

			Vec3 DiffScale = DirectX::XMVectorSubtract(DestScale, TransScale);

			if (DiffScale.y <= 0.1f && DiffScale.z <= 0.1f)
			{
				m_PrevStopState = _StopState;
				break;
			}

			m_Laser[i]->Transform()->SetLocalScale(TransScale);

			TransScale.x = m_LaserCol[i]->Transform()->GetLocalScale().x;

			m_LaserCol[i]->Transform()->SetLocalScale(TransScale);
		}


	}
}



void CSpinLaser::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_COL == Obj->GetLayerIndex())
	{
		m_StopCheck = true;
		m_PrevStopState = true;

	}
}

void CSpinLaser::OnCollision(CGameObject* _pOther)
{
}

void CSpinLaser::OnCollisionExit(CGameObject* _pOther)
{
}

void CSpinLaser::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CSpinLaser::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

CSpinLaser::CSpinLaser()
	: m_StopTime(0.0f)
{
	m_iScriptType = (int)SCRIPT_TYPE::SPINLASER;
	m_GimicType = GIMICTYPE::SPIN_LASER;

	m_Laser[0] = nullptr;
	m_Laser[1] = nullptr;

	m_LaserCol[0] = nullptr;
	m_LaserCol[1] = nullptr;

	m_Dist[0] = 0.0f;
	m_Dist[1] = 0.0f;
}

CSpinLaser::~CSpinLaser()
{
}
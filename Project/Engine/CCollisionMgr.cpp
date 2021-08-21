#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider2D.h"

#include "CCollider3D.h"
#include "CTransform.h"


CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::update()
{
	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroup(iRow, iCol);				
			}
		}
	}

}

void CCollisionMgr::CollisionGroup(int _iLeft, int _iRight)
{
#pragma region Origin



	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//const vector<CGameObject*>& vecLeft = pCurScene->GetLayer(_iLeft)->GetObjects();
	//const vector<CGameObject*>& vecRight = pCurScene->GetLayer(_iRight)->GetObjects();

	//for (size_t i = 0; i < vecLeft.size(); ++i)
	//{
	//	if (nullptr == vecLeft[i]->Collider2D())
	//		continue;

	//	for (size_t j = 0; j < vecRight.size(); ++j)
	//	{
	//		if (nullptr == vecRight[j]->Collider2D() || vecLeft[i] == vecRight[j])
	//		{
	//			continue;
	//		}

	//		// �� �浹ü ���� ���̵� ����
	//		COLLIDER_PAIR pairid;
	//		pairid.iLeftID = vecLeft[i]->Collider2D()->GetID();
	//		pairid.iRightID = vecRight[j]->Collider2D()->GetID();

	//		map<LONGLONG, bool>::iterator iter = m_mapColPairInfo.find(pairid.iPairID);
	//		if (m_mapColPairInfo.end() == iter)
	//		{
	//			m_mapColPairInfo.insert(make_pair(pairid.iPairID, false));
	//			iter = m_mapColPairInfo.find(pairid.iPairID);
	//		}

	//		// ���� ����, �浹 ������ �˻�
	//		if(Collision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
	//		{
	//			// �浹 �ߴ�.
	//			if (iter->second) // �������� �浹�߾���.
	//			{					
	//				// �� ������Ʈ�� �ϳ��� ���� �������¶��
	//				if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
	//				{
	//					vecLeft[i]->Collider2D()->OnCollisionExit(vecRight[j]->Collider2D());
	//					vecRight[j]->Collider2D()->OnCollisionExit(vecLeft[i]->Collider2D());
	//				}
	//				else
	//				{
	//					vecLeft[i]->Collider2D()->OnCollision(vecRight[j]->Collider2D());
	//					vecRight[j]->Collider2D()->OnCollision(vecLeft[i]->Collider2D());
	//				}					
	//			}
	//			else // �������� �浹���� �ʾҴ�.
	//			{
	//				// �� ������Ʈ�� �ϳ��� ���� �������¶��
	//				if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())					
	//					continue;

	//				vecLeft[i]->Collider2D()->OnCollisionEnter(vecRight[j]->Collider2D());
	//				vecRight[j]->Collider2D()->OnCollisionEnter(vecLeft[i]->Collider2D());
	//				iter->second = true;
	//			}
	//		}
	//		else // �浹���� �ʴ´�.
	//		{				
	//			if (iter->second) // ������ �浹 �߾���.
	//			{
	//				vecLeft[i]->Collider2D()->OnCollisionExit(vecRight[j]->Collider2D());
	//				vecRight[j]->Collider2D()->OnCollisionExit(vecLeft[i]->Collider2D());
	//				iter->second = false;
	//			}
	//		}
	//	}
	//}
#pragma endregion

	// ���̾� �׷쳢�� �浹�� �����ϴ� �Լ�

	// ���� ���� �����´�
	CScene* CurScenc = CSceneMgr::GetInst()->GetCurScene();

	// ���̾ �޴´�
	const std::vector<CGameObject*>& VecLeft = CurScenc->GetLayer(_iLeft)->GetObjects();
	const std::vector<CGameObject*>& VecRight = CurScenc->GetLayer(_iRight)->GetObjects();

	for (size_t i = 0; i < VecLeft.size(); ++i)
	{
		if (nullptr == VecLeft[i]->Collider3D())
		{
			// ���� �ϳ��� �浹ü�� ���� ���
			continue;
		}

		for (size_t j = 0; j < VecRight.size(); ++j)
		{
			if (nullptr == VecRight[j]->Collider3D() || VecLeft[i] == VecRight[j])
			{
				// ���� �ϳ��� �浹ü�� ���� ���, �浹����� �ڱ� �ڽ��� ���
				continue;
			}

			// �� �浹ü ���� ���̵� ����
			COLLIDER_PAIR CombineID;
			CombineID.iLeftID = VecLeft[i]->Collider3D()->GetID();
			CombineID.iRightID = VecRight[j]->Collider3D()->GetID();

			std::map<LONGLONG, bool>::iterator FindIter = m_mapColPairInfo.find(CombineID.iPairID);

			if (m_mapColPairInfo.end() == FindIter)
			{
				m_mapColPairInfo.insert(std::map<LONGLONG, bool>::value_type(CombineID.iPairID, false));
				FindIter = m_mapColPairInfo.find(CombineID.iPairID);
			}

			// ���� ����, �浹 ������ �˻�
			if (/*true == Collision(VecLeft[i]->Collider2D(), VecRight[j]->Collider2D()) ||*/
				true == Collision(VecLeft[i]->Collider3D(), VecRight[j]->Collider3D()))
			{
				// ���� �����ӿ� �浹 �ߴ�.
				if (true == FindIter->second) // �������� �浹�߾���.
				{
					// �� ������Ʈ �� �ϳ��� ���� �������¶��
					if (true == VecLeft[i]->IsDead() || true == VecRight[j]->IsDead())
					{
						VecLeft[i]->Collider3D()->OnCollisionExit(VecRight[j]->Collider3D());
						VecRight[j]->Collider3D()->OnCollisionExit(VecLeft[i]->Collider3D());
					}
					else
					{
						VecLeft[i]->Collider3D()->OnCollision(VecRight[j]->Collider3D());
						VecRight[j]->Collider3D()->OnCollision(VecLeft[i]->Collider3D());
					}

				}
				else // �������� �浹���� �ʾҴ�.
				{
					// �� ������Ʈ �� �ϳ��� ���� ���� ���¶��
					if (true == VecLeft[i]->IsDead() || true == VecRight[j]->IsDead())
					{
						continue;
					}

					VecLeft[i]->Collider3D()->OnCollisionEnter(VecRight[j]->Collider3D());
					VecRight[j]->Collider3D()->OnCollisionEnter(VecLeft[i]->Collider3D());
					FindIter->second = true;
				}
			}
			else // �浹���� �ʴ´�.
			{
				if (true == FindIter->second) // ������ �浹 �߾���.
				{
					VecLeft[i]->Collider3D()->OnCollisionExit(VecRight[j]->Collider3D());
					VecRight[j]->Collider3D()->OnCollisionExit(VecLeft[i]->Collider3D());
					FindIter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::Collision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	if (COLLIDER2D_TYPE::RECT == _pLeftCol->GetCollider2DType()
		&& COLLIDER2D_TYPE::RECT == _pRightCol->GetCollider2DType())
	{
		return CollisionRvsR(_pLeftCol, _pRightCol);
	}

	else if (COLLIDER2D_TYPE::CIRCLE == _pLeftCol->GetCollider2DType()
		&& COLLIDER2D_TYPE::CIRCLE == _pRightCol->GetCollider2DType())
	{
		return CollisionCvsC(_pLeftCol, _pRightCol);
	}

	return false;
}

bool CCollisionMgr::CollisionRvsR(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	const Matrix& matColLeft = _pLeftCol->GetmatColWorld();
	const Matrix& matColRight = _pRightCol->GetmatColWorld();

	// 0 -- 1
	// |    |
	// 3 -- 2
	static const Vec3 vLocalMesh[4] = { Vec3(-0.5f, 0.5f, 0.f)
		, Vec3(0.5f, 0.5f, 0.f)
		, Vec3(0.5f, -0.5f, 0.f)
		, Vec3(-0.5f, -0.5f, 0.f) };

	// �� �浹ü�� ���� ����
	Vec2 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matColLeft) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matColRight);

	// ���ÿ� ������ų ����
	Vec2 vAxis[4] = {};
		
	vAxis[0] = XMVector3TransformCoord(vLocalMesh[1], matColLeft) - XMVector3TransformCoord(vLocalMesh[0], matColLeft);
	vAxis[1] = XMVector3TransformCoord(vLocalMesh[3], matColLeft) - XMVector3TransformCoord(vLocalMesh[0], matColLeft);
	vAxis[2] = XMVector3TransformCoord(vLocalMesh[1], matColRight) - XMVector3TransformCoord(vLocalMesh[0], matColRight);
	vAxis[3] = XMVector3TransformCoord(vLocalMesh[3], matColRight) - XMVector3TransformCoord(vLocalMesh[0], matColRight);
	
	// ���� ��
	Vec2 vProjAxis[4] = {};
	for (int i = 0; i < 4; ++i)
	{
		vProjAxis[i] = vAxis[i];
		vProjAxis[i].Normalize();
	}

	for (int i = 0; i < 4; ++i)
	{
		float fProjDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{			
			fProjDist += abs(vProjAxis[i].Dot(vAxis[j])) / 2.f;
		}

		if (fProjDist < abs(vProjAxis[i].Dot(vCenter)))
		{
			return false;
		}
	}

	return true;
}



bool CCollisionMgr::CollisionCvsC(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	_pLeftCol->GetmatColWorld();
	_pRightCol->GetmatColWorld();

	return false;
}

bool CCollisionMgr::Collision(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	if (COLLIDER3D_TYPE::CUBE == _LeftCol->GetCollider3DType() &&
		COLLIDER3D_TYPE::CUBE == _RightCol->GetCollider3DType())
	{
		return CubeToCube(_LeftCol, _RightCol);
	}
	else if (COLLIDER3D_TYPE::SHPHERE == _LeftCol->GetCollider3DType() &&
		COLLIDER3D_TYPE::SHPHERE == _RightCol->GetCollider3DType())
	{
		return SphereToSphere(_LeftCol, _RightCol);
	}
	else if (COLLIDER3D_TYPE::CUBE == _LeftCol->GetCollider3DType() &&
		COLLIDER3D_TYPE::POINT == _RightCol->GetCollider3DType())
	{
		return CubeToPoint(_LeftCol, _RightCol);
	}
	//else if (COLLIDER3D_TYPE::CUBE == _LeftCol->GetCollider3DType() &&
	//		 COLLIDER3D_TYPE::SHPHERE == _RightCol->GetCollider3DType())
	//{
	//	return CubeToSphere(_LeftCol, _RightCol);
	//}
	//else if (COLLIDER3D_TYPE::SHPHERE == _LeftCol->GetCollider3DType() &&
	//	COLLIDER3D_TYPE::CUBE == _RightCol->GetCollider3DType())
	//{
	//	return CubeToPoint(_LeftCol, _RightCol);
	//}

	return false;
}

bool CCollisionMgr::CubeToCube(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	const Matrix& MatColLeft = _LeftCol->GetMatColWorld();
	const Matrix& MatColRight = _RightCol->GetMatColWorld();

	Vec3 t = DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColLeft) - DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColRight);;
	// center ���� ���� 

	//�� �浹ü�� ���� ���͸� ���� �浹ü�� �࿡�� �翵��Ų ���� 
	float LAxisProjDistance[3];

	//�� �浹ü�� ���� ���͸� ������ �浹ü�� �࿡�� �翵��Ų ���� 
	float RAxisProjDistance[3];

	float d, e0, e1; // center �Ÿ�, projected extents �Ÿ�

	Vec3 ArrLeftAxis[3] = {};
	//x �� 
	ArrLeftAxis[0] = DirectX::XMVector3TransformNormal(Vec3(1.0f, 0.0f, 0.0f), MatColLeft);
	ArrLeftAxis[1] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 1.0f, 0.0f), MatColLeft);
	ArrLeftAxis[2] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 0.0f, 1.0f), MatColLeft);

	Vec3 ArrRightAxis[3] = {};
	//x �� 
	ArrRightAxis[0] = DirectX::XMVector3TransformNormal(Vec3(1.0f, 0.0f, 0.0f), MatColRight);
	ArrRightAxis[1] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 1.0f, 0.0f), MatColRight);
	ArrRightAxis[2] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 0.0f, 1.0f), MatColRight);

	static const Vec3 LocalMesh[5] = {
	Vec3(-0.5f,0.5f,-0.5f),Vec3(0.5f,0.5f,-0.5f) ,Vec3(0.5f,0.5f,0.5f),Vec3(-0.5f,0.5f,0.5f),Vec3(-0.5f,-0.5f,-0.5f) };

	Vec3 vCubeHalf[6] = {};
	vCubeHalf[0] = DirectX::XMVector3TransformCoord(LocalMesh[1], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);
	vCubeHalf[1] = DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[4], MatColLeft);
	vCubeHalf[2] = DirectX::XMVector3TransformCoord(LocalMesh[3], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);

	vCubeHalf[3] = DirectX::XMVector3TransformCoord(LocalMesh[1], MatColRight) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColRight);
	vCubeHalf[4] = DirectX::XMVector3TransformCoord(LocalMesh[0], MatColRight) - DirectX::XMVector3TransformCoord(LocalMesh[4], MatColRight);
	vCubeHalf[5] = DirectX::XMVector3TransformCoord(LocalMesh[3], MatColRight) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColRight);

	for (int i = 0; i < 6; ++i)
		vCubeHalf[i] /= 2.0f;

	Vec3 vLeftHalfSize = { abs(vCubeHalf[0].x),abs(vCubeHalf[1].y),abs(vCubeHalf[2].z) };
	Vec3 vRighthalfSize = { abs(vCubeHalf[3].x),abs(vCubeHalf[4].y),abs(vCubeHalf[5].z) };

	LAxisProjDistance[0] = abs(ArrLeftAxis[0].Dot(t));
	LAxisProjDistance[1] = abs(ArrLeftAxis[1].Dot(t));
	LAxisProjDistance[2] = abs(ArrLeftAxis[2].Dot(t));

	RAxisProjDistance[0] = abs(ArrRightAxis[0].Dot(t));
	RAxisProjDistance[1] = abs(ArrRightAxis[1].Dot(t));
	RAxisProjDistance[2] = abs(ArrRightAxis[2].Dot(t));

	e0 = abs(ArrLeftAxis[0].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[0].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[0];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[1].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[1].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[1];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[2].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[2].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[2];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrRightAxis[0].Dot(vCubeHalf[0])) + abs(ArrRightAxis[0].Dot(vCubeHalf[1])) + abs(ArrRightAxis[0].Dot(vCubeHalf[2]));
	e1 = abs(ArrRightAxis[0].Dot(vCubeHalf[3])) + abs(ArrRightAxis[0].Dot(vCubeHalf[4])) + abs(ArrRightAxis[0].Dot(vCubeHalf[5]));
	d = RAxisProjDistance[0];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrRightAxis[1].Dot(vCubeHalf[0])) + abs(ArrRightAxis[1].Dot(vCubeHalf[1])) + abs(ArrRightAxis[1].Dot(vCubeHalf[2]));
	e1 = abs(ArrRightAxis[1].Dot(vCubeHalf[3])) + abs(ArrRightAxis[1].Dot(vCubeHalf[4])) + abs(ArrRightAxis[1].Dot(vCubeHalf[5]));
	d = RAxisProjDistance[1];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrRightAxis[2].Dot(vCubeHalf[0])) + abs(ArrRightAxis[2].Dot(vCubeHalf[1])) + abs(ArrRightAxis[2].Dot(vCubeHalf[2]));
	e1 = abs(ArrRightAxis[2].Dot(vCubeHalf[3])) + abs(ArrRightAxis[2].Dot(vCubeHalf[4])) + abs(ArrRightAxis[2].Dot(vCubeHalf[5]));
	d = RAxisProjDistance[2];

	if (d > e0 + e1)
		return false;

	return true;
}

bool CCollisionMgr::CubeToPoint(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	const Matrix& MatColLeft = _LeftCol->GetMatColWorld();
	const Matrix& MatColRight = _RightCol->GetMatColWorld();

	Vec3 t = DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColLeft) - DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColRight);;
	// center ���� ���� 

	//�� �浹ü�� ���� ���͸� ���� �浹ü�� �࿡�� �翵��Ų ���� 
	float LAxisProjDistance[3];

	float d, e0, e1; // center �Ÿ�, projected extents �Ÿ�

	Vec3 ArrLeftAxis[3] = {};
	//x �� 
	ArrLeftAxis[0] = DirectX::XMVector3TransformNormal(Vec3(1.0f, 0.0f, 0.0f), MatColLeft);
	ArrLeftAxis[1] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 1.0f, 0.0f), MatColLeft);
	ArrLeftAxis[2] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 0.0f, 1.0f), MatColLeft);

	static const Vec3 LocalMesh[5] = {
	Vec3(-0.5f,0.5f,-0.5f),Vec3(0.5f,0.5f,-0.5f) ,Vec3(0.5f,0.5f,0.5f),Vec3(-0.5f,0.5f,0.5f),Vec3(-0.5f,-0.5f,-0.5f) };

	Vec3 vCubeHalf[6] = {};
	vCubeHalf[0] = DirectX::XMVector3TransformCoord(LocalMesh[1], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);
	vCubeHalf[1] = DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[4], MatColLeft);
	vCubeHalf[2] = DirectX::XMVector3TransformCoord(LocalMesh[3], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);

	vCubeHalf[3] = Vec3(0.0f, 0.0f, 0.0f);
	vCubeHalf[4] = Vec3(0.0f, 0.0f, 0.0f);
	vCubeHalf[5] = Vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 6; ++i)
		vCubeHalf[i] /= 2.0f;

	Vec3 vLeftHalfSize = { abs(vCubeHalf[0].x),abs(vCubeHalf[1].y),abs(vCubeHalf[2].z) };

	LAxisProjDistance[0] = abs(ArrLeftAxis[0].Dot(t));
	LAxisProjDistance[1] = abs(ArrLeftAxis[1].Dot(t));
	LAxisProjDistance[2] = abs(ArrLeftAxis[2].Dot(t));

	e0 = abs(ArrLeftAxis[0].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[0].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[0];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[1].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[1].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[1];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[2].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[2].Dot(vCubeHalf[3])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[4])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[5]));
	d = LAxisProjDistance[2];

	if (d > e0 + e1)
		return false;

	return true;
}


bool CCollisionMgr::SphereToSphere(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	const Matrix& MatColLeft = _LeftCol->GetMatColWorld();
	const Matrix& MatColRight = _RightCol->GetMatColWorld();

	float Distanse = Vec3::Distance(DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColLeft), DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColRight)) / 2.0f;
	float LSphereHalf = _LeftCol->Transform()->GetLocalScale().x / 2.0f;
	float RSphereHalf = _RightCol->Transform()->GetLocalScale().x / 2.0f;

	if (abs(Distanse) > LSphereHalf + RSphereHalf)
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::CubeToSphere(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	const Matrix& MatColLeft = _LeftCol->GetMatColWorld();
	const Matrix& MatColRight = _RightCol->GetMatColWorld();

	Vec3 t = DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColLeft) - DirectX::XMVector3TransformCoord(Vec3(0.0f, 0.0f, 0.0f), MatColRight);;
	// center ���� ���� 

	//�� �浹ü�� ���� ���͸� ���� �浹ü�� �࿡�� �翵��Ų ���� 
	float LAxisProjDistance[3];

	float d, e0, e1; // center �Ÿ�, projected extents �Ÿ�

	Vec3 ArrLeftAxis[3] = {};
	//x �� 
	ArrLeftAxis[0] = DirectX::XMVector3TransformNormal(Vec3(1.0f, 0.0f, 0.0f), MatColLeft);
	ArrLeftAxis[1] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 1.0f, 0.0f), MatColLeft);
	ArrLeftAxis[2] = DirectX::XMVector3TransformNormal(Vec3(0.0f, 0.0f, 1.0f), MatColLeft);

	static const Vec3 LocalMesh[5] = {
	Vec3(-0.5f,0.5f,-0.5f),Vec3(0.5f,0.5f,-0.5f) ,Vec3(0.5f,0.5f,0.5f),Vec3(-0.5f,0.5f,0.5f),Vec3(-0.5f,-0.5f,-0.5f) };

	Vec3 vCubeHalf[3] = {};
	vCubeHalf[0] = DirectX::XMVector3TransformCoord(LocalMesh[1], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);
	vCubeHalf[1] = DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[4], MatColLeft);
	vCubeHalf[2] = DirectX::XMVector3TransformCoord(LocalMesh[3], MatColLeft) - DirectX::XMVector3TransformCoord(LocalMesh[0], MatColLeft);

	Vec3 SphereHalf[3] = {};
	SphereHalf[0] = Vec3(_RightCol->Transform()->GetLocalScale().x, 0.0f, 0.0f);
	SphereHalf[1] = Vec3(0.0f, _RightCol->Transform()->GetLocalScale().y, 0.0f);
	SphereHalf[2] = Vec3(0.0f, 0.0f, _RightCol->Transform()->GetLocalScale().z);

	for (int i = 0; i < 3; ++i)
		vCubeHalf[i] /= 2.0f;

	Vec3 vLeftHalfSize = { abs(vCubeHalf[0].x),abs(vCubeHalf[1].y),abs(vCubeHalf[2].z) };

	LAxisProjDistance[0] = abs(ArrLeftAxis[0].Dot(t));
	LAxisProjDistance[1] = abs(ArrLeftAxis[1].Dot(t));
	LAxisProjDistance[2] = abs(ArrLeftAxis[2].Dot(t));

	e0 = abs(ArrLeftAxis[0].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[0].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[0].Dot(SphereHalf[0]));
	d = LAxisProjDistance[0];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[1].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[1].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[1].Dot(SphereHalf[1]));
	d = LAxisProjDistance[1];

	if (d > e0 + e1)
		return false;

	e0 = abs(ArrLeftAxis[2].Dot(vCubeHalf[0])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[1])) + abs(ArrLeftAxis[2].Dot(vCubeHalf[2]));
	e1 = abs(ArrLeftAxis[2].Dot(SphereHalf[2]));
	d = LAxisProjDistance[2];

	if (d > e0 + e1)
		return false;

	return true;
}

bool CCollisionMgr::SphereToCube(CCollider3D* _LeftCol, CCollider3D* _RightCol)
{
	return false;
}


void CCollisionMgr::CollisionCheckClear()
{
	memset(m_arrCheck, 0, sizeof(UINT) * MAX_LAYER);
}

void CCollisionMgr::CollisionCheck(int _iLeft, int _iRight)
{
	int iRow = -1;
	int iCol = -1;

	if ((UINT)_iLeft < (UINT)_iRight)
	{
		iRow = (UINT)_iLeft;
		iCol = (UINT)_iRight;
	}
	else
	{
		iRow = (UINT)_iRight;
		iCol = (UINT)_iLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= 1 << iCol;
	}
}

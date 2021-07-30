#pragma once

#include "CSingleton.h"

class CCollider3D;
class CCollider2D;

union COLLIDER_PAIR
{
	struct {
		UINT iLeftID;
		UINT iRightID;
	};
	LONGLONG iPairID;
};


class CCollisionMgr
	: public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT				m_arrCheck[(UINT)MAX_LAYER];
	map<LONGLONG, bool>	m_mapColPairInfo;

public:
	void update();

public:
	void CollisionCheck(int _iLeftLayer, int _iRightLayer);
	void CollisionCheckClear();

private:
	void CollisionGroup(int _iLeft, int _iRight);
	bool Collision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool CollisionRvsR(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool CollisionCvsC(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);

private:

	bool Collision(CCollider3D* _LeftCol, CCollider3D* _RightCol);
	bool CubeToCube(CCollider3D* _LeftCol, CCollider3D* _RightCol);
	bool CubeToPoint(CCollider3D* _LeftCol, CCollider3D* _RightCol);
	bool SphereToSphere(CCollider3D* _LeftCol, CCollider3D* _RightCol);
	bool CubeToSphere(CCollider3D* _LeftCol, CCollider3D* _RightCol);
	bool SphereToCube(CCollider3D* _LeftCol, CCollider3D* _RightCol);
};


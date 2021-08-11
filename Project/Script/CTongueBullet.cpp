#include "pch.h"
#include "CTongueBullet.h"
#include "CPlayerScript.h"

void CTongueBullet::update()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();
	Vec3 vRelative = PlayerPos - Pos;
	vRelative.Normalize();

	m_fTimetoCheckPlayerPos += fDT;


		//사이각 
		m_fInternalRadianWithPlayer = vFront.Dot(vRelative);

		Vec3 vCross = vFront.Cross(vRelative);

		float fDot = vCross.Dot(vUp);

		//플레이어는 내 왼쪽에 있다 
		if (fDot > 0.0f)
			m_bPlayerMyLeft = true;

		//플레이어는 내 오른쪽에 있다 
		else if (fDot < 0.0f)
			m_bPlayerMyLeft = false;




		if (m_bPlayerMyLeft)
			Rot.y += CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);
		else
			Rot.y -= CTimeMgr::GetInst()->GetfDT() * (1.5f + m_fInternalRadianWithPlayer / 2.0f);

		if (m_fTimetoCheckPlayerPos < 1.0f)
		{

			Pos.x += fDT * vRelative.x * 800.0f;
			Pos.z += fDT * vRelative.z * 800.0f;

		}
		else
		{
			Pos += (vFront / 0.95f) * fDT * 800.f;
		}


	Transform()->SetLocalPos(Pos);
	Transform()->SetLocalRot(Rot);

}

CTongueBullet::CTongueBullet()
{
	m_iScriptType = (int)SCRIPT_TYPE::TONGUEBULLET;
}

CTongueBullet::~CTongueBullet()
{
}

void CTongueBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CTongueBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}
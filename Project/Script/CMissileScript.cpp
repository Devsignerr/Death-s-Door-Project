#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: m_fSpeed(200.f)
	, CScript((int)SCRIPT_TYPE::MISSILESCRIPT)
{

}

CMissileScript::~CMissileScript()
{

}


void CMissileScript::update()
{
	Vec3 vPos = Transform()->GetLocalPos();

	vPos.y += fDT * m_fSpeed;

	Transform()->SetLocalPos(vPos);
}
#include "pch.h"
#include "CTestMonsterAttackEffect.h"
#include "CRandomMgrScript.h"

#include <Engine/CTimeMgr.h>


void CTestMonsterAttackEffect::SetVector()
{


	m_ObjVector.x = cosf(m_XRad) * m_Power.x * m_Speed;
	m_ObjVector.y = sinf(m_Rad) * m_Power.y * m_Speed;
	m_ObjVector.z = sinf(m_ZRad) * m_Power.x * m_Speed;

	m_Power.x -= m_AirResist * CTimeMgr::GetInst()->GetfDT();
	m_Power.y -= m_Gravity * CTimeMgr::GetInst()->GetfDT();

	if (10.0f > m_ObjVector.x)
	{
		m_ObjVector.x = 10.0f;
	}

	// 오브젝트를 회전시켜야하는 경우 ===========================

	//float Rad = atan2(m_ObjPower.x, m_ObjPower.y);
	//
	//Vec3 Rot = Transform()->GetLocalRot();
	//Rot.y = Rad;
	//Transform()->SetLocalRot(Rot);
	// ========================================================	
}

void CTestMonsterAttackEffect::Action()
{
	Vec3 Pos = Transform()->GetLocalPos();

	if (0.0f > Pos.y)
	{
		Pos.y = 0.0f;
		return;
	}

	
	// 여러번 튕겨야하는 경우 =========================
	//if (0.0f > Pos.y)
	//{
	//	m_ObjPower = Vec2(300.0f, 300.0f); 
	//	return;
	//}
	// ==============================================

	Pos.x += m_ObjVector.x * CTimeMgr::GetInst()->GetfDT();
	Pos.y += m_ObjVector.y * CTimeMgr::GetInst()->GetfDT();
	Pos.z += m_ObjVector.z * CTimeMgr::GetInst()->GetfDT();

	Transform()->SetLocalPos(Pos);
}

void CTestMonsterAttackEffect::awake()
{
	int a = 0;
}

void CTestMonsterAttackEffect::update()
{
	SetVector();
	Action();
}

CTestMonsterAttackEffect::CTestMonsterAttackEffect()
	: CScript((UINT)SCRIPT_TYPE::TESTMONSTERATTACKEFFECT)
	, m_Speed(4.0f)
	, m_ObjVector(0.0f,0.0f,0.0f)
	, m_Power(200.0f,300.0f)
	, m_Gravity(400.0f)
	, m_Rad(55.0f * XM_PI / 180.0f)
	, m_AirResist(10.0f)
	, m_XRad(0.0f)
	, m_ZRad(0.0f)
{
	srand(time(nullptr));
	{
		int X = CRandomMgrScript::GetRandomintNumber(60, 85);
		int Z = CRandomMgrScript::GetRandomintNumber(-15, 15);
		int CorrectionValue = CRandomMgrScript::GetRandomintNumber(0, 5);
		X += CorrectionValue;
		Z += CorrectionValue;
		m_XRad = ((float)X * XM_PI / 180.0f);
		m_ZRad = ((float)Z * XM_PI / 180.0f);
	}
}

CTestMonsterAttackEffect::~CTestMonsterAttackEffect()
{
	

}


void CTestMonsterAttackEffect::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CTestMonsterAttackEffect::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}

#include "pch.h"
#include "TCrowJump.h"
#include "CPlayerScript.h"
#include "CCrowScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowJump::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	m_Script->RotateSysTem(8.0f);

	m_Time += fDT;

	if (m_Time <= m_DestAttachTime)
	{
		float X = m_Pos.x + m_VelocityX * m_Time;
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;
		float Z = m_Pos.z + m_VelocityZ * m_Time;

		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Pos.x = X;
		Pos.y = Y;
		Pos.z = Z;

		GetObj()->Transform()->SetLocalPos(Pos);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		m_LastPos = GetObj()->Transform()->GetLocalPos();
		GetFSM()->ChangeState(L"Stomp", 0.03f, L"Stomp", false);
	}
}

void TCrowJump::Enter()
{
	m_Script = (CCrowScript*)GetScript();
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	m_Pos = GetObj()->Transform()->GetLocalPos();

	m_MaxHeight = m_Pos.y + 800.0f;

	// 최고점 도달 시간
	m_MaxHeightTime = 0.7f;
	// 도착점 높이
	float DestHeight = PlayerPos.y - m_Pos.y;

	// 최고점 - 시작점(높이)
	float Height = m_MaxHeight - m_Pos.y;
	// y축 방향의 중력가속도 
	m_Gravity = 2 * Height / (m_MaxHeightTime * m_MaxHeightTime);

	// y축 방향의 속도 
	m_VelocityY = sqrtf(2 * m_Gravity * Height);

	float a = m_Gravity;
	float b = -2 * m_VelocityY;
	float c = 2 * DestHeight;

	m_DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	m_VelocityX = -(m_Pos.x - PlayerPos.x) / m_DestAttachTime;
	m_VelocityZ = -(m_Pos.z - PlayerPos.z) / m_DestAttachTime;
}

void TCrowJump::Exit()
{
	m_Distance = 0.0f;
	m_Time = 0.0f;
	m_DestAttachTime = 0.0f;
	m_Gravity = 0.0f;
	m_VelocityX = 0.0f;
	m_VelocityY = 0.0f;
	m_VelocityZ = 0.0f;
	m_MaxHeightTime = 0.0f;
}

TCrowJump::TCrowJump()
	: m_Pos{}
	, m_Distance(0.0f)
	, m_Time(0.0f)
	, m_DestAttachTime(0.0f)
	, m_Gravity(0.0f)
	, m_VelocityX(0.0f)
	, m_VelocityY(0.0f)
	, m_VelocityZ(0.0f)
	, m_MaxHeightTime(0.0f)
	, m_MaxHeight(0.0f)
	, m_Script(nullptr)
{
}

TCrowJump::~TCrowJump()
{
}

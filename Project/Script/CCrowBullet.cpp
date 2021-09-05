#include "pch.h"
#include "CCrowBullet.h"
#include "CPlayerScript.h"

bool CCrowBullet::AwakeMove()
{
	m_Time += fDT;

	if (m_Time <= m_DestAttachTime)
	{
		Vec3 Pos = Transform()->GetLocalPos();
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 Diff = PlayerPos - Pos;
		Diff.Normalize();

		float X = m_AwakeMoveDir.x * 500.0f * fDT;
		float Y = m_Pos.y + m_VelocityY * m_Time - 0.5f * m_Gravity * m_Time * m_Time;
		float Z = m_AwakeMoveDir.z * 500.0f * fDT;

		Pos.x += X;
		Pos.y = Y;
		Pos.z += Z;

		Transform()->SetLocalPos(Pos);
		return true;
	}
	else
	{
		return false;
	}

}

void CCrowBullet::GuidedMove()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();
	Vec3 vRelative = PlayerPos - Pos;
	vRelative.Normalize();

	m_fTimetoCheckPlayerPos += fDT;


	//���̰� 
	m_fInternalRadianWithPlayer = vFront.Dot(vRelative);

	Vec3 vCross = vFront.Cross(vRelative);

	float fDot = vCross.Dot(vUp);

	//�÷��̾�� �� ���ʿ� �ִ� 
	if (fDot > 0.0f)
		m_bPlayerMyLeft = true;

	//�÷��̾�� �� �����ʿ� �ִ� 
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

void CCrowBullet::awake()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 PlayerFront = CPlayerScript::GetPlayerFront();

	m_Pos = Transform()->GetLocalPos();

	// �ְ��� ���� �ð�
	m_MaxHeightTime = 0.5f;
	// ������ ����
	float DestHeight = PlayerPos.y - m_Pos.y;

	// �ְ��� - ������(����)
	float Height = m_MaxHeight - m_Pos.y;
	// y�� ������ �߷°��ӵ� 
	m_Gravity = 2 * Height / (m_MaxHeightTime * m_MaxHeightTime);

	// y�� ������ �ӵ� 
	m_VelocityY = sqrtf(2 * m_Gravity * Height);

	float a = m_Gravity;
	float b = -2 * m_VelocityY;
	float c = 2 * DestHeight;

	m_DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	m_VelocityX = -(m_Pos.x - (PlayerPos.x)) / m_DestAttachTime;
	m_VelocityZ = -(m_Pos.z - (PlayerPos.z)) / m_DestAttachTime;
}

void CCrowBullet::update()
{
	CProjectile::update();
	if (false == AwakeMove())
		GuidedMove();
}

void CCrowBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCrowBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

CCrowBullet::CCrowBullet()
	: m_Pos{}
	, m_Distance(0.0f)
	, m_Time(0.0f)
	, m_DestAttachTime(0.0f)
	, m_Gravity(0.0f)
	, m_VelocityX(0.0f)
	, m_VelocityY(0.0f)
	, m_VelocityZ(0.0f)
	, m_MaxHeightTime(0.0f)
	, m_MaxHeight(300.0f)
	, m_fTimetoCheckPlayerPos(0.0f)
	, m_fInternalRadianWithPlayer(0.0f)
	, m_bPlayerMyLeft(false)
{
	m_iScriptType = (int)SCRIPT_TYPE::CROWBULLET;
}

CCrowBullet::~CCrowBullet()
{
}

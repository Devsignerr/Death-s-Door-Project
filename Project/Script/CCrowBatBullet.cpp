#include "pch.h"
#include "CCrowBatBullet.h"
#include "CPlayerScript.h"

bool CCrowBatBullet::AwakeMove()
{
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);

	m_AwakeMoveTime += fDT;

	if (1.0f > m_AwakeMoveTime)
	{
		Pos.x += Front.x * 1000.0f * fDT;
		Pos.z += Front.z * 1000.0f * fDT;
		GetObj()->Transform()->SetLocalPos(Pos);
	}
	else
	{
		return false;
	}

	return true;
}

bool CCrowBatBullet::GuidedMove()
{
	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vRelative = PlayerPos - Pos;
	vRelative.y = 0.f;

	float Distance = Vec3::Distance(PlayerPos, Pos);

	if (600.0f < Distance)
	{
		m_fTheta += fDT * 0.2f;
		m_fLimitTheta = 45.f;
	}
	else
	{
		m_fTheta -= fDT * 0.2f;
		m_fLimitTheta = 90.f;
	}


	Vec3 PlayerRot = CPlayerScript::GetPlayerRot();

	//���� �÷��̾ �ٶ󺸴� ���Ϳ� �� front ���͸� ���� 
	Vec3 vCross = vRelative.Cross(vFront);

	//�� ������� ���� ���Ϳ� ������ up����(������ �� front �ƴ�)�� ���������� , 
	// �� ���� ����� �÷��̾�� �� ���� , ������ �����ʿ� �ִ� �� 

	//����Ʈ�� �������� ȸ�����Ѽ� �������Ƿ� �����ͷ� ���� 

	float dot = vCross.Dot(vUp);

	float dist = (-vFront + vRelative).Length();

	vRelative.Normalize();

	float InnerRadian = vRelative.Dot(vFront);
	InnerRadian = acos(InnerRadian);


	if (InnerRadian > m_fLimitTheta * XM_PI / 180.f)
	{
		if (dot > 0.0) {
			Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_fTheta;
		}

		//�÷��̾�� �� �����ʿ� �ִ� 
		else if (dot < 0.0)
		{
			Rot.y += CTimeMgr::GetInst()->GetfDT() * m_fTheta;
		}
	}

	Vec3 vDiff = PlayerPos - Pos;
	vDiff.Normalize();
	vDiff.x = 0.0f;
	vDiff.z = 0.0f;

	Pos += (vFront * fDT * 1000.f) + (vDiff * fDT * 1000.f);

	Transform()->SetLocalPos(Pos);
	Transform()->SetLocalRot(Rot);

	return true;
}

bool CCrowBatBullet::CircleMove()
{
	float fRadius = 300.f;

	// �߽� �� -> �÷��̾��� ��ġ
	Vec3 PlayerPos = m_LastPlayerpos;

	// �׵θ� �� -> �����̵� ���� ��ġ
	Vec3 SlidingPoint = Vec3(fRadius * cosf(m_fTheta), 0.0f, fRadius * sinf(m_fTheta));

	// ������ ��ġ�� �̵�
	PlayerPos += SlidingPoint;
	GetObj()->Transform()->SetLocalPos(PlayerPos);

	m_fTheta += fDT;

	if (XM_2PI < m_fTheta)
		m_fTheta = 0.0f;

	return false;
}

void CCrowBatBullet::awake()
{
	m_fTheta = 1.5f;
}

void CCrowBatBullet::update()
{
	CProjectile::update();

	if (false == AwakeMove())
	{
		if (m_ChangeMove)
		{
			CircleMove();
		}
		else
		{
			GuidedMove();
		}
	}

}

void CCrowBatBullet::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIndex() == (UINT)LAYER_TYPE::PLAYER_ATTACK_COL)
	{
		Vec3 Pos = Transform()->GetLocalPos();
		Vec3 DiffPos = Pos-CPlayerScript::GetPlayerPos();

		ActivateImpactParticle(Vec4(0.f, 0.f, 0.f, 0.f), Pos, DiffPos, 2, 34,Vec2(300,300),Vec2(4000,8000));
		ReturnToMemoryPool();
	}
}

void CCrowBatBullet::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CCrowBatBullet::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}

CCrowBatBullet::CCrowBatBullet()
	: m_AwakeMoveTime(0.0f)
	, m_fTimetoCheckPlayerPos(0.0f)
	, m_fInternalRadianWithPlayer(0.0f)
	, m_bPlayerMyLeft(false)
	, m_ChangeMove(false)
	, m_fTheta(0.0f)
	, m_LastPlayerpos{}
{
	m_bMemoryObj = true;
	m_fLifeTime = 100.f;
	m_iScriptType = (int)SCRIPT_TYPE::CROWBATBULLET;
}

CCrowBatBullet::~CCrowBatBullet()
{
}
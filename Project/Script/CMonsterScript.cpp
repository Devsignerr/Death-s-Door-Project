#include "pch.h"
#include "CMonsterScript.h"

#include "CPlayerScript.h"

CMonsterScript::CMonsterScript()
	: m_CurState(MONSTERSTATE::IDLE)
	, m_MonsterJumpInfo{}
{
	m_iScriptType = (int)SCRIPT_TYPE::MONSTERSCRIPT;
}

CMonsterScript::~CMonsterScript()
{
}

bool CMonsterScript::RangeSearch(float _Range)
{
	Vec3 Pos = CPlayerScript::GetPlayerPos();
	Vec3 Pos2 = Transform()->GetLocalPos();
	float Distance = Vec3::Distance(CPlayerScript::GetPlayerPos(), Transform()->GetLocalPos());

	if (abs(Distance) <= _Range)
		return true;

	return false;
}

void CMonsterScript::ChangeState(MONSTERSTATE _State, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	m_CurState = _State;
	CAnimator3D* CurAnim = Animator3D();
	vector<tMTAnimClip>* vecAnimClip= CurAnim->GetMTAnimClip();
	vector<tMTAnimClip>::iterator Iter = vecAnimClip->begin();

	for (int i = 0; Iter != CurAnim->GetMTAnimClip()->end(); ++Iter, ++i)
	{
		if (Iter->strAnimName == _AniName)
		{
			CurAnim->ChangeAnimation(i, _BlendingTime, _Stay);
			break;
		}
	}
}

bool CMonsterScript::MonsterRotateSystem(float _fRotSpeed)
{
	
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = Transform()->GetLocalRot();


	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	//���� �÷��̾ �ٶ󺸴� ���Ϳ� �� front ���͸� ���� 
	Vec3 vCross = relativePos.Cross(vFront);

	//�� ������� ���� ���Ϳ� ������ up����(������ �� front �ƴ�)�� ���������� , 
	// �� ���� ����� �÷��̾�� �� ���� , ������ �����ʿ� �ִ� �� 

	//����Ʈ�� �������� ȸ�����Ѽ� �������Ƿ� �����ͷ� ���� 
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);

	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	if (dist > 1.9f)
	{
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}

	//�÷��̾�� �� ���ʿ� �ִ� 
	else if (dot > 20.0) {
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}

	//�÷��̾�� �� �����ʿ� �ִ� 
	else if (dot < -20.0)
	{
		Rot.y += CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}
	
	//�÷��̾ �ȹٷ� �ٶ󺸰� �ִ� 
	else if (dot > -20.0 && dot < 20.0 && dist < 1.f)
	{
		return true;
	}



	Transform()->SetLocalRot(Rot);


	return false;
}

void CMonsterScript::SetMonsterJumpInfo(float _JumpTime, float _JumpHeight)
{
	m_MonsterJumpInfo = {};
	Vec3 BackDir = -(Transform()->GetLocalPos() - CPlayerScript::GetPlayerPos());
	BackDir.Normalize();
	BackDir.y *= -1.0f;
	BackDir *= 2000.0f;

	Vec3 GoalPos = Transform()->GetLocalPos() + BackDir;
	m_MonsterJumpInfo.Pos = Transform()->GetLocalPos();

	// �ְ��� ���� �ð�
	m_MonsterJumpInfo.MaxHeightTime = _JumpTime;
	// ������ ����
	float DestHeight = GoalPos.y - m_MonsterJumpInfo.Pos.y;

	// �ְ��� - ������(����)
	float Height = _JumpHeight - m_MonsterJumpInfo.Pos.y;
	// y�� ������ �߷°��ӵ� 
	m_MonsterJumpInfo.Gravity = 2 * Height / (m_MonsterJumpInfo.MaxHeightTime * m_MonsterJumpInfo.MaxHeightTime);

	// y�� ������ �ӵ� 
	m_MonsterJumpInfo.VelocityY = sqrtf(2 * m_MonsterJumpInfo.Gravity * Height);

	float a = m_MonsterJumpInfo.Gravity;
	float b = -2 * m_MonsterJumpInfo.VelocityY;
	float c = 2 * DestHeight;

	m_MonsterJumpInfo.DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	//bool ������ ������ �����Ұ��� �ڷ� �����Ұ��� ����
	//m_MonsterJumpInfo.VelocityX = -(m_MonsterJumpInfo.Pos.x - m_MonsterJumpInfo.Pos.x) / m_MonsterJumpInfo.DestAttachTime;
	//m_MonsterJumpInfo.VelocityZ = -(m_MonsterJumpInfo.Pos.z - m_MonsterJumpInfo.Pos.z) / m_MonsterJumpInfo.DestAttachTime;

	m_MonsterJumpInfo.VelocityX = -(GoalPos.x - m_MonsterJumpInfo.Pos.x) / m_MonsterJumpInfo.DestAttachTime;
	m_MonsterJumpInfo.VelocityZ = -(GoalPos.z - m_MonsterJumpInfo.Pos.z) / m_MonsterJumpInfo.DestAttachTime;
}

void CMonsterScript::MonsterJumpSystem()
{
	m_MonsterJumpInfo.Time += fDT;
	Vec3 vMovePos = {};
	if (m_MonsterJumpInfo.Time <= m_MonsterJumpInfo.DestAttachTime)
	{
		float X = (m_MonsterJumpInfo.Pos.x + m_MonsterJumpInfo.VelocityX * m_MonsterJumpInfo.Time);
		float Z = (m_MonsterJumpInfo.Pos.z + m_MonsterJumpInfo.VelocityZ * m_MonsterJumpInfo.Time);

		Vec3 Pos = Transform()->GetLocalPos();

		if (false == m_MonsterJumpInfo.bJump)
		{
			vMovePos.x = (m_MonsterJumpInfo.Pos.x + m_MonsterJumpInfo.VelocityX * m_MonsterJumpInfo.Time);
			vMovePos.z = (m_MonsterJumpInfo.Pos.z + m_MonsterJumpInfo.VelocityZ * m_MonsterJumpInfo.Time);
		}
		else
		{
			vMovePos.x = Pos.x;
			vMovePos.z = Pos.z;
		}

		bool IsGround = GroundCheck(vMovePos);
		if (!IsGround)
			IsGround = ResearchNode(vMovePos);

		if (true == IsGround)
			Transform()->SetLocalPos(vMovePos);
		else
			m_MonsterJumpInfo.bJump = true;
	}
	else
	{
		m_MonsterJumpInfo.bJump = false;
	}
}

void CMonsterScript::awake()
{
}

void CMonsterScript::update()
{
	CActorScript::update();
}

//void CMonsterScript::OnCollisionEnter(CGameObject* _pOther)
//{	
//	CPlayerScript* pScript = dynamic_cast<CPlayerScript*>(_pOther->GetComponent(COMPONENT_TYPE::SCRIPT));
//	if (nullptr != pScript)
//	{
//		DeleteObject(GetObj());
//	}
//}


void CMonsterScript::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CMonsterScript::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}
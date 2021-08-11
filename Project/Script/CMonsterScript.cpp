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
	//Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	//Vec3 Pos = Transform()->GetLocalPos();
	//Vec3 relativePos = PlayerPos - Pos;
	//
	//Vec3 Rot = Transform()->GetLocalRot();
	//
	//float Rad = atan2((PlayerPos.x - Pos.x), (PlayerPos.z - Pos.z));
	//Rad = Rad - 180.0f * XM_PI / 180.0f;
	//float Degree = Rad * 180.0f / XM_PI;
	//
	//if (Degree > 180.0f) {
	//	Degree -= 360.0f;
	//}
	//else if (Degree < -180.0f) {
	//	Degree += 360.0f;
	//}
	//
	//Rad = Degree * XM_PI / 180.0f;
	//
	//if (Rad > Rot.y)
	//{
	//	if (Rad >= 0.0f && 0.0f > Rot.y)
	//	{
	//		float AngleCheck = Rad + Rot.y;
	//		if (Rad < -XM_PI || AngleCheck < XM_PI && AngleCheck < -XM_PI)
	//			Rot.y += CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//		else
	//		{
	//			Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//			if (-0.1f >= Rot.y && relativePos.z < 0.0f)
	//			{
	//				Rot.y = 0.0f;
	//			}
	//		}
	//
	//
	//	}
	//	else
	//		Rot.y += CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//}
	//else
	//{
	//
	//	if (Rad < 0.0f && 0.0f < Rot.y)
	//	{
	//		float AngleCheck = Rad + Rot.y;
	//		if (Rad < -XM_PI || AngleCheck < XM_PI && AngleCheck > -XM_PI)
	//		{
	//			Rot.y += CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//			if (XM_PI >= Rot.y && relativePos.z < 0.0f)
	//			{
	//				Rot.y = 0.0f;
	//			}
	//		}
	//		else
	//			Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//	}
	//	else
	//		Rot.y -= CTimeMgr::GetInst()->GetfDT() * m_MonsterInfo.Speed;
	//}
	//
	//if (Rot.y > XM_PI)
	//	Rot.y -= XM_2PI;
	//else if (Rot.y < -XM_PI)
	//	Rot.y += XM_2PI;
	//
	//
	//if (Rot.y < Rad + 0.04f && Rot.y > Rad - 0.04f)
	//{
	//	return true;
	//}
	//else
	//{
	//	Transform()->SetLocalRot(Rot);
	//	return false;
	//}

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	Vec3 Rot = Transform()->GetLocalRot();


	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::UP);

	//내가 플레이어를 바라보는 벡터와 내 front 벡터를 외적 
	Vec3 vCross = relativePos.Cross(vFront);

	//위 결과에서 나온 벡터와 몬스터의 up벡터(위에서 쓴 front 아님)를 내적했을때 , 
	// 위 값이 양수면 플레이어는 내 왼쪽 , 음수면 오른쪽에 있는 것 

	//프론트를 얻어왔지만 회전시켜서 세웠으므로 업벡터로 쓰임 
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	float dot = vCross.Dot(vUP);

	relativePos.Normalize();
	float dist = (-vFront + relativePos).Length();

	if (dist > 1.9f)
	{
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}

	//플레이어는 내 왼쪽에 있다 
	else if (dot > 20.0) {
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}

	//플레이어는 내 오른쪽에 있다 
	else if (dot < -20.0)
	{
		Rot.y += CTimeMgr::GetInst()->GetfDT() * _fRotSpeed;
	}
	
	//플레이어를 똑바로 바라보고 있다 
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

	// 최고점 도달 시간
	m_MonsterJumpInfo.MaxHeightTime = _JumpTime;
	// 도착점 높이
	float DestHeight = GoalPos.y - m_MonsterJumpInfo.Pos.y;

	// 최고점 - 시작점(높이)
	float Height = _JumpHeight - m_MonsterJumpInfo.Pos.y;
	// y축 방향의 중력가속도 
	m_MonsterJumpInfo.Gravity = 2 * Height / (m_MonsterJumpInfo.MaxHeightTime * m_MonsterJumpInfo.MaxHeightTime);

	// y축 방향의 속도 
	m_MonsterJumpInfo.VelocityY = sqrtf(2 * m_MonsterJumpInfo.Gravity * Height);

	float a = m_MonsterJumpInfo.Gravity;
	float b = -2 * m_MonsterJumpInfo.VelocityY;
	float c = 2 * DestHeight;

	m_MonsterJumpInfo.DestAttachTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2 * a);
	//bool 값으로 앞으로 점프할건지 뒤로 점프할건지 세팅
	//m_MonsterJumpInfo.VelocityX = -(m_MonsterJumpInfo.Pos.x - m_MonsterJumpInfo.Pos.x) / m_MonsterJumpInfo.DestAttachTime;
	//m_MonsterJumpInfo.VelocityZ = -(m_MonsterJumpInfo.Pos.z - m_MonsterJumpInfo.Pos.z) / m_MonsterJumpInfo.DestAttachTime;

	m_MonsterJumpInfo.VelocityX = -(GoalPos.x - m_MonsterJumpInfo.Pos.x) / m_MonsterJumpInfo.DestAttachTime;
	m_MonsterJumpInfo.VelocityZ = -(GoalPos.z - m_MonsterJumpInfo.Pos.z) / m_MonsterJumpInfo.DestAttachTime;
}

void CMonsterScript::MonsterJumpSystem()
{
	m_MonsterJumpInfo.Time += fDT;

	if (m_MonsterJumpInfo.Time <= m_MonsterJumpInfo.DestAttachTime)
	{
		float X = (m_MonsterJumpInfo.Pos.x + m_MonsterJumpInfo.VelocityX * m_MonsterJumpInfo.Time);
		float Y = m_MonsterJumpInfo.Pos.y + m_MonsterJumpInfo.VelocityY * m_MonsterJumpInfo.Time - 0.5f * m_MonsterJumpInfo.Gravity * m_MonsterJumpInfo.Time * m_MonsterJumpInfo.Time;
		float Z = (m_MonsterJumpInfo.Pos.z + m_MonsterJumpInfo.VelocityZ * m_MonsterJumpInfo.Time);

		Vec3 Pos = Transform()->GetLocalPos();
		Pos.x = X;
		//Pos.y = Y;
		Pos.z = Z;

		Transform()->SetLocalPos(Pos);
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
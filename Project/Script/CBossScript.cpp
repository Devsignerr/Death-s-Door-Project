#include "pch.h"
#include "CBossScript.h"
#include "CPlayerScript.h"

bool CBossScript::RangeSearch(float _Range)
{
	Vec3 Pos = CPlayerScript::GetPlayerPos();
	Vec3 Pos2 = Transform()->GetLocalPos();
	float Distance = Vec3::Distance(CPlayerScript::GetPlayerPos(), Transform()->GetLocalPos());

	if (abs(Distance) <= _Range)
		return true;

	return false;
}

bool CBossScript::RotateSysTem(float _RotSpeed)
{
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
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * (_RotSpeed * 2.0f);
	//플레이어는 내 왼쪽에 있다 
	else if (dot > 20.0) 
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _RotSpeed;

	//플레이어는 내 오른쪽에 있다 
	else if (dot < -20.0)
		Rot.y += CTimeMgr::GetInst()->GetfDT() * _RotSpeed;
	//플레이어를 똑바로 바라보고 있다 
	else if (dot > -20.0 && dot < 20.0 && dist < 1.f)
		return true;

	Transform()->SetLocalRot(Rot);

	return false;
}

void CBossScript::awake()
{
}

void CBossScript::start()
{
}

void CBossScript::update()
{
	CActorScript::update();
}

void CBossScript::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CBossScript::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}

CBossScript::CBossScript()
{
	m_iScriptType = (int)SCRIPT_TYPE::BOSSSCRIPT;
}

CBossScript::~CBossScript()
{
}

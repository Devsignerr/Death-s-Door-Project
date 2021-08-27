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
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * (_RotSpeed * 2.0f);
	//�÷��̾�� �� ���ʿ� �ִ� 
	else if (dot > 20.0) 
		Rot.y -= CTimeMgr::GetInst()->GetfDT() * _RotSpeed;

	//�÷��̾�� �� �����ʿ� �ִ� 
	else if (dot < -20.0)
		Rot.y += CTimeMgr::GetInst()->GetfDT() * _RotSpeed;
	//�÷��̾ �ȹٷ� �ٶ󺸰� �ִ� 
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
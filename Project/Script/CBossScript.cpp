#include "pch.h"
#include "CBossScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>


void CBossScript::PlaySound(wstring _wstr, bool _bOverlap, float _Volume)
{
	if (false == m_SoundCheck)
	{
		m_SoundCheck = true;
		m_iPrevSoundFrame = Animator3D()->GetFrameIdx();

		Play_Sound(_wstr, 1, _bOverlap, _Volume);
	}
}

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

void CBossScript::OnOffAttackCol(bool _OnOff, LAYER_TYPE _Type)
{
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->GetLayerIndex() == (UINT)_Type)
		{
			childvec[i]->MeshRender()->Activate(_OnOff);
			//childvec[i]->MeshRender()->Activate(false);
			childvec[i]->Collider3D()->Activate(_OnOff);
			break;
		}
	}
}

void CBossScript::CreateCol(const wstring& _Name, Vec3 _Pos, Vec3 _Scale, LAYER_TYPE _Type)
{
	Vec3 Pos = Transform()->GetLocalPos();

	CGameObject* Obj = new CGameObject;
	Obj->SetName(_Name);

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(_Pos);
	Obj->Transform()->SetLocalScale(_Scale);

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)_Type);

	GetObj()->AddChild(Obj);

	if (_Type == LAYER_TYPE::BOSS_ATTACK_COL)
	{
		Obj->Collider3D()->SetParentOffsetPos(_Pos);
		Obj->Collider3D()->Activate(false);
	}

	Obj->MeshRender()->Activate(false);
}

void CBossScript::TransColPos(Vec3 _Pos, LAYER_TYPE _Type)
{
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->GetLayerIndex() == (UINT)_Type)
		{
			childvec[i]->Transform()->SetLocalPos(_Pos);
			break;
		}
	}
}

void CBossScript::TransColScale(Vec3 _Scale, LAYER_TYPE _Type)
{
	const vector<CGameObject*>& childvec = GetObj()->GetChild();

	for (int i = 0; i < childvec.size(); ++i)
	{
		if (childvec[i]->GetLayerIndex() == (UINT)_Type)
		{
			childvec[i]->Transform()->SetLocalScale(_Scale);
			break;
		}
	}
}

void CBossScript::awake()
{
	CActorScript::awake();
}

void CBossScript::start()
{
}

void CBossScript::update()
{
	if (m_SoundCheck == true)
	{
		int FrameIdx = Animator3D()->GetFrameIdx();
		if (FrameIdx != m_iPrevSoundFrame)
		{
			m_SoundCheck = false;
		}
	}

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

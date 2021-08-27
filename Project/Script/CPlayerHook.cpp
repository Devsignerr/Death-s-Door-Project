#include "pch.h"
#include "CPlayerHook.h"
#include "CPlayerScript.h"
#include "CPlayerChain.h"
#include "CMemoryPoolScript.h"

#include <Engine/CEventMgr.h>

CPlayerHook::CPlayerHook() : 
	m_vHookDir{},
	m_fHookSpeed(4000.0f),
	m_fMaxRange(2000.f),
	m_bArrived(false),
	m_bFailed(false),
	m_bHooked(false)
{
	m_iScriptType = (UINT)SCRIPT_TYPE::PLAYERHOOK;
	m_bMemoryObj = false;
	m_fLifeTime = 0.01f;
}

CPlayerHook::~CPlayerHook()
{
}


void CPlayerHook::Destroy()
{
	returnAllChainToMemoryPool();

	tEvent even = {};

	even.eEvent = EVENT_TYPE::DELETE_OBJECT;
	even.lParam = (DWORD_PTR)GetObj();

	CEventMgr::GetInst()->AddEvent(even);

	m_vecChain.clear();
}

void CPlayerHook::update()
{
	//������ ���ư�����
	if (!m_bHooked)
		Spawnupdate();

	//������ �ɷ����� 
	else
		Hookupdate();

}

void CPlayerHook::Spawnupdate()
{
	Vec3 Pos = Transform()->GetLocalPos();
	Pos += m_vHookDir * m_fHookSpeed * fDT;

	Transform()->SetLocalPos(Pos);

	//������ �ɱ⿡ �����ߴ��� 
	CalculateDistance();	

	//ü�� ��� ���� 
	if(!m_bFailed)
		SpawnChain();
}

void CPlayerHook::Hookupdate()
{
	AdjustChainEmissive();
	returnChainToMemoryPool();
	CheckArrived();
}


void CPlayerHook::SpawnChain()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetWorldPos();

	float Distance = (PlayerPos - Pos).Length();

	float Scale = CPlayerChain::GetScale();
	float Step = CPlayerChain::GetStep();

	int ChainCount = m_vecChain.size();
	CGameObject* Chain = nullptr;

	//���� �÷��̾� �� ���̿� ü�� �ϳ� �� ���� ������ �ִٸ� 
	if (Distance > (ChainCount + 1) * Step)
	{
		Chain = CMemoryPoolScript::GetChain();

		if (nullptr == Chain)
			return;

		m_vecChain.push_back(Chain);


		Vec3 LocalRot = Chain->Transform()->GetLocalRot();
		LocalRot.y += 1.57f;
		if (ChainCount % 2 == 0)
			LocalRot.z += 1.57f;

	
		GetObj()->AddChild(Chain);

		Chain->Transform()->SetLocalRot(LocalRot);

		//����� ���ݰ� ü�� ������ ���ؼ� ��ġ�� ���Ѵ� 
		Vec3 ChainPos = Vec3(0.f,0.f, Step * ChainCount);
		Chain->Transform()->SetLocalPos(ChainPos);
	}

}

void CPlayerHook::returnChainToMemoryPool()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	float Distance = (PlayerPos - m_vHookedPos).Length();

	float Scale = CPlayerChain::GetScale();
	float Step = CPlayerChain::GetStep();

	int ChainCount = m_vecChain.size();
	CGameObject* Chain = nullptr;

	//���� �÷��̾� �� ���̰� ü�� ��ü�� ���̺��� ª�����ٸ� 
	//���� ������ �ִ� ü�κ��� ������� ��ȯ 
	if (Distance < ChainCount * Step)
	{
		if (!m_vecChain.empty())
		{
			//���� ������ �ִ� ü�� 
			CGameObject* pObj = m_vecChain.back();
			pObj->SetAllMeshrenderActive(false);
					
			//�޸�Ǯ�� �ݳ� 
			((CPlayerChain*)pObj->GetScript())->ReturnToMemoryPool();

			//�ڽ� ���Ϳ��� ������ 
			pObj->DisconnectWithParent();
			pObj->RegisterAsParentObj();

			m_vecChain.pop_back();
		}
	}
}

void CPlayerHook::returnAllChainToMemoryPool()
{	
	int ChainCount = m_vecChain.size();

	for(int i=0; i< ChainCount;i++)
	{
		CGameObject* pObj = m_vecChain[i];
				
		pObj->SetAllMeshrenderActive(false);
		((CPlayerChain*)pObj->GetScript())->ReturnToMemoryPool();

		//�ڽ� ���Ϳ��� ������ 
		pObj->DisconnectWithParent();
		pObj->RegisterAsParentObj();
		
	}
}

void CPlayerHook::CalculateDistance()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetWorldPos();

	float Distance = (PlayerPos - Pos).Length();

	if (Distance > m_fMaxRange)
			m_bFailed = true;
}

void CPlayerHook::AdjustChainEmissive()
{


}

void CPlayerHook::CheckArrived()
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	float Distance = (PlayerPos - m_vHookedPos).Length();

	//���������� 50��ŭ�������ִٸ� 
	if (Distance < 70.f)
	{
		m_bArrived = true;
	}
}


void CPlayerHook::OnCollisionEnter(CGameObject* _pOther)
{
	m_bHooked = true;
	m_vHookedPos = _pOther->Transform()->GetLocalPos();
}

void CPlayerHook::OnCollision(CGameObject* _pOther)
{
}

void CPlayerHook::OnCollisionExit(CGameObject* _pOther)
{
}

void CPlayerHook::SaveToScene(FILE* _pFile)
{
	CProjectile::SaveToScene(_pFile);
}

void CPlayerHook::LoadFromScene(FILE* _pFile)
{
	CProjectile::LoadFromScene(_pFile);
}


#include "pch.h"
#include "CExplosionParticle.h"
#include "CMemoryPoolScript.h"
#include "CCameraScript.h"
#include <Engine/CParticleSystem.h>

CExplosionParticle::CExplosionParticle()
{
	m_iScriptType = (UINT)SCRIPT_TYPE::EXPLOSIONPARTICLE;
}

CExplosionParticle::~CExplosionParticle()
{
}



void CExplosionParticle::awake()
{
	m_iMaxCount = GetObj()->ParticleSystem()->GetMaxCount();
	GetObj()->ParticleSystem()->SetRepeat(false);
}

void CExplosionParticle::update()
{
	if (m_bActive)
	{
		CParticleSystem* pParticle = GetObj()->ParticleSystem();

		int AccLiveCount = pParticle->GetAccLiveCount();
		int MaxLiveCount = pParticle->GetMaxLiveCount();

		if (AccLiveCount >= MaxLiveCount)
		{
			m_fCurTime += fDT;
			float MaxLifeTime = pParticle->GetMaxLifeTime();

			//�������� ����ٸ� .. ���� �ð��� �ش� ���� �ʱ�ȭ �����ִ� �Լ��� ������..
			if (m_eType == EXPLOSION_PTC_TYPE::PLAYER_BOMB)
			{
				int Off = 0;
				CResMgr::GetInst()->FindRes<CMaterial>(L"PostEffectMtrl")->SetData(SHADER_PARAM::INT_1, &Off);
			}

			if (m_fCurTime > MaxLifeTime)
			{
				SetActive(false);
				m_fCurTime = 0.f;

				if(m_bMemoryObj)
					CMemoryPoolScript::ReturnObj(GetObj());
				else
				{
					GetObj()->ParticleSystem()->Activate(false);
					GetObj()->SetAllColliderActive(false);
					Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
					GetObj()->ParticleSystem()->Destroy();
				}
			}	
		}
	}
}

void CExplosionParticle::SetActive(bool _b)
{
	if (_b == false && m_bActive==true)
	{
		GetObj()->ParticleSystem()->SetMaxCount(0);
		GetObj()->ParticleSystem()->Activate(false);
		GetObj()->ParticleSystem()->SetAccLiveCount(0);
		GetObj()->ParticleSystem()->SetAccTime(0.f);
	}

	if (_b == true && m_bActive == false)
	{
		GetObj()->ParticleSystem()->Activate(true);
		GetObj()->ParticleSystem()->SetMaxCount(m_iMaxCount);		
		GetObj()->ParticleSystem()->Reset();

		//�������� ����ٸ� .. ���� �ð��� �ش� ���� �ʱ�ȭ �����ִ� �Լ��� ������..
		if (m_eType == EXPLOSION_PTC_TYPE::PLAYER_BOMB)
		{
			int On = 1;
			CResMgr::GetInst()->FindRes<CMaterial>(L"PostEffectMtrl")->SetData(SHADER_PARAM::INT_1, &On);
		}

		CCameraScript::SetCameraShake(0.3f, 100.f, 5.f);
	}

	m_bActive = _b;

}

void CExplosionParticle::Reset()
{
	GetObj()->ParticleSystem()->Reset();
}


void CExplosionParticle::SaveToScene(FILE* _pFile)
{
	CEffectScript::SaveToScene(_pFile);
}

void CExplosionParticle::LoadFromScene(FILE* _pFile)
{
	CEffectScript::LoadFromScene(_pFile);
}

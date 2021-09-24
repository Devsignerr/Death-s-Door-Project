#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CMeshRender.h"
#include "CParticleSystem.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CFrustumSphere.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CScript.h"
#include "CUI.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_iLayerIdx(-1)
	, m_pParentObj(nullptr)
	, m_bFrustum(true)
	, m_bDynamicShadow(true)
	, m_bDead(false)
{
	
}

CGameObject::CGameObject(CGameObject& _origin)
	: m_arrCom{}
	, m_vecChild()
	, m_pParentObj(nullptr)
	, m_bDynamicShadow(true)
	, m_iLayerIdx(-1)
	, m_bFrustum(_origin.m_bFrustum)
	, m_bDead(false)
{
	// ������Ʈ ����
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());			
		}		
	}

	// Script ����
	for (size_t i = 0; i < _origin.m_vecScript.size(); ++i)
	{
		AddComponent(_origin.m_vecScript[i]->Clone());
	}

	// �ڽ� ������Ʈ ����
	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
	wstring Name = _origin.GetName();
	SetName(Name);
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrCom);
	Safe_Delete_Vector(m_vecScript);
	Safe_Delete_Vector(m_vecChild);
}

void CGameObject::awake()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->awake();
	}	

	
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->awake();
	}
	

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->awake();
	}
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}
	

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::update()
{
	
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{

		if (nullptr != m_arrCom[i])
			m_arrCom[i]->update();

	}


	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->update();
	}
	
	

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->update();
	}
}

void CGameObject::lateupdate()
{
	
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->lateupdate();
	}

	
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->lateupdate();
	}
	

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}
	
		// Register
	CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObject(this);
	
}


void CGameObject::finalupdate_ex()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}
}


void CGameObject::AddComponent(CComponent* _pComponent)
{
	UINT iType = (UINT)_pComponent->GetType();
		
	if (iType != (UINT)COMPONENT_TYPE::SCRIPT)
	{
		assert(!m_arrCom[iType]); // �̹� ������ ������Ʈ�� �����ϰ� �ִ� ���
		assert(!_pComponent->GetGameObject()); // ������Ʈ�� �̹� �ٸ� ������Ʈ�� ���� ���
		m_arrCom[iType] = _pComponent;
	}
	else
	{
		m_vecScript.push_back((CScript*)_pComponent);
	}
		
	_pComponent->SetGameObject(this);
}

void CGameObject::SetDynamicShadow(bool _bTrue)
{
	m_bDynamicShadow = _bTrue;

	UINT ChildCount = GetChild().size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		GetChild()[i]->SetDynamicShadow(_bTrue);
	}
}

void CGameObject::SetFrustumCheck(bool _bTrue)
{
	m_bFrustum = _bTrue;

	UINT ChildCount = GetChild().size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		GetChild()[i]->SetFrustumCheck(_bTrue);
	}
}

void CGameObject::SetAllColliderActive(bool _b)
{
	const vector<CGameObject*>& vecChild = GetChild();

	UINT ChildCount = vecChild.size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		if (vecChild[i]->Collider3D())
			vecChild[i]->Collider3D()->Activate(_b);
	}

	if(Collider3D())
		Collider3D()->Activate(_b);
}

void CGameObject::SetAllMeshrenderActive(bool _b)
{
	const vector<CGameObject*>& vecChild = GetChild();

	UINT ChildCount = vecChild.size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		if (vecChild[i]->MeshRender()&& nullptr==vecChild[i]->Collider3D())
			vecChild[i]->MeshRender()->Activate(_b);
	}

	if (MeshRender())
		MeshRender()->Activate(_b);
}

void CGameObject::SetAlldynamicShadow(bool _b)
{
	const vector<CGameObject*>& vecChild = GetChild();

	UINT ChildCount = vecChild.size();

	for (UINT i = 0; i < ChildCount; ++i)
	{
		vecChild[i]->SetDynamicShadow(false);
		vecChild[i]->SetFrustumCheck(false);
	}

	SetDynamicShadow(false);
	SetFrustumCheck(false);
}

void CGameObject::ChangeLayerIdx(UINT _Idx)
{
	CGameObject* Parent = m_pParentObj;

	if (Parent)
	{
		DisconnectWithParent();
		SetLayerIndex(_Idx);
		RegisterAsParentObj();
		Parent->AddChild(this);
	}
		
	else 
	{
		ReleaseAsParentObj();
		SetLayerIndex(_Idx);
		RegisterAsParentObj();
	}

}

void CGameObject::DisconnectWithParent()
{
	// �θ� ���� ������Ʈ�� �θ����� �Լ� ȣ���� ���
	assert(m_pParentObj);

	vector<CGameObject*>::iterator iter = m_pParentObj->m_vecChild.begin();
	for (; iter != m_pParentObj->m_vecChild.end(); ++iter)
	{
		if (this == (*iter))
		{
			m_pParentObj->m_vecChild.erase(iter);
			m_pParentObj = nullptr;
			return;
		}
	}
	
	// �ڽ��� �θ� �ƴµ�, �θ�� �ڽ��� �𸣴°��
	assert(nullptr);
}

void CGameObject::RegisterAsParentObj()
{
	// �Ҽ� Layer �� ���� ���
	if (-1 == m_iLayerIdx)
	{
		return;
	}

	// �Ҽ� Layer �� �θ������Ʈ�� ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	int iLayerIdx = m_iLayerIdx;
	m_iLayerIdx = -1;
	pCurScene->AddObject(this, iLayerIdx);
}

void CGameObject::ReleaseAsParentObj()
{
	// �Ҽӷ��̾ ���� ���(Scene �� ���Ե����� ������)
	if (-1 == m_iLayerIdx)
		return;
	
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(m_iLayerIdx);	

	vector<CGameObject*>::iterator iter = pCurLayer->m_vecParentObj.begin();
	for (; iter != pCurLayer->m_vecParentObj.end(); ++iter)
	{
		if (this == (*iter))
		{
			pCurLayer->m_vecParentObj.erase(iter);			
			return;
		}
	}

	// �θ������Ʈ�� �Ҽ� ���̾ ����� �ȵǾ� �־��� --> ����
	assert(nullptr);
}

void CGameObject::AddChild(CGameObject* _pChildObj)
{
	// 1. �ֻ��� �θ� ������Ʈ�� �ڽ����� ������ ���
	if (nullptr == _pChildObj->GetParent())
	{
		_pChildObj->ReleaseAsParentObj();
	}

	// 2. �ٸ� �θ� �ִ� ������Ʈ�� �ڽ����� ���� ���
	else
	{
		// �̹� �ش� ������Ʈ�� �ڽ��� ���
		if (this == _pChildObj->m_pParentObj)
		{
			return;
		}

		// ���� ����
		_pChildObj->DisconnectWithParent();
	}
	
	m_vecChild.push_back(_pChildObj);
	_pChildObj->m_pParentObj = this;

	// �ڽĿ�����Ʈ�� ���̾� �Ҽ��� ������ �θ� ���󰣴�.
	if (-1 == _pChildObj->m_iLayerIdx)
	{
		_pChildObj->m_iLayerIdx = m_iLayerIdx;
	}
}

void CGameObject::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);

	fwrite(&m_iLayerIdx, sizeof(int), 1, _pFile);

	UINT i = 0;
	wstring Name = {};
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCom[i] != nullptr)
		{
			Name = m_arrCom[i]->GetComponentName();
			SaveWString(Name, _pFile);
			//fwrite(&i, sizeof(UINT), 1, _pFile);
			m_arrCom[i]->SaveToScene(_pFile);
		}
	}

	//���̶�°� �˸��� ���ؼ� 
	//fwrite(&i, sizeof(UINT), 1, _pFile);
	Name = L"END";
	SaveWString(Name, _pFile);

	UINT iScriptCount = (UINT)m_vecScript.size();
	fwrite(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		g_pScriptSave(m_vecScript[i], _pFile);
	}

	UINT iChildCount = (UINT)m_vecChild.size();
	vector<int> saveIdx = {};

	for (int k = 0; k < m_vecChild.size(); ++k)
	{
		if (DONTSAVEGOBJ)
		{
			--iChildCount;
		}
		else
		{
			saveIdx.push_back(k);
		}
	}

	fwrite(&iChildCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		int idx = saveIdx[i];
		m_vecChild[idx]->SaveToScene(_pFile);
	}
}

void CGameObject::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);

	fread(&m_iLayerIdx, sizeof(int), 1, _pFile);

	UINT i = 0;	
	while (true)
	{
		//fread(&i, sizeof(UINT), 1, _pFile);
		wstring wstrComponentName = {};
		LoadWString(wstrComponentName, _pFile);

		CComponent* pComponent = nullptr;
		//switch ((COMPONENT_TYPE)i)
		//{
		//case COMPONENT_TYPE::TRANSFORM:
		//	pComponent = new CTransform;			
		//	break;
		//case COMPONENT_TYPE::MESHRENDER:
		//	pComponent = new CMeshRender;			
		//	break;
		//case COMPONENT_TYPE::CAMERA:
		//	pComponent = new CCamera;
		//	break;
		//case COMPONENT_TYPE::COLLIDER2D:
		//	pComponent = new CCollider2D;
		//	break;
		//case COMPONENT_TYPE::COLLIDER3D:
		//	pComponent = new CCollider3D;
		//	break;
		//case COMPONENT_TYPE::ANIMATOR2D:
		//	pComponent = new CAnimator2D;
		//	break;
		//case COMPONENT_TYPE::ANIMATOR3D:
		//	pComponent = new CAnimator3D;
		//	break;
		//case COMPONENT_TYPE::LIGHT2D:
		//	pComponent = new CLight2D;
		//	break;
		//case COMPONENT_TYPE::LIGHT3D:
		//	pComponent = new CLight3D;
		//	break;
		//case COMPONENT_TYPE::PARTICLE:
		//	pComponent = new CParticleSystem;
		//	break;			
		//}

		if (wstrComponentName == L"TRANSFORM")
		{
			pComponent = new CTransform;
		}

		else if (wstrComponentName == L"MESHRENDER")
		{
			pComponent = new CMeshRender;
		}

		else if (wstrComponentName == L"CAMERA")
		{
			pComponent = new CCamera;
		}

		else if (wstrComponentName == L"COLLIDER2D")
		{
			pComponent = new CCollider2D;
		}

		else if (wstrComponentName == L"COLLIDER3D")
		{
			pComponent = new CCollider3D;
		}

		else if (wstrComponentName == L"ANIMATOR2D")
		{
			pComponent = new CAnimator2D;
		}

		else if (wstrComponentName == L"ANIMATOR3D")
		{
			pComponent = new CAnimator3D;
		}

		else if (wstrComponentName == L"LIGHT2D")
		{
			pComponent = new CLight2D;
		}

		else if (wstrComponentName == L"LIGHT3D")
		{
			pComponent = new CLight3D;
		}

		else if (wstrComponentName == L"PARTICLE")
		{
			pComponent = new CParticleSystem;
		}


		else if (wstrComponentName == L"FRUSTUMSPHERE")
		{
			pComponent = new CFrustumSphere;
		}

		else if (wstrComponentName == L"UI")
		{
			pComponent = new CUI;
		}


		//if (i == (UINT)COMPONENT_TYPE::END)
		if (wstrComponentName == L"END")
			break;
		else
		{
			pComponent->LoadFromScene(_pFile);
			AddComponent(pComponent);
		}
		
	}

	UINT iScriptCount = 0;
	fread(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{
		CScript* pScript = g_pScriptLoad(_pFile);
		AddComponent(pScript);
	}

	UINT iChildCount = 0;
	fread(&iChildCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		CGameObject* pChild = new CGameObject;
		pChild->LoadFromScene(_pFile);	
		int iLayerIdx = pChild->m_iLayerIdx;
		pChild->m_iLayerIdx = -1;
		AddChild(pChild);
		pChild->m_iLayerIdx = iLayerIdx;
	}

	//���� ���� �׺�޽����ٸ� ���ҽ� �Ŵ����� �����θ� ����Ѵ� 
	if(MeshRender())
	{
		if (nullptr != MeshRender()->GetMesh())
		{
			CMesh* pMesh = MeshRender()->GetMesh().Get();

			if (pMesh->IsNavMesh())
			{
				CResMgr::GetInst()->GetNavMeshVec().push_back(this);
				SetDynamicShadow(false);
			}
				
		}		
	}



}
#include "pch.h"
#include "CFSM.h"
#include "CState.h"
#include "CAnimator3D.h"

CFSM::CFSM()
    : m_pObject(nullptr)
    , m_pCurState(nullptr)
{
}

CFSM::~CFSM()
{
    for (const auto& pair : m_mapState)
    {
        delete pair.second;
    } 
}

void CFSM::update()
{
    if (nullptr != m_pCurState)
        m_pCurState->update();
}

void CFSM::lateupdate()
{
    if (nullptr != m_pCurState)
        m_pCurState->lateupdate();
}

void CFSM::AddState(wstring _strStateName, CState* _pState)
{
    CState* pState = FindState(_strStateName);
    assert(nullptr == pState);

    _pState->SetStateName(_strStateName);
    _pState->SetFSM(this);
    m_mapState.insert(make_pair(_strStateName, _pState));
}

CState* CFSM::FindState(wstring _strStateName)
{
    map<wstring, CState*>::iterator iter = m_mapState.find(_strStateName);

    if (m_mapState.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

void CFSM::ChangeState(wstring _strNextStateName, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
    CState* pNextState = FindState(_strNextStateName);
    assert(pNextState);

    if (nullptr != m_pCurState)
        m_pCurState->Exit(); // 이전 상태를 떠날때 처리할 일 

    m_pCurState = pNextState;

    m_pCurState->Enter(); //현재 상태로 돌입할때 처리할 일 


    //===========애니메이션 전환 부분 ==========================


    CAnimator3D* CurAnim = GetObj()->Animator3D();
    vector<tMTAnimClip>* vecAnimClip = CurAnim->GetMTAnimClip();
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


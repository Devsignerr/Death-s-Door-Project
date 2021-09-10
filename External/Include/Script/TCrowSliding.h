#pragma once
#include <Engine/CState.h>

class CCrowScript;
class TCrowSliding :
    public CState
{
private:
    CCrowScript* m_Script;
    int     m_SlidingCount;
    Vec3    m_SlidingDir;
    bool    m_SpecialPattern;
    int     m_SpecialPatternType;
    bool    m_SpecialPatternEndCheck;
    float   m_SlidingTime;
    int     m_PrevPoint;
    bool    m_IsSliding;

    Vec3    m_PlayerPos;
    float   m_Radius;
    float   m_PlayerToBossDistance;
    bool    m_ChainMoveEnd;
    Vec3    m_Point;
    int     m_SlidingTotalCount;
private:
    void SlidingPosSet(int _Point);
    void DiagonalPattern(int _StartPoint);
    void CrossPattern(int _StartPoint);
    void GoboardPattern(int _StartPoint);
    void EndMotion();
public:
    bool GetIsSliding() { return m_IsSliding; }
    void StompEnd() { m_IsSliding = false; }
private:
    void CreateChain();
    void ChainMoveEndCheck();
    bool EmptyChainCheck();


private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowSliding();
    ~TCrowSliding();
};

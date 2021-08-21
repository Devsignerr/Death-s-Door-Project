#pragma once
#include <Engine/CState.h>

class CCrowScript;
class TCrowJump :
    public CState
{
private:
	Vec3    m_Pos;
	float   m_Distance;
	float   m_Time;
	float   m_DestAttachTime;
	float	m_Gravity;
	float   m_VelocityX;
	float   m_VelocityY;
	float   m_VelocityZ;
	float   m_MaxHeight;
	float   m_MaxHeightTime;
	CCrowScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowJump();
    ~TCrowJump();
};


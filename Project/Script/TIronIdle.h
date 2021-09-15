#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronIdle : public CState
{
private:
	CIronmaceScript* m_Script;
private:
	void update() override;
private:
	void Enter() override;
	void Exit() override;
public:
	TIronIdle();
	~TIronIdle();
};


#pragma once
#include <Engine/CState.h>

class TIronCutScene : public CState
{
private:
	void update() override;
private:
	void Enter() override;
	void Exit() override;
public:
	TIronCutScene();
	~TIronCutScene();
};


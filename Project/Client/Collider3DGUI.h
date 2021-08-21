#pragma once
#include "ComponentGUI.h"
class Collider3DGUI :
	public ComponentGUI
{
private:
    int     m_iColliderTypeIdx;

public:
  void lateupdate() override;
  void render() override;

    Collider3DGUI();
    ~Collider3DGUI();
};


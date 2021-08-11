#include "pch.h"
#include "CRandomMgrScript.h"
#include <random>


CRandomMgrScript::CRandomMgrScript()
	: CScript((UINT)SCRIPT_TYPE::RANDOMMGRSCRIPT)
{

}

CRandomMgrScript::~CRandomMgrScript()
{
}

void CRandomMgrScript::awake()
{
}

void CRandomMgrScript::update()
{
}

int CRandomMgrScript::GetRandomintNumber(int _Start, int _End)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(_Start, _End);

		return dist(gen);
}

double CRandomMgrScript::GetRandomDoubleNumber(double _Start, double _End)
{
	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_double_distribution<double>  dist(_Start, _End);

		return dist(gen);*/
	return 0;
}

void CRandomMgrScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CRandomMgrScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
}
#include "pch.h"
#include "CPathMgr.h"
#include <io.h>

wchar_t CPathMgr::g_szResPath[255] = {};
vector<wstring> CPathMgr::g_VecFileName = {};
vector<wstring> CPathMgr::g_VecFilePath = {};
wstring CPathMgr::g_FilePath = {};

void CPathMgr::init()
{
	GetCurrentDirectory(255, g_szResPath);
	size_t iLen = wcslen(g_szResPath);

	for (size_t i = iLen - 1; i >= 0; --i)
	{
		if (L'\\' == g_szResPath[i])
		{
			g_szResPath[i] = 0;
			break;
		}		
	}

	wcscat_s(g_szResPath, L"\\bin\\content\\");
}

wstring CPathMgr::GetRelativePath(const wstring& _strPath)
{
	if (-1 == _strPath.find(g_szResPath))
		return nullptr;

	wstring strRelativePath = _strPath.substr(wcslen(g_szResPath), _strPath.length());

	return strRelativePath;
}

std::wstring CPathMgr::GetFileName(std::wstring& _path)
{
	std::wstring path = {};
	wchar_t filename[256] = {};

	path = wcsrchr(_path.c_str(), L'\\');

	size_t Len = wcslen(path.c_str());

	for (size_t i = 0; i < Len; ++i)
	{
		if (L'.' == path[i])
		{
			path[i] = 0;
			break;
		}

		filename[i] = path[i];
	}

	return filename + 1;
}

void CPathMgr::GetAllFilePath(const std::wstring& _path, const std::wstring& _filter)
{
	const std::wstring szFile = _path + _filter;

	_wfinddata_t fd;

	intptr_t handle = _wfindfirst(szFile.c_str(), &fd);

	if (handle == -1L)
		return;
	do
	{
		g_VecFilePath.push_back(fd.name);

	} while (_wfindnext(handle, &fd) == 0);


	_findclose(handle);

}

void CPathMgr::GetAllFileName(const std::wstring& _path, const std::wstring& _filter)
{

	const std::wstring szFile = _path + _filter;

	_wfinddata_t fd;

	intptr_t handle = _wfindfirst(szFile.c_str(), &fd);

	if (handle == -1L)
		return;
	do
	{
		g_VecFileName.push_back(fd.name);

	} while (_wfindnext(handle, &fd) == 0);

	for (size_t i = 0; i < g_VecFileName.size(); ++i)
	{
		std::wstring::size_type stTmp;
		stTmp = g_VecFileName[i].find(L".pref", 0);
		g_VecFileName[i].erase(stTmp, 5);
	}

	_findclose(handle);
}

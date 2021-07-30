#include "pch.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_szResPath[255] = {};

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

#pragma once

class CPathMgr
{
private:
	static wchar_t g_szResPath[255];
	static vector<wstring> g_VecFileName;
	static vector<wstring> g_VecFilePath;
	static wstring g_FilePath;
public: 
	static void init();

	static wchar_t* GetResPath()
	{
		return g_szResPath;
	}


	static wstring GetRelativePath(const wstring& _strPath);
	
public:
	static std::wstring GetFileName(std::wstring& _path);

	static void GetAllFilePath(const std::wstring& _path, const std::wstring& _filter);
	static void GetAllFileName(const std::wstring& _path, const std::wstring& _filter);

	static vector<wstring> GetVecFileName()
	{
		return g_VecFileName;
	}

	static vector<wstring> GetVecFilePath()
	{
		return g_VecFilePath;
	}

private:
	CPathMgr()
	{}
	~CPathMgr()
	{}
};


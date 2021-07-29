#pragma once

extern tTransform g_transform;
extern tGlobalValue g_global;

class CScript;
typedef void (*pSaveFunc)(CScript*, FILE*);
typedef CScript* (*pLoadFunc)(FILE*);

extern pSaveFunc g_pScriptSave;
extern pLoadFunc g_pScriptLoad;

extern const wchar_t* g_szResName[(UINT)RES_TYPE::END];
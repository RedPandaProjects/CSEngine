#include "CSEngine.hpp"
ui_enginefuncs_t FUIEngine;
static void FNull(...)
{
	BearLog::DebugPrintf(TEXT("~ Function Null"));
#ifdef RELEASE
	BEAR_FATALERROR(false, TEXT("Вызван Null,неполная комплектация экспортных функций!!!"));
#endif
}
void CSEngine::FillListFunction(void* Functions_, bsize count)
{
	void(**Functions)(...) = (void(**)(...))Functions_;
	while (count--)
	{
		*Functions = &FNull;
		Functions++;
	}
}
static void* MallocExport(size_t size, const char* file, int line)
{
	return bear_alloc<uint8>(size);
}
static void FreeExport(void* buf, const char* file, int line)
{
	bear_free(buf);
}
static int GetGameInfoExport(GAMEINFO* info)
{
	bear_fill(*info);
#ifdef UNICODE
	BearString::Copy(info->gamefolder, *BearEncoding::FastToAnsi(GGameName));
	BearString::Copy(info->title, *BearEncoding::FastToAnsi(GGameName));
#else 
	BearString::Copy(info->gamefolder, GGameName);
	BearString::Copy(info->title, GGameName);
#endif
	return 0;
}
static int CreateMapListExport(int)
{
	//BearSource::BearSource::MakeMapList();
	return 1;
}
static void PrintfExport(char* text_, ...)
{
	BearStringAnsi4096 string_out;
	va_list va;
	va_start(va, text_);
	BearString::PrintfVa(string_out, text_, va);
	va_end(va);
	BearLog::Printf(TEXT(BEAR_PRINT_STR), string_out);
}
static void NPrintfExport(int ind, char* text_, ...)
{
	BearStringAnsi4096 string_out;
	va_list va;
	va_start(va, text_);
	BearString::PrintfVa(string_out, text_, va);
	va_end(va);
	BearLog::Printf(TEXT("NPrint[%d] [") TEXT(BEAR_PRINT_STR) TEXT("]"), ind, string_out);
}
static int FileExistExport(const char* file, int dironly)
{
	if (dironly)
	{
		BearStringPath dirgame;
		GFS->UpdatePath(TEXT("%game%"), 0, dirgame);
		BearString::Contact(dirgame, BEAR_PATH);
		BearString::Contact(dirgame,
#ifdef UNICODE
			*BearEncoding::FastToUnicode(file)
#else
			file
#endif
		);

		return BearFileManager::FileExists(dirgame);
	}
	else
	{
		return	GFS->ExistFile(TEXT("%game%"),
			
#ifdef UNICODE
			* BearEncoding::FastToUnicode(file)
#else
			file
#endif
			);
	}
}
void CSEngine::FunctionToC()
{
	FillListFunction(&FUIEngine, sizeof(FUIEngine) / sizeof(void(*)(...)));
	FUIEngine.pfnMemAlloc = MallocExport;
	FUIEngine.pfnMemFree = FreeExport;
	FUIEngine.pfnGetGameInfo = GetGameInfoExport;
	FUIEngine.pfnCreateMapsList = CreateMapListExport;
	FUIEngine.Con_Printf = PrintfExport;
	FUIEngine.Con_NPrintf = NPrintfExport;
	FUIEngine.pfnFileExists = FileExistExport;
}

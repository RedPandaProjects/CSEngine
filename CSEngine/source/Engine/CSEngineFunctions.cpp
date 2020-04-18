#include "CSEngine.hpp"
extern const bchar* GDLLName;
extern const bchar* GGameName;
 SEngineMenuFunctions GMenuEngineFunctions;
 SServerEngineFunction GServerEngineFunction;
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
static void PrintExport(const char* text)
{
	BearLog::Printf(TEXT(BEAR_PRINT_STR), text);
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

static int ClientInGameExport()
{
	return 0;
}
static void DrawLogoExport(const char* name, float, float, float, float)
{
	
}
static int CheckGameDllExport()
{
	BearString64 name;
	BearString::Copy(name, GDLLName);
	BearString::Contact(name, TEXT("MP"));
	return BearManagerProjects::CheckProject(name);

}
static  int	IsMapValidExport(char* filename)
{
	return 1;
}
static void GetGameDirExport(char* out)
{
	BearString::Copy(out, 64,* BearEncoding::FastToAnsi(GGameName));
}
static int IsDedicatedServerExport()
{
	return 0;
}
static int EngCheckParmExport(const char* param,char**)
{
	return !!strstr(GetCommandLineA(), param);
}
void CSEngine::FunctionToC()
{
	FillListFunction(&GMenuEngineFunctions, sizeof(GMenuEngineFunctions) / sizeof(void(*)(...)));
	GMenuEngineFunctions.MemAlloc = MallocExport;
	GMenuEngineFunctions.MemFree = FreeExport;
	GMenuEngineFunctions.GetGameInfo = GetGameInfoExport;
	GMenuEngineFunctions.CreateMapsList = CreateMapListExport;
	GMenuEngineFunctions.Con_Printf = PrintfExport;
	GMenuEngineFunctions.Con_NPrintf = NPrintfExport;
	GMenuEngineFunctions.FileExists = FileExistExport;
	GMenuEngineFunctions.ClientInGame = ClientInGameExport;
	GMenuEngineFunctions.DrawLogo = DrawLogoExport;
	GMenuEngineFunctions.CheckGameDll = CheckGameDllExport;
	GMenuEngineFunctions.IsMapValid = IsMapValidExport;

	FillListFunction(&GServerEngineFunction, sizeof(GServerEngineFunction) / sizeof(void(*)(...)));
	GServerEngineFunction.GetGameDir = GetGameDirExport;
	GServerEngineFunction.IsDedicatedServer = IsDedicatedServerExport;
	GServerEngineFunction.EngCheckParm = EngCheckParmExport;
	GServerEngineFunction.ServerPrint = PrintExport;

}

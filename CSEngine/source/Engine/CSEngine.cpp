#include "CSEngine.hpp"
CSEngine* GEngine = 0;
const bchar* GGameName = TEXT("cstrike");
const bchar* GBaseDir = TEXT("valve");
const bchar* GDLLName = TEXT("CS");
CSEngine::CSEngine()
{
}

CSEngine::~CSEngine()
{
}

void CSEngine::Startup()
{
	BearCore::Initialize(TEXT("cs16"));
	GEngine = bear_new<CSEngine>();
	GEngine->Initialize();
	GEngine->Loop();
	GEngine->Destroy();
	bear_delete(GEngine);
	BearCore::Destroy();
}

void CSEngine::Initialize()
{
	BearLog::Printf(TEXT("BearSource build %s"), *BearLog::GetBuild(2018, 12, 25));

	BearStringPath FSFilePath;
	BearFileManager::GetApplicationPath(FSFilePath);

	BearString::Contact(FSFilePath, BEAR_PATH);
	BearString::Contact(FSFilePath, TEXT(".."));
	BearString::Contact(FSFilePath, BEAR_PATH);
	BearString::Contact(FSFilePath, TEXT("cs16.fs"));

	GFS = bear_new<BearFileSystem>();
	BEAR_ERRORMESSAGE(GFS->LoadFromFile(FSFilePath, BearEncoding::UTF8), TEXT("Не найден %s"), TEXT("cs16.fs"));

	

	InitializeGame();
	InitializeDLL();
	FunctionToC();
	GCommonParser = bear_new<CSCommonParser>();
	GCVars = bear_new< CSCVars>();
	GConsole = bear_new<CSConsole>();
	GHW = bear_new<CSHardware>();

	GRender = bear_new< CSRender>();
	GResourcesManager = bear_new<CSResourcesManager>();
	GMenuManager = bear_new< CSMenuManager>();
}

void CSEngine::Loop()
{

	while (GHW->Window.Update())
	{
		GHW->Context->ClearFrameBuffer();
		GMenuManager->Draw();
		GHW->Context->Flush(true);
	}

}

void CSEngine::DestroyDLL()
{
	BearString64 name;
	{
		BearString::Copy(name, GDLLName);
		BearString::Contact(name, TEXT("Menu"));
		BearManagerProjects::UnLoad(name);
	}
}

void CSEngine::Destroy()
{
	bear_delete(GRender);
	bear_delete(GMenuManager);
	bear_delete(GResourcesManager);
	bear_delete(GHW);
	bear_delete(GConsole);
	bear_delete(GCVars);
	bear_delete(GCommonParser);
	
	bear_delete(GFS);
}

void CSEngine::MakeMapList()
{
}

void CSEngine::InitializeDLL()
{
	BearString64 name;
	{
		BearString::Copy(name, GDLLName);
		BearString::Contact(name, TEXT("Menu"));
		FMenuAPI = BearManagerProjects::GetFunctionInProject<MENUAPI>(name, TEXT("GetMenuAPI"));
	}
}

void CSEngine::InitializeGame()
{
	GFS->AppendPath(TEXT("%game%"), GBaseDir, TEXT("%content%"), 0);
	if (BearString::Compare(GBaseDir, GGameName) == 0)return;
	GFS->AppendPath(TEXT("%game%"), GGameName, TEXT("%content%"), 1);

}



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
	GKeyState = bear_new<CSKeyState>();

	GHW = bear_new<CSHardware>();

	GSound = bear_new<CSSound>();

	GResourcesManager = bear_new<CSResourcesManager>();

	GRender = bear_new< CSRender>();

	GMenuManager = bear_new< CSMenuManager>();
	GMPManager = bear_new<CSMPManager>();
	GMPManager->Initialize();
}

void CSEngine::Loop()
{

	while (GHW->Window.Update())
	{
		BearEventWindows Event;
		{
		
			while (GHW->Window.GetEvent(Event))
			{
				switch (Event.Type)
				{
				case WET_KeyDown:
					GKeyState->Event(Event.Key, true);
					break;
				case WET_KeyUp:
					GKeyState->Event(Event.Key, false);
					break;
				case WET_MouseMove:
					GMenuManager->MouseEvent(int(Event.Position.x),int( Event.Position.y));
				default:
					break;
				}
			}
		}
		GHW->Context->ClearFrameBuffer();
		GMenuManager->Draw();
		GHW->Context->Flush(true);
		GConsole->Execute();
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
	bear_delete(GMPManager);
	bear_delete(GRender);
	bear_delete(GMenuManager);
	bear_delete(GResourcesManager);
	bear_delete(GSound);
	bear_delete(GHW);
	bear_delete(GKeyState);
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
		FGETMenuAPI = BearManagerProjects::GetFunctionInProject<FGET_MENU_FUNCIOTNS>(name, TEXT("GetMenuAPI"));
	}
	{
		BearString::Copy(name, GDLLName);
		BearString::Contact(name, TEXT("MP"));
		FGETServerAPI = BearManagerProjects::GetFunctionInProject <FGET_SERVER_FUNCTIONS>(name, TEXT("GetEntityAPI"));
		FGETServerNewAPI =BearManagerProjects::GetFunctionInProject<FGET_SERVER_NEW_FUNCTIONS>(name, TEXT("GetNewDLLFunctions"));
		FSETServerAPI = BearManagerProjects::GetFunctionInProject < FSET_SERVERENGINE_FUNCTIONS>(name, TEXT("GiveFnptrsToDll"));
	
	}
}

void CSEngine::InitializeGame()
{
	GFS->AppendPath(TEXT("%game%"), GBaseDir, TEXT("%content%"), 0);
	if (BearString::Compare(GBaseDir, GGameName) == 0)return;
	GFS->AppendPath(TEXT("%game%"), GGameName, TEXT("%content%"), 1);

}



#include "CSEngine.hpp"
FGET_MENU_FUNCIOTNS FGETMenuAPI = 0;
SUIFunctions     FUI ;
ui_globalvars_t   FUIGlobalVars;
CSMenuManager* GMenuManager = 0;
CSMenuManager::CSMenuManager()
{
	BEAR_ASSERT(FGETMenuAPI);
	bear_fill(FUIGlobalVars);

	FUIGlobalVars.scrWidth = 0x400;
	FUIGlobalVars.scrHeight = 0x300;

	BEAR_ERRORMESSAGE(FGETMenuAPI(&FUI, &GMenuEngineFunctions, &FUIGlobalVars), TEXT("Ошибка инцылизации [%s_menu]"), *GGameName);
	FUI.Init();

	GCVars->SetValue("ui_precache", 2);

	FUI.VidInit();
	FUI.SetActiveMenu(1);

}

void CSMenuManager::Draw()
{
	FUI.Redraw(0);
	GRender->FlushUI();
}

void CSMenuManager::KeyEvent(int key, bool down)
{

	if(FUI.KeyEvent)
		FUI.KeyEvent(key, down);
	
}

void CSMenuManager::MouseEvent(int x, int y)
{
	FUI.MouseMove(x, y);
	FUI.SetCursorPos(x, y);
}

CSMenuManager::~CSMenuManager()
{
	FUI.Shutdown();
}


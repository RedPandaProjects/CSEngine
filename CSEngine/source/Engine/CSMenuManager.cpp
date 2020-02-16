#include "CSEngine.hpp"
MENUAPI  FMenuAPI = 0;
UI_FUNCTIONS     FUI ;
ui_globalvars_t   FUIGlobalVars;
CSMenuManager* GMenuManager = 0;
CSMenuManager::CSMenuManager()
{
	BEAR_ASSERT(FMenuAPI);
	bear_fill(FUIGlobalVars);

	FUIGlobalVars.scrWidth = 0x400;
	FUIGlobalVars.scrHeight = 0x300;

	BEAR_ERRORMESSAGE(FMenuAPI(&FUI, &FUIEngine, &FUIGlobalVars), TEXT("Ошибка инцылизации [%s_menu]"), *GGameName);
	FUI.pfnInit();

	GCVars->SetValue("ui_precache", 2);

	FUI.pfnVidInit();
	FUI.pfnSetActiveMenu(1);

}

void CSMenuManager::Draw()
{
	FUI.pfnRedraw(0);
	GRender->FlushUI();
}

CSMenuManager::~CSMenuManager()
{
	FUI.pfnShutdown();
}

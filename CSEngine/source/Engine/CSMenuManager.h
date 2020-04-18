#pragma once
class CSMenuManager
{
	BEAR_CLASS_WITHOUT_COPY(CSMenuManager);
public:
	CSMenuManager();
	void Draw();
	void KeyEvent(int key, bool down);
	void MouseEvent(int x, int y);
	~CSMenuManager();
};
extern CSMenuManager* GMenuManager;
extern SEngineMenuFunctions GEngineMenuFunctions;
extern FGET_MENU_FUNCIOTNS  FGETMenuAPI;
extern ui_globalvars_t   FUIGlobalVars;
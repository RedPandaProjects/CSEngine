#pragma once
class CSMenuManager
{
	BEAR_CLASS_WITHOUT_COPY(CSMenuManager);
public:
	CSMenuManager();
	void Draw();
	~CSMenuManager();

};
extern CSMenuManager* GMenuManager;
extern MENUAPI  FMenuAPI;
extern ui_globalvars_t   FUIGlobalVars;
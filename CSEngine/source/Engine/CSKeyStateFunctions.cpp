#include "CSEngine.hpp"

#include "engine/keydefs.h"
struct KeyName
{
	char* Name;    // key name
	int        Keynum;   // key number
	const char* Binding; // default bind
};
static KeyName LKeyNames[] =
{
{"TAB",		K_TAB,		""		},
{"ENTER",		K_ENTER,		""		},
{"ESCAPE",	K_ESCAPE, 	"cancelselect"	}, // hardcoded
{"SPACE",		K_SPACE,		"+moveup"		},
{"BACKSPACE",	K_BACKSPACE,	""		},
{"UPARROW",	K_UPARROW,	"+forward"	},
{"DOWNARROW",	K_DOWNARROW,	"+back"		},
{"LEFTARROW",	K_LEFTARROW,	"+left"		},
{"RIGHTARROW",	K_RIGHTARROW,	"+right"		},
{"ALT",		K_ALT,		"+strafe"		},
{"CTRL",		K_CTRL,		"+attack"		},
{"SHIFT",		K_SHIFT,		"+speed"		}, // replace with +attack2 ?
{"CAPSLOCK",	K_CAPSLOCK,	""		},
{"F1",		K_F1,		"cmd help"	},
{"F2",		K_F2,		"menu_savegame"	},
{"F3",		K_F3,		"menu_loadgame"	},
{"F4",		K_F4,		"menu_keys"	},
{"F5",		K_F5,		"menu_startserver"	},
{"F6",		K_F6,		"savequick"	},
{"F7",		K_F7,		"loadquick"	},
{"F8",		K_F8,		"stop"		},
{"F9",		K_F9,		""		},
{"F10",		K_F10,		"menu_quit"	},
{"F11",		K_F11,		""		},
{"F12",		K_F12,		"screenshot"	},
{"INS",		K_INS,		""		},
{"DEL",		K_DEL,		"+lookdown"	},
{"PGDN",		K_PGDN,		"+lookup"		},
{"PGUP",		K_PGUP,		""		},
{"HOME",		K_HOME,		""		},
{"END",		K_END,		"centerview"	},

// mouse buttouns
{"MOUSE1",	K_MOUSE1,		"+attack"		},
{"MOUSE2",	K_MOUSE2,		"+attack2"	},
{"MOUSE3",	K_MOUSE3,		""		},
{"MOUSE4",	K_MOUSE4,		""		},
{"MOUSE5",	K_MOUSE5,		""		},
{"MWHEELUP",	K_MWHEELUP,	""		},
{"MWHEELDOWN",	K_MWHEELDOWN,	""		},

// digital keyboard
{"KP_HOME",	K_KP_HOME,	""		},
{"KP_UPARROW",	K_KP_UPARROW,	"+forward"	},
{"KP_PGUP",	K_KP_PGUP,	""		},
{"KP_LEFTARROW",	K_KP_LEFTARROW,	"+left"		},
{"KP_5",		K_KP_5,		""		},
{"KP_RIGHTARROW",	K_KP_RIGHTARROW,	"+right"		},
{"KP_END",	K_KP_END,		"centerview"	},
{"KP_DOWNARROW",	K_KP_DOWNARROW,	"+back"		},
{"KP_PGDN",	K_KP_PGDN,	"+lookup" 	},
{"KP_ENTER",	K_KP_ENTER,	""		},
{"KP_INS",	K_KP_INS,		""		},
{"KP_DEL",	K_KP_DEL,		"+lookdown"	},
{"KP_SLASH",	K_KP_SLASH,	""		},
{"KP_MINUS",	K_KP_MINUS,	""		},
{"KP_PLUS",	K_KP_PLUS,	""		},
{"PAUSE",		K_PAUSE,		"pause"		},
{"A_BUTTON", K_A_BUTTON, ""}, // they match xbox controller
{"B_BUTTON", K_B_BUTTON, ""},
{"X_BUTTON", K_X_BUTTON, ""},
{"Y_BUTTON", K_Y_BUTTON, ""},
{"L1_BUTTON",  K_L1_BUTTON, ""},
{"R1_BUTTON",  K_R1_BUTTON, ""},
{"BACK",   K_BACK_BUTTON, ""},
{"MODE",   K_MODE_BUTTON, ""},
{"START",  K_START_BUTTON, ""},
{"STICK1", K_LSTICK, ""},
{"STICK2", K_RSTICK, ""},
{"L2_BUTTON", K_L2_BUTTON, ""}, // in case...
{"R2_BUTTON", K_R2_BUTTON, ""},
{"C_BUTTON", K_C_BUTTON, ""},
{"Z_BUTTON", K_Z_BUTTON, ""},
{"AUX16", K_AUX16, ""}, // generic
{"AUX17", K_AUX17, ""},
{"AUX18", K_AUX18, ""},
{"AUX19", K_AUX19, ""},
{"AUX20", K_AUX20, ""},
{"AUX21", K_AUX21, ""},
{"AUX22", K_AUX22, ""},
{"AUX23", K_AUX23, ""},
{"AUX24", K_AUX24, ""},
{"AUX25", K_AUX25, ""},
{"AUX26", K_AUX26, ""},
{"AUX27", K_AUX27, ""},
{"AUX28", K_AUX28, ""},
{"AUX29", K_AUX29, ""},
{"AUX30", K_AUX30, ""},
{"AUX31", K_AUX31, ""},
{"AUX32", K_AUX32, ""},
{"LTRIGGER" , K_JOY1 , ""},
{"RTRIGGER" , K_JOY2 , ""},
{"JOY3" , K_JOY3 , ""},
{"JOY4" , K_JOY4 , ""},
// raw semicolon seperates commands
{"SEMICOLON", ';', ""},
{NULL, 0, NULL},
};
static const char* KeynumToStringExport(int keynum)
{
	KeyName* kn;
	static char	tinystr[5];
	int		i, j;

	if (keynum == -1) return "<KEY NOT FOUND>";
	if (keynum < 0 || keynum > 255) return "<OUT OF RANGE>";

	// check for printable ascii (don't use quote)
	if (keynum > 32 && keynum < 127 && keynum != '"' && keynum != ';')
	{
		tinystr[0] = char(keynum);
		tinystr[1] = 0;
		return tinystr;
	}

	// check for a key string
	for (kn = LKeyNames; kn->Name; kn++)
	{
		if (keynum == kn->Keynum)
			return kn->Name;
	}

	// make a hex string
	i = keynum >> 4;
	j = keynum & 15;

	tinystr[0] = '0';
	tinystr[1] = 'x';
	tinystr[2] = char(i > 9 ? i - 10 + 'a' : i + '0');
	tinystr[3] = char(j > 9 ? j - 10 + 'a' : j + '0');
	tinystr[4] = 0;

	return tinystr;
}
static const char* GetBindingExport(int key)
{
	const char*result  = *GKeyState->GetBinding(key);
	if (result[0] == 0)return nullptr;
	return result;
}
static void SetBindingExport(int key, const char* bind)
{
	if (bind == nullptr)bind = "";
	GKeyState->SetBinding(key, bind);
}
static void KeyClearStatesExport()
{
	GKeyState->ClearState();
}
static int GetKeyIsDownExport(int key)
{
	return GKeyState->IsDown(key);
}

static void SetKeyDestExport(int dest)
{

}
static int	KeyGetOverstrikeModeExport() 
{
	return 0;
}
static void	KeySetOverstrikeModeExport(int fActive)
{

}
static void* KeyGetStateExport(const char* name)
{
	return NULL;
}
void CSKeyState::FunctionToC()
{
	GMenuEngineFunctions.KeynumToString = KeynumToStringExport;
	GMenuEngineFunctions.KeyGetBinding = GetBindingExport;
	GMenuEngineFunctions.KeySetBinding = SetBindingExport;
	GMenuEngineFunctions.KeyIsDown = GetKeyIsDownExport;
	GMenuEngineFunctions.KeyClearStates = KeyClearStatesExport;
	GMenuEngineFunctions.SetKeyDest = SetKeyDestExport;
	GMenuEngineFunctions.KeyGetOverstrikeMode = KeyGetOverstrikeModeExport;
	GMenuEngineFunctions.KeySetOverstrikeMode = KeySetOverstrikeModeExport;
	GMenuEngineFunctions.KeyGetState = KeyGetStateExport;

}

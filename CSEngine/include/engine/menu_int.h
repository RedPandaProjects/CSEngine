#pragma once

typedef int		HIMAGE;		// handle to a graphic

// flags for PIC_Load
#define PIC_NEAREST		(1<<0)		// disable texfilter
#define PIC_KEEP_RGBDATA	(1<<1)		// some images keep source
#define PIC_NOFLIP_TGA	(1<<2)		// Steam background completely ignore tga attribute 0x20
#define PIC_KEEP_8BIT	(1<<3)		// keep original 8-bit image (if present)

typedef struct ui_globalvars_s
{	
	float		time;		// unclamped host.realtime
	float		frametime;

	int		scrWidth;		// actual values
	int		scrHeight;

	int		maxClients;
	int		developer;
	int		demoplayback;
	int		demorecording;
	char		demoname[64];	// name of currently playing demo
	char		maptitle[64];	// title of active map
} ui_globalvars_t;

typedef struct SEngineMenuFunction
{
	// image handlers
	HIMAGE	(*PIC_Load)( const char *szPicName, const byte *ucRawImage, long ulRawImageSize, long flags );
	void	(*PIC_Free)( const char *szPicName );
	int	(*PIC_Width)( HIMAGE hPic );
	int	(*PIC_Height)( HIMAGE hPic );
	void	(*PIC_Set)( HIMAGE hPic, int r, int g, int b, int a );
	void	(*PIC_Draw)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*PIC_DrawHoles)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*PIC_DrawTrans)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*PIC_DrawAdditive)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*PIC_EnableScissor)( int x, int y, int width, int height );
	void	(*PIC_DisableScissor)( void );

	// screen handlers
	void	(*FillRGBA)( int x, int y, int width, int height, int r, int g, int b, int a );

	// cvar handlers
	cvar_t*	(*RegisterVariable)( const char *szName, const char *szValue, int flags );
	float	(*GetCvarFloat)( const char *szName );
	char*	(*GetCvarString)( const char *szName );
	void	(*CvarSetString)( const char *szName, const char *szValue );
	void	(*CvarSetValue)( const char *szName, float flValue );

	// command handlers
	int	(*AddCommand)( const char *cmd_name, void (*function)(void) );
	void	(*ClientCmd)( int execute_now, const char *szCmdString );
	void	(*DelCommand)( const char *cmd_name );
	int       (*CmdArgc)( void );	
	char*	(*CmdArgv)( int argc );
	char*	(*Cmd_Args)( void );

	// debug messages (in-menu shows only notify)	
	void	(*Con_Printf)( char *fmt, ... );
	void	(*Con_DPrintf)( char *fmt, ... );
	void	(*Con_NPrintf)( int pos, char *fmt, ... );
	void	(*Con_NXPrintf)( struct con_nprint_s *info, char *fmt, ... );

	// sound handlers
	void	(*PlayLocalSound)( const char *szSound );

	// cinematic handlers
	void	(*DrawLogo)( const char *filename, float x, float y, float width, float height );
	int	(*GetLogoWidth)( void );
	int	(*GetLogoHeight)( void );
	float	(*GetLogoLength)( void );	// cinematic duration in seconds

	// text message system
	void	(*DrawCharacter)( int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont );
	int	(*DrawConsoleString)( int x, int y, const char *string );
	void	(*DrawSetTextColor)( int r, int g, int b, int alpha );
	void	(*DrawConsoleStringLen)(  const char *string, int *length, int *height );
	void	(*SetConsoleDefaultColor)( int r, int g, int b ); // color must came from colors.lst

	// custom rendering (for playermodel preview)
	struct cl_entity_s* (*GetPlayerModel)( void );	// for drawing playermodel previews
	void	(*SetModel)( struct cl_entity_s *ed, const char *path );
	void	(*ClearScene)( void );
	void	(*RenderScene)( const struct ref_params_s *fd );
	int	(*CL_CreateVisibleEntity)( int type, struct cl_entity_s *ent );

	// misc handlers
	void	(*HostError)( const char *szFmt, ... );
	int	(*FileExists)( const char *filename, int gamedironly );
	void	(*GetGameDir)( char *szGetGameDir );

	// gameinfo handlers
	int	(*CreateMapsList)( int fRefresh );
	int	(*ClientInGame)( void );
	void	(*ClientJoin)( const struct netadr_s adr );
	
	// parse txt files
	byte*	(*COM_LoadFile)( const char *filename, int *pLength );
	char*	(*COM_ParseFile)( char *data, char *token );
	void	(*COM_FreeFile)( void *buffer );

	// keyfuncs
	void	(*KeyClearStates)( void );				// call when menu open or close
	void	(*SetKeyDest)( int dest );
	const char *(*KeynumToString)( int keynum );
	const char *(*KeyGetBinding)( int keynum );
	void	(*KeySetBinding)( int keynum, const char *binding );
	int	(*KeyIsDown)( int keynum );
	int	(*KeyGetOverstrikeMode)( void );
	void	(*KeySetOverstrikeMode)( int fActive );
	void	*(*KeyGetState)( const char *name );			// for mlook, klook etc

	// engine memory manager
	void*	(*MemAlloc)( size_t cb, const char *filename, const int fileline );
	void	(*MemFree)( void *mem, const char *filename, const int fileline );

	// collect info from engine
	int	(*GetGameInfo)( GAMEINFO *pgameinfo );
	GAMEINFO	**(*GetGamesList)( int *numGames );			// collect info about all mods
	char 	**(*GetFilesList)( const char *pattern, int *numFiles, int gamedironly );	// find in files
	int 	(*GetSaveComment)( const char *savename, char *comment );
	int	(*GetDemoComment)( const char *demoname, char *comment );
	int	(*CheckGameDll)( void );				// returns false if hl.dll is missed or invalid
	char	*(*GetClipboardData)( void );

	// engine launcher
	void	(*ShellExecute)( const char *name, const char *args, int closeEngine );
	void	(*WriteServerConfig)( const char *name );
	void	(*ChangeInstance)( const char *newInstance, const char *szFinalMessage );
	void	(*PlayBackgroundTrack)( const char *introName, const char *loopName );
	void	(*HostEndGame)( const char *szFinalMessage );

	// menu interface is freezed at version 0.75
	// new functions starts here 
	float	(*RandomFloat)( float flLow, float flHigh );	
	long	(*RandomLong)( long lLow, long lHigh );

	void	(*SetCursor)( void *hCursor );			// change cursor
	int	(*IsMapValid)( char *filename );
	void	(*ProcessImage)( int texnum, float gamma, int topColor, int bottomColor );
	int	(*CompareFileTime)( char *filename1, char *filename2, int *iCompare );
} SEngineMenuFunctions;

typedef struct ui_textfuncs_s {
	void (*EnableTextInput)( int enable );
	int (*UtfProcessChar) ( int ch );
	int (*UtfMoveLeft) ( char *str, int pos );
	int (*UtfMoveRight) ( char *str, int pos, int length );
} ui_textfuncs_t;

typedef struct
{
	int	(*VidInit)( void );
	void	(*Init)( void );
	void	(*Shutdown)( void );
	void	(*Redraw)( float flTime );
	void	(*KeyEvent)( int key, int down );
	void	(*MouseMove)( int x, int y );
	void	(*SetActiveMenu)( int active );
	void	(*AddServerToList)( struct netadr_s adr, const char *info );
	void	(*GetCursorPos)( int *pos_x, int *pos_y );
	void	(*SetCursorPos)( int pos_x, int pos_y );
	void	(*ShowCursor)( int show );
	void	(*CharEvent)( int key );
	int	(*MouseInRect)( void );	// mouse entering\leave game window
	int	(*IsVisible)( void );
	int	(*CreditsActive)( void );	// unused
	void	(*FinalCredits)( void );	// show credits + game end
} SUIFunctions;

typedef int (*FGET_MENU_FUNCIOTNS)(SUIFunctions*pFunctionTable, SEngineMenuFunctions* engfuncs, ui_globalvars_t *pGlobals );

typedef int (*UITEXTAPI)( ui_textfuncs_t* engfuncs );


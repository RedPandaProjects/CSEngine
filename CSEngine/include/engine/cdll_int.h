/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  cdll_int.h
//
// 4-23-98
// JOHN:  client dll interface declarations
//
#pragma once

typedef int (*UserMsgHook)(const char* pszName, int iSize, void* pbuf);


// this file is included by both the engine and the client-dll,
// so make sure engine declarations aren't done twice

typedef int HSPRITE;	// handle to a graphic

#define SCRINFO_SCREENFLASH 1
#define SCRINFO_STRETCHED	2

typedef struct SCREENINFO_s
{
	int		iSize;
	int		iWidth;
	int		iHeight;
	int		iFlags;
	int		iCharHeight;
	short	charWidths[256];
} SCREENINFO;


typedef struct client_data_s
{
	// fields that cannot be modified  (ie. have no effect if changed)
	vec3_t origin;

	// fields that can be changed by the cldll
	vec3_t viewangles;
	int		iWeaponBits;
	float	fov;	// field of view
} client_data_t;

typedef struct client_sprite_s
{
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	wrect_t rc;
} client_sprite_t;

typedef struct client_textmessage_s
{
	int		effect;
	byte	r1, g1, b1, a1;		// 2 colors for effects
	byte	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char *pName;
	const char *pMessage;
} client_textmessage_t;

typedef struct hud_player_info_s
{
	char *name;
	short ping;
	byte thisplayer;  // TRUE if this is the calling player

  // stuff that's unused at the moment,  but should be done
	byte spectator;
	byte packetloss;

	char *model;
	short topcolor;
	short bottomcolor;

} hud_player_info_t;


typedef struct cl_enginefuncs_s
{
	// sprite handlers
	HSPRITE						( *SPR_Load )			( const char *szPicName );
	int							( *SPR_Frames )			( HSPRITE hPic );
	int							( *SPR_Height )			( HSPRITE hPic, int frame );
	int							( *SPR_Width )			( HSPRITE hPic, int frame );
	void						( *SPR_Set )				( HSPRITE hPic, int r, int g, int b );
	void						( *SPR_Draw )			( int frame, int x, int y, const wrect_t *prc );
	void						( *SPR_DrawHoles )		( int frame, int x, int y, const wrect_t *prc );
	void						( *SPR_DrawAdditive )	( int frame, int x, int y, const wrect_t *prc );
	void						( *SPR_EnableScissor )	( int x, int y, int width, int height );
	void						( *SPR_DisableScissor )	( void );
	client_sprite_t				*( *SPR_GetList )			( char *psz, int *piCount );

	// screen handlers
	void						( *FillRGBA )			( int x, int y, int width, int height, int r, int g, int b, int a );
	int							( *GetScreenInfo ) 		( SCREENINFO *pscrinfo );
	void						( *SetCrosshair )		( HSPRITE hspr, wrect_t rc, int r, int g, int b );

	// cvar handlers
	struct cvar_s				*( *RegisterVariable )	( char *szName, char *szValue, int flags );
	float						( *GetCvarFloat )		( char *szName );
	char*						( *GetCvarString )		( char *szName );

	// command handlers
	int							( *AddCommand )			( char *cmd_name, void (*function)(void) );
	int							( *HookUserMsg )			( char *szMsgName, UserMsgHook  );
	int							( *ServerCmd )			( char *szCmdString );
	int							( *ClientCmd )			( char *szCmdString );

	void						( *GetPlayerInfo )		( int ent_num, hud_player_info_t *pinfo );

	// sound handlers
	void						( *PlaySoundByName )		( char *szSound, float volume );
	void						( *PlaySoundByIndex )	( int iSound, float volume );

	// vector helpers
	void						( *AngleVectors )		( const float * vecAngles, float * forward, float * right, float * up );

	// text message system
	client_textmessage_t		*( *TextMessageGet )		( const char *pName );
	int							( *DrawCharacter )		( int x, int y, int number, int r, int g, int b );
	int							( *DrawConsoleString )	( int x, int y, char *string );
	void						( *DrawSetTextColor )	( float r, float g, float b );
	void						( *DrawConsoleStringLen )(  const char *string, int *length, int *height );

	void						( *ConsolePrint )		( const char *string );
	void						( *CenterPrint )			( const char *string );


// Added for user input processing
	int							( *GetWindowCenterX )		( void );
	int							( *GetWindowCenterY )		( void );
	void						( *GetViewAngles )			( float * );
	void						( *SetViewAngles )			( float * );
	int							( *GetMaxClients )			( void );
	void						( *Cvar_SetValue )			( char *cvar, float value );

	int       					(*Cmd_Argc)					(void);
	char						*( *Cmd_Argv )				( int arg );
	void						( *Con_Printf )				( char *fmt, ... );
	void						( *Con_DPrintf )			( char *fmt, ... );
	void						( *Con_NPrintf )			( int pos, char *fmt, ... );
	void						( *Con_NXPrintf )			( struct con_nprint_s *info, char *fmt, ... );

	const char					*( *PhysInfo_ValueForKey )	( const char *key );
	const char					*( *ServerInfo_ValueForKey )( const char *key );
	float						( *GetClientMaxspeed )		( void );
	int							( *CheckParm )				( char *parm, char **ppnext );
	void						( *Key_Event )				( int key, int down );
	void						( *GetMousePosition )		( int *mx, int *my );
	int							( *IsNoClipping )			( void );

	struct cl_entity_s			*( *GetLocalPlayer )		( void );
	struct cl_entity_s			*( *GetViewModel )			( void );
	struct cl_entity_s			*( *GetEntityByIndex )		( int idx );

	float						( *GetClientTime )			( void );
	void						( *V_CalcShake )			( void );
	void						( *V_ApplyShake )			( float *origin, float *angles, float factor );

	int							( *PM_PointContents )		( float *point, int *truecontents );
	int							( *PM_WaterEntity )			( float *p );
	struct pmtrace_s			*( *PM_TraceLine )			( float *start, float *end, int flags, int usehull, int ignore_pe );

	struct model_s				*( *CL_LoadModel )			( const char *modelname, int *index );
	int							( *CL_CreateVisibleEntity )	( int type, struct cl_entity_s *ent );

	const struct model_s *		( *GetSpritePointer )		( HSPRITE hSprite );
	void						( *PlaySoundByNameAtLocation )	( char *szSound, float volume, float *origin );

	unsigned short				( *PrecacheEvent )		( int type, const char* psz );
	void						( *PlaybackEvent )		( int flags, const struct edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	void						( *WeaponAnim )			( int iAnim, int body );
	float						( *RandomFloat )			( float flLow, float flHigh );
	long						( *RandomLong )			( long lLow, long lHigh );
	void						( *HookEvent )			( char *name, void ( *Event )( struct event_args_s *args ) );
	int							(*Con_IsVisible)			();
	const char					*( *GetGameDirectory )	( void );
	struct cvar_s				*( *GetCvarPointer )		( const char *szName );
	const char					*( *Key_LookupBinding )		( const char *pBinding );
	const char					*( *GetLevelName )		( void );
	void						( *GetScreenFade )		( struct screenfade_s *fade );
	void						( *SetScreenFade )		( struct screenfade_s *fade );
	void                        *( *VGui_GetPanel )         ( );
	void                         ( *VGui_ViewportPaintBackground ) (int extents[4]);

	byte*						(*COM_LoadFile)				( char *path, int usehunk, int *pLength );
	char*						(*COM_ParseFile)			( char *data, char *token );
	void						(*COM_FreeFile)				( void *buffer );

	struct triangleapi_s		*pTriAPI;
	struct efx_api_s			*pEfxAPI;
	struct event_api_s			*pEventAPI;
	struct demo_api_s			*pDemoAPI;
	struct net_api_s			*pNetAPI;
	struct IVoiceTweak_s		*pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	int							( *IsSpectateOnly ) ( void );
	struct model_s				*( *LoadMapSprite )			( const char *filename );

	// file search functions
	void						( *COM_AddAppDirectoryToSearchPath ) ( const char *pszBaseDir, const char *appName );
	int							( *COM_ExpandFilename)				 ( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char					*( *PlayerInfo_ValueForKey )( int playerNum, const char *key );
	void						( *PlayerInfo_SetValueForKey )( const char *key, const char *value );

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	int					(*GetPlayerUniqueID)(int iPlayer, char playerID[16]);

	// TrackerID access
	int							(*GetTrackerIDForPlayer)(int playerSlot);
	int							(*GetPlayerForTrackerID)(int trackerID);

	// Same as ServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	int							( *ServerCmdUnreliable )( char *szCmdString );

	void						( *GetMousePos )( struct tagPOINT *ppt );
	void						( *SetMousePos )( int x, int y );
	void						( *SetMouseEnable )( int fEnable );
} SClientEngineFunction;


#define CLDLL_INTERFACE_VERSION		7


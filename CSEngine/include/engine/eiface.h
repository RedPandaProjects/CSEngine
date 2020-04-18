/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
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
#pragma once

#ifdef HLDEMO_BUILD
#define INTERFACE_VERSION       001
#else  // !HLDEMO_BUILD, i.e., regular version of HL
#define INTERFACE_VERSION		140
#endif // !HLDEMO_BUILD

//
// Defines entity interface between engine and DLLs.
// This header file included by engine files and DLL files.
//
// Before including this header, DLLs must:
//		include progdefs.h
// This is conveniently done for them in extdll.h
//

/*
#ifdef _WIN32
#define DLLEXPORT __stdcall EXT_FUNC
#else
#define DLLEXPORT  __attribute__ ((visibility("default"))) EXT_FUNC
#endif
*/

enum ALERT_TYPE
{
	at_notice,
	at_console,		// same as at_notice, but forces a ConPrintf, not a message box
	at_aiconsole,	// same as at_console, but only shown if developer level is 2!
	at_warning,
	at_error,
	at_logged		// Server print to console ( only in multiplayer games ).
};

// 4-22-98  JOHN: added for use in ClientPrintf
enum PRINT_TYPE
{
	print_console,
	print_center,
	print_chat,
};

// For integrity checking of content on clients
enum FORCE_TYPE
{
	force_exactfile,					// File on client must exactly match server's file
	force_model_samebounds,				// For model files only, the geometry must fit in the same bbox
	force_model_specifybounds,			// For model files only, the geometry must fit in the specified bbox
	force_model_specifybounds_if_avail,	// For Steam model files only, the geometry must fit in the specified bbox (if the file is available)
};

// Returned by TraceLine
struct TraceResult
{
	int		fAllSolid;			// if true, plane is not valid
	int		fStartSolid;		// if true, the initial point was in a solid area
	int		fInOpen;
	int		fInWater;
	float	flFraction;			// time completed, 1.0 = didn't hit anything
	vec3_t	vecEndPos;			// final position
	float	flPlaneDist;
	vec3_t	vecPlaneNormal;		// surface normal at impact
	edict_t	*pHit;				// entity the surface is on
	int		iHitgroup;			// 0 == generic, non zero is specific body part
};

// CD audio status
typedef struct
{
	int	fPlaying;// is sound playing right now?
	int	fWasPlaying;// if not, CD is paused if WasPlaying is true.
	int	fInitialized;
	int	fEnabled;
	int	fPlayLooping;
	float	cdvolume;
	//BYTE 	remap[100];
	int	fCDRom;
	int	fPlayTrack;
} CDStatus;

//#include "../common/crc.h"


// Engine hands this to DLLs for functionality callbacks
typedef struct SServerEngineFunction
{
	int			(*PrecacheModel)			(const char* s);
	int			(*PrecacheSound)			(const char* s);
	void		(*SetModel)				(edict_t *e, const char *m);
	int			(*ModelIndex)			(const char *m);
	int			(*ModelFrames)			(int modelIndex);
	void		(*SetSize)				(edict_t *e, const float *rgflMin, const float *rgflMax);
	void		(*ChangeLevel)			(const char* s1, const char* s2);
	void		(*GetSpawnParms)			(edict_t *ent);
	void		(*SaveSpawnParms)		(edict_t *ent);
	float		(*VecToYaw)				(const float *rgflVector);
	void		(*VecToAngles)			(const float *rgflVectorIn, float *rgflVectorOut);
	void		(*MoveToOrigin)			(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
	void		(*ChangeYaw)				(edict_t* ent);
	void		(*ChangePitch)			(edict_t* ent);
	edict_t*	(*FindEntityByString)	(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
	int			(*GetEntityIllum)		(edict_t* pEnt);
	edict_t*	(*FindEntityInSphere)	(edict_t *pEdictStartSearchAfter, const float *org, float rad);
	edict_t*	(*FindClientInPVS)		(edict_t *pEdict);
	edict_t* (*EntitiesInPVS)			(edict_t *pplayer);
	void		(*MakeVectors)			(const float *rgflVector);
	void		(*AngleVectors)			(const float *rgflVector, float *forward, float *right, float *up);
	edict_t*	(*CreateEntity)			(void);
	void		(*RemoveEntity)			(edict_t* e);
	edict_t*	(*CreateNamedEntity)		(int className);
	void		(*MakeStatic)			(edict_t *ent);
	int			(*EntIsOnFloor)			(edict_t *e);
	int			(*DropToFloor)			(edict_t* e);
	int			(*WalkMove)				(edict_t *ent, float yaw, float dist, int iMode);
	void		(*SetOrigin)				(edict_t *e, const float *rgflOrigin);
	void		(*EmitSound)				(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
	void		(*EmitAmbientSound)		(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
	void		(*TraceLine)				(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	void		(*TraceToss)				(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr);
	int			(*TraceMonsterHull)		(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	void		(*TraceHull)				(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
	void		(*TraceModel)			(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
	const char *(*TraceTexture)			(edict_t *pTextureEntity, const float *v1, const float *v2 );
	void		(*TraceSphere)			(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
	void		(*GetAimVector)			(edict_t* ent, float speed, float *rgflReturn);
	void		(*ServerCommand)			(const char* str);
	void		(*ServerExecute)			(void);
	void		(*ClientCommand)			(edict_t* pEdict, const char* szFmt, ...);
	void		(*ParticleEffect)		(const float *org, const float *dir, float color, float count);
	void		(*LightStyle)			(int style, const char* val);
	int			(*DecalIndex)			(const char *name);
	int			(*PointContents)			(const float *rgflVector);
	void		(*MessageBegin)			(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	void		(*MessageEnd)			(void);
	void		(*WriteByte)				(int iValue);
	void		(*WriteChar)				(int iValue);
	void		(*WriteShort)			(int iValue);
	void		(*WriteLong)				(int iValue);
	void		(*WriteAngle)			(float flValue);
	void		(*WriteCoord)			(float flValue);
	void		(*WriteString)			(const char *sz);
	void		(*WriteEntity)			(int iValue);
	void		(*CVarRegister)			(cvar_t *pCvar);
	float		(*CVarGetFloat)			(const char *szVarName);
	 char*	(*CVarGetString)			(const char *szVarName);
	void		(*CVarSetFloat)			(const char *szVarName, float flValue);
	void		(*CVarSetString)			(const char *szVarName, const char *szValue);
	void		(*AlertMessage)			(ALERT_TYPE atype, const char *szFmt, ...);
	void		(*EngineFprintf)			(void *pfile, const char *szFmt, ...);
	void*		(*PvAllocEntPrivateData)	(edict_t *pEdict, int32 cb);
	void*		(*PvEntPrivateData)		(edict_t *pEdict);
	void		(*FreeEntPrivateData)	(edict_t *pEdict);
	const char*	(*SzFromIndex)			(int iString);
	int			(*AllocString)			(const char *szValue);
	struct entvars_s*	(*GetVarsOfEnt)			(edict_t *pEdict);
	edict_t*	(*PEntityOfEntOffset)	(int iEntOffset);
	int			(*EntOffsetOfPEntity)	(const edict_t *pEdict);
	int			(*IndexOfEdict)			(const edict_t *pEdict);
	edict_t*	(*PEntityOfEntIndex)		(int iEntIndex);
	edict_t*	(*FindEntityByVars)		(struct entvars_s* pvars);
	void*		(*GetModelPtr)			(edict_t* pEdict);
	int			(*RegUserMsg)			(const char *pszName, int iSize);
	void		(*AnimationAutomove)		(const edict_t* pEdict, float flTime);
	void		(*GetBonePosition)		(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles );
	uint32 (*FunctionFromName)	( const char *pName );
	const char *(*NameForFunction)		( uint32 function );
	void		(*ClientPrintf)			( edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg ); // JOHN: engine callbacks so game DLL can print messages to individual clients
	void		(*ServerPrint)			( const char *szMsg );
	const char *(*Cmd_Args)				( void );		// these 3 added
	const char *(*Cmd_Argv)				( int argc );	// so game DLL can easily
	int			(*Cmd_Argc)				( void );		// access client 'cmd' strings
	void		(*GetAttachment)			(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );
	void		(*CRC32_Init)			(CRC32_t *pulCRC);
	void        (*CRC32_ProcessBuffer)   (CRC32_t *pulCRC, void *p, int len);
	void		(*CRC32_ProcessByte)     (CRC32_t *pulCRC, unsigned char ch);
	CRC32_t		(*CRC32_Final)			(CRC32_t pulCRC);
	int32		(*RandomLong)			(int32  lLow,  int32  lHigh);
	float		(*RandomFloat)			(float flLow, float flHigh);
	void		(*SetView)				(const edict_t *pClient, const edict_t *pViewent );
	float		(*Time)					( void );
	void		(*CrosshairAngle)		(const edict_t *pClient, float pitch, float yaw);
	byte *      (*LoadFileForMe)         (const char *filename, int *pLength);
	void        (*FreeFile)              (void *buffer);
	void        (*EndSection)            (const char *pszSectionName); // trigger_endsection
	int 		(*CompareFileTime)       (char *filename1, char *filename2, int *iCompare);
	void        (*GetGameDir)            (char *szGetGameDir);
	void		(*RegisterVariable) (cvar_t *variable);
	void        (*FadeClientVolume)      (const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	void        (*SetClientMaxspeed)     (edict_t *pEdict, float fNewMaxspeed);
	edict_t *	(*CreateFakeClient)		(const char *netname);	// returns NULL if fake client can't be created
	void		(*RunPlayerMove)			(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec );
	int			(*NumberOfEntities)		(void);
	char*		(*GetInfoKeyBuffer)		(edict_t *e);	// passing in NULL gets the serverinfo
	char*		(*InfoKeyValue)			(char *infobuffer, const char *key);
	void		(*SetKeyValue)			(char *infobuffer, const char *key, const char *value);
	void		(*SetClientKeyValue)		(int clientIndex, char *infobuffer, const char *key, const char *value);
	int			(*IsMapValid)			(const char *filename);
	void		(*StaticDecal)			( const float *origin, int decalIndex, int entityIndex, int modelIndex );
	int			(*PrecacheGeneric)		(const char* s);
	int			(*GetPlayerUserId)		(edict_t *e ); // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	void		(*BuildSoundMsg)			(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	int			(*IsDedicatedServer)		(void);// is this a dedicated server?
	cvar_t		*(*CVarGetPointer)		(const char *szVarName);
	unsigned int (*GetPlayerWONId)		(edict_t *e); // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

	// YWB 8/1/99 TFF Physics additions
	void		(*Info_RemoveKey)		( char *s, const char *key );
	const char *(*GetPhysicsKeyValue)	( const edict_t *pClient, const char *key );
	void		(*SetPhysicsKeyValue)	( const edict_t *pClient, const char *key, const char *value );
	const char *(*GetPhysicsInfoString)	( const edict_t *pClient );
	unsigned short (*PrecacheEvent)		( int type, const char*psz );
	void		(*PlaybackEvent)			( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );

	unsigned char *(*SetFatPVS)			( float *org );
	unsigned char *(*SetFatPAS)			( float *org );

	int			(*CheckVisibility )		( edict_t *entity, unsigned char *pset );

	void		(*DeltaSetField)			( struct delta_s *pFields, const char *fieldname );
	void		(*DeltaUnsetField)		( struct delta_s *pFields, const char *fieldname );
	void		(*DeltaAddEncoder)		( const char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
	int			(*GetCurrentPlayer)		( void );
	int			(*CanSkipPlayer)			( const edict_t *player );
	int			(*DeltaFindField)		( struct delta_s *pFields, const char *fieldname );
	void		(*DeltaSetFieldByIndex)	( struct delta_s *pFields, int fieldNumber );
	void		(*DeltaUnsetFieldByIndex)( struct delta_s *pFields, int fieldNumber );

	void		(*SetGroupMask)			( int mask, int op );

	int			(*CreateInstancedBaseline) ( int classname, struct entity_state_s *baseline );
	void		(*Cvar_DirectSet)		( struct cvar_s *var, const char *value );

	// Forces the client and server to be running with the same version of the specified file
	//  ( e.g., a player model ).
	// Calling this has no effect in single player
	void		(*ForceUnmodified)		( FORCE_TYPE type, float *mins, float *maxs, const char *filename );

	void		(*GetPlayerStats)		( const edict_t *pClient, int *ping, int *packet_loss );

	void		(*AddServerCommand)		( const char *cmd_name, void (*function) (void) );

	// For voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	qboolean	(*Voice_GetClientListening)(int iReceiver, int iSender);
	qboolean	(*Voice_SetClientListening)(int iReceiver, int iSender, qboolean bListen);

	const char *(*GetPlayerAuthId)		( edict_t *e );

	// PSV: Added for CZ training map
//	const char *(*KeyNameForBinding)					( const char* pBinding );

	sequenceEntry_s*	(*SequenceGet)				( const char* fileName, const char* entryName );
	sentenceEntry_s*	(*SequencePickSentence)		( const char* groupName, int pickMethod, int *picked );

	// LH: Give access to filesize via filesystem
	int			(*GetFileSize)						( const char *filename );

	unsigned int (*GetApproxWavePlayLen)				(const char *filepath);
	// MDC: Added for CZ career-mode
	int			(*IsCareerMatch)						( void );

	// BGC: return the number of characters of the localized string referenced by using "label"
	int			(*GetLocalizedStringLength)			(const char *label);

	// BGC: added to facilitate persistent storage of tutor message decay values for
	// different career game profiles.  Also needs to persist regardless of mp.dll being
	// destroyed and recreated.
	void		(*RegisterTutorMessageShown)			(int mid);
	int			(*GetTimesTutorMessageShown)			(int mid);
	void		(*ProcessTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
	void		(*ConstructTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
	void		(*ResetTutorMessageDecayData)		( void );

	// Added 2005/08/11 (no SDK update):
	void(*QueryClientCvarValue)		(const edict_t *player, const char *cvarName);

	// Added 2005/11/21 (no SDK update):
	void(*QueryClientCvarValue2)		(const edict_t *player, const char *cvarName, int requestID);

	// Added 2009/06/19 (no SDK update):
	int(*EngCheckParm)				(const char *pchCmdLineToken, char **ppnext);
} SServerEngineFunction;


// ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT.  INTERFACE VERSION IS FROZEN AT 138

// Passed to KeyValue
typedef struct KeyValueData_s
{
	char		*szClassName;	// in: entity classname
	char		*szKeyName;		// in: name of key
	char		*szValue;		// in: value of key
	qboolean	fHandled;		// out: DLL sets to true if key-value pair was understood
} KeyValueData;


typedef struct
{
	char		mapName[ 32 ];
	char		landmarkName[ 32 ];
	edict_t		*pentLandmark;
	vec3_t		vecLandmarkOrigin;
} LEVELLIST;
#define MAX_LEVEL_CONNECTIONS	16		// These are encoded in the lower 16bits of ENTITYTABLE->flags

typedef struct
{
	int			id;				// Ordinal ID of this entity (used for entity <--> pointer conversions)
	edict_t	*pent;			// Pointer to the in-game entity

	int			location;		// Offset from the base data of this entity
	int			size;			// Byte size of this entity's data
	int			flags;			// This could be a short -- bit mask of transitions that this entity is in the PVS of
	string_t	classname;		// entity class name

} ENTITYTABLE;

#define FENTTABLE_PLAYER		0x80000000
#define FENTTABLE_REMOVED		0x40000000
#define FENTTABLE_MOVEABLE		0x20000000
#define FENTTABLE_GLOBAL		0x10000000

typedef struct saverestore_s SAVERESTOREDATA;

#ifdef _WIN32
typedef
#endif
struct saverestore_s
{
	char		*pBaseData;		// Start of all entity save data
	char		*pCurrentData;	// Current buffer pointer for sequential access
	int			size;			// Current data size
	int			bufferSize;		// Total space for data
	int			tokenSize;		// Size of the linear list of tokens
	int			tokenCount;		// Number of elements in the pTokens table
	char		**pTokens;		// Hash table of entity strings (sparse)
	int			currentIndex;	// Holds a global entity table ID
	int			tableCount;		// Number of elements in the entity table
	int			connectionCount;// Number of elements in the levelList[]
	ENTITYTABLE	*pTable;		// Array of ENTITYTABLE elements (1 for each entity)
	LEVELLIST	levelList[ MAX_LEVEL_CONNECTIONS ];		// List of connections from this level

	// smooth transition
	int			fUseLandmark;
	char		szLandmarkName[20];// landmark we'll spawn near in next level
	vec3_t		vecLandmarkOffset;// for landmark transitions
	float		time;
	char		szCurrentMapName[32];	// To check global entities

}
#ifdef _WIN32
SAVERESTOREDATA
#endif
;

typedef enum _fieldtypes
{
	FIELD_FLOAT = 0,		// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_ENTITY,			// An entity offset (EOFFSET)
	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EVARS,			// EVARS *
	FIELD_EDICT,			// edict_t *, or edict_t *  (same thing)
	FIELD_VECTOR,			// Any vector
	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_POINTER,			// Arbitrary data pointer... to be removed, use an array of FIELD_CHARACTER
	FIELD_INTEGER,			// Any integer or enum
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
	FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_TYPECOUNT,		// MUST BE LAST
} FIELDTYPE;

#if !defined(offsetof)  && !defined(GNUC)
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#endif

#define _FIELD(type,name,fieldtype,count,flags)		{ fieldtype, #name, offsetof(type, name), count, flags }
#define DEFINE_FIELD(type,name,fieldtype)			_FIELD(type, name, fieldtype, 1, 0)
#define DEFINE_ARRAY(type,name,fieldtype,count)		_FIELD(type, name, fieldtype, count, 0)
#define DEFINE_ENTITY_FIELD(name,fieldtype)			_FIELD(entvars_t, name, fieldtype, 1, 0 )
#define DEFINE_ENTITY_GLOBAL_FIELD(name,fieldtype)	_FIELD(entvars_t, name, fieldtype, 1, FTYPEDESC_GLOBAL )
#define DEFINE_GLOBAL_FIELD(type,name,fieldtype)	_FIELD(type, name, fieldtype, 1, FTYPEDESC_GLOBAL )


#define FTYPEDESC_GLOBAL			0x0001		// This field is masked for global entity save/restore

typedef struct
{
	FIELDTYPE		fieldType;
	char			*fieldName;
	int				fieldOffset;
	short			fieldSize;
	short			flags;
} TYPEDESCRIPTION;

#ifndef ARRAYSIZE
#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))
#endif

typedef struct
{
	// Initialize/shutdown the game (one-time call after loading of game .dll )
	void			(*GameInit)			( void );
	int				(*Spawn)				( edict_t *pent );
	void			(*Think)				( edict_t *pent );
	void			(*Use)				( edict_t *pentUsed, edict_t *pentOther );
	void			(*Touch)				( edict_t *pentTouched, edict_t *pentOther );
	void			(*Blocked)			( edict_t *pentBlocked, edict_t *pentOther );
	void			(*KeyValue)			( edict_t *pentKeyvalue, KeyValueData *pkvd );
	void			(*Save)				( edict_t *pent, SAVERESTOREDATA *pSaveData );
	int 			(*Restore)			( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
	void			(*SetAbsBox)			( edict_t *pent );

	void			(*SaveWriteFields)	( SAVERESTOREDATA *, const char *, void *, TYPEDESCRIPTION *, int );
	void			(*SaveReadFields)	( SAVERESTOREDATA *, const char *, void *, TYPEDESCRIPTION *, int );

	void			(*SaveGlobalState)		( SAVERESTOREDATA * );
	void			(*RestoreGlobalState)	( SAVERESTOREDATA * );
	void			(*ResetGlobalState)		( void );

	qboolean		(*ClientConnect)		( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );

	void			(*ClientDisconnect)	( edict_t *pEntity );
	void			(*ClientKill)		( edict_t *pEntity );
	void			(*ClientPutInServer)	( edict_t *pEntity );
	void			(*ClientCommand)		( edict_t *pEntity );
	void			(*ClientUserInfoChanged)( edict_t *pEntity, char *infobuffer );

	void			(*ServerActivate)	( edict_t *pEdictList, int edictCount, int clientMax );
	void			(*ServerDeactivate)	( void );

	void			(*PlayerPreThink)	( edict_t *pEntity );
	void			(*PlayerPostThink)	( edict_t *pEntity );

	void			(*StartFrame)		( void );
	void			(*ParmsNewLevel)		( void );
	void			(*ParmsChangeLevel)	( void );

	 // Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
	const char     *(*GetGameDescription)( void );

	// Notify dll about a player customization.
	void            (*PlayerCustomization) ( edict_t *pEntity, customization_t *pCustom );

	// Spectator funcs
	void			(*SpectatorConnect)		( edict_t *pEntity );
	void			(*SpectatorDisconnect)	( edict_t *pEntity );
	void			(*SpectatorThink)		( edict_t *pEntity );

	// Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.
	void			(*Sys_Error)			( const char *error_string );

	void			(*PM_Move) ( struct playermove_s *ppmove, qboolean server );
	void			(*PM_Init) ( struct playermove_s *ppmove );
	char			(*PM_FindTextureType)( char *name );
	void			(*SetupVisibility)( struct edict_s *pViewEntity, struct edict_s *pClient, unsigned char **pvs, unsigned char **pas );
	void			(*UpdateClientData) ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd );
	int				(*AddToFullPack)( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
	void			(*CreateBaseline) ( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs );
	void			(*RegisterEncoders)	( void );
	int				(*GetWeaponData)		( struct edict_s *player, struct weapon_data_s *info );

	void			(*CmdStart)			( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed );
	void			(*CmdEnd)			( const edict_t *player );

	// Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
	//  size of the response_buffer, so you must zero it out if you choose not to respond.
	int				(*ConnectionlessPacket )	( const struct netadr_s *net_from_, const char *args, char *response_buffer, int *response_buffer_size );

	// Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
	int				(*GetHullBounds)	( int hullnumber, float *mins, float *maxs );

	// Create baselines for certain "unplaced" items.
	void			(*CreateInstancedBaselines) ( void );

	// One of the ForceUnmodified files failed the consistency check for the specified player
	// Return 0 to allow the client to continue, 1 to force immediate disconnection ( with an optional disconnect message of up to 256 characters )
	int				(*InconsistentFile)( const struct edict_s *player, const char *filename, char *disconnect_message );

	// The game .dll should return 1 if lag compensation should be allowed ( could also just set
	//  the sv_unlag cvar.
	// Most games right now should return 0, until client-side weapon prediction code is written
	//  and tested for them.
	int				(*AllowLagCompensation)( void );
} SServerFunction;


// Current version.
#define NEW_DLL_FUNCTIONS_VERSION	1

typedef struct
{
	// Called right before the object's memory is freed.
	// Calls its destructor.
	void			(*OnFreeEntPrivateData)(edict_t *pEnt);
	void			(*GameShutdown)(void);
	int				(*ShouldCollide)( edict_t *pentTouched, edict_t *pentOther );
	void			(*CvarValue)( const edict_t *pEnt, const char *value );
    void            (*CvarValue2)( const edict_t *pEnt, int requestID, const char *cvarName, const char *value );
} SServerNewFunction;
typedef int(*FGET_SERVER_NEW_FUNCTIONS)(SServerNewFunction*pFunctionTable, int *interfaceVersion);


typedef int(*FGET_SERVER_FUNCTIONS)(SServerFunction*pFunctionTable, int interfaceVersion);
typedef int(*FGET_SERVER_FUNCTIONS2)(SServerFunction*pFunctionTable, int *interfaceVersion);

typedef void(__cdecl* FSET_SERVERENGINE_FUNCTIONS)(SServerEngineFunction* engfuncs, globalvars_t* pGlobals);
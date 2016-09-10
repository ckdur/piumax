#ifndef __PUMP_INCLUDE__
#define __PUMP_INCLUDE__

//#include "target.h"

#define SatDec(v) if (v) (v)--

#define PROGRAM_NAME "PUMP IT UP: NX2"
// SYSTEM SPECIFIC

#define MAX_STAGE 4

#define ALL_SKINS

enum {
	SCORE_PERFECT = 1000,
	SCORE_GREAT   = 500,
	SCORE_GOOD    = 100,
	SCORE_BAD     = -200,
	SCORE_MISS    = -500,
	SCORE_MISS_LONGNOTE = -300,
	SCORE_NIGHTMARE_BONUS = 500000,
	SCORE_GRADE_S = 100000,
	SCORE_ITEM    = 100 /* for Bad, +100/(1 higher grade) */
};

enum {
	ePerfect = 0, eGreat, eGood, eBad, eMiss, eZoneMax,
	eStepG = eZoneMax, eStepW, eStepA, eStepB, eStepC, eDivTypeMax, eDivLocalTypeMax = eStepA
};

typedef enum {
	eGradeS = 0, eGradeA, eGradeB, eGradeC, eGradeD, eGradeF, eGradeMax
} enumGrade;

extern const float GradeRateTable[eGradeMax];
extern const char GradeCharTable[eGradeMax];

typedef enum {
	ePlayerNone = 0,
	ePlayer1 = 1,
	ePlayer2 = ePlayer1 << 1,
	ePlayerBoth = ePlayer1 | ePlayer2,
} enumActivePlayer;

typedef enum {
	eStepDoubleBit = 8,

	eStepNormal = 0,
	eStepHard,
	eStepDivision,
	eStepCrazy,
	eStepPractice,

	eStepHalfDouble = eStepDoubleBit,
	eStepFreestyle,
	eStepNightmare,
	eStepLightMap,

	eStepTypeInvalid = -1
} enumStepType;

typedef enum {
	eBattleModeMaskGroup = 4,
	eBattleModeGroupNormal = 0,
	eBattleModeGroupItem = 4,

	eBattleModeNone = eBattleModeGroupNormal,
	eBattleModeScore,
	eBattleModeCombo,

	eBattleModeItem = eBattleModeGroupItem,
	eBattleModeMineSweeper,
	eBattleModeHyperVelocity,
	eBattleModeHybridStep,
} enumBattleMode;

typedef enum {
	eSpeedHalf = 2,
	eSpeedNormal = 4,
	eSpeedX2 = 2 * eSpeedNormal,
	eSpeedX3 = 3 * eSpeedNormal,
	eSpeedX4 = 4 * eSpeedNormal,
	eSpeedX5 = 5 * eSpeedNormal, // Pawprint
	eSpeedX6 = 6 * eSpeedNormal, // Pawprint
	eSpeedX8 = 8 * eSpeedNormal,
	eSpeedMask = 0xFF,
	eSpeedRandomVelocity = 0x0100,
	eSpeedEarthworm = 0x0200
} enumCmdSpeed;

// PlayerCmd
#define GAMEMODE_VANISH			0x00001
#define GAMEMODE_APPEAR			0x00002
#define GAMEMODE_NONSTEP		(GAMEMODE_VANISH|GAMEMODE_APPEAR)
#define GAMEMODE_FLASH			0x00004
#define GAMEMODE_FREEDOM		0x00008

#define GAMEMODE_MIRROR			0x00010
#define GAMEMODE_RANDOM			0x00020
#define GAMEMODE_RANDOMSKIN		0x00040
#define GAMEMODE_180			0x00080
#define GAMEMODE_JUDGEREVERSE	0x00100

#define GAMEMODE_DECEL			0x00200
#define GAMEMODE_ACCEL			0x00400
#define GAMEMODE_COSMOS			0x00800

#define GAMEMODE_EXCEED			0x01000
#define GAMEMODE_NX				0x02000
#define GAMEMODE_UPSIDEDOWN		0x04000

#define GAMEMODE_MULTIBANK		0x08000
#define GAMEMODE_GHOSTBUSTER	0x10000
#define GAMEMODE_TREASUREHUNTER	0x20000
#define GAMEMODE_SHOWMETHEBANK	0x40000

#define GAMEMODE_SINK			0x080000
#define GAMEMODE_RISE			0x100000
#define GAMEMODE_SNAKE			0x200000
#define GAMEMODE_HARD			0x400000
#define GAMEMODE_EXPERT			0x800000

//#define GAMEMODE_ACCEL		(GAMEMODE_X2|GAMEMODE_X3|GAMEMODE_X4)

#define ORG_Y		378.0f
#define ORG_1P_X	28.0f
#define ORG_2P_X	348.0f
#define ORG_DB1_X	64.0f
#define ORG_DB2_X	312.0f


enum
{
	CHANNEL_NEWTUNE = 0,
	CHANNEL_KPOP,
	CHANNEL_POP,
	CHANNEL_BANYA,

	CHANNEL_SPECIAL_BIT = 4,
	CHANNEL_FULL = CHANNEL_SPECIAL_BIT,
	CHANNEL_REMIX,
	CHANNEL_ANOTHER,

	CHANNEL_WORLDTOUR_BIT = 8,
	CHANNEL_ASIA = CHANNEL_WORLDTOUR_BIT,
	CHANNEL_NA,
	CHANNEL_SA,
	CHANNEL_EU,
	//CHANNEL_BATTLE,
	
	NUM_CHANNEL,
};

// Pawprint
enum
{
	MODE_NORMAL = 0,
	MODE_HARD,
	MODE_CRAZY,
#if defined (HALF_DOUBLE)
	MODE_HALFDOUBLE,
#endif
	MODE_FREESTYLE,
	MODE_NIGHTMARE,
	NUM_MODES,
#if defined (HALF_DOUBLE)
	MODE_SINGLE_PLAYER_ONLY = MODE_HALFDOUBLE
#else
	MODE_SINGLE_PLAYER_ONLY = MODE_FREESTYLE
#endif
};

#endif

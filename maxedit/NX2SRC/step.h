#ifndef __step_h__
#define __step_h__

//************************************************************************
// step.h
//
// This file comes from a modification enough necesary to
// use in MaxEdit. THIS WILL BE BLOCKED
// CkDur
//************************************************************************

#include "stdafx.h"
#include "pump.h"

#define ANYSIZE_ARRAY 1


typedef unsigned char uint8;
/*
class CTimer;
namespace PEngine { class CTexture; }

extern CTimer g_Timer;
extern PEngine::CTexture g_SongTitle;
*/
namespace Step
{
/*
	Step values obtained from STEPEdit v5.63

	Step:
	Normal		0xB3 (1011 0011)
	Long Start 	0xB4 (1011 0100)
	Long Cont	0xB6 (1011 0110)
	Long End	0xB7 (1011 0111)

	Special:
	G			0x03F2 (0000 0011 1111 0010)
	W			0x07F2 (0000 0111 1111 0010)
	A			0x0BF2 (0000 1011 1111 0010)
	B			0x0FF2 (0000 1111 1111 0010)
	C			0x13F2 (0001 0011 1111 0010)

	Item:
	Action		0x03F1 (0000 0011 1111 0001)
	Shield		0x07F1 (0000 0111 1111 0001)
	Change		0x0BF1 (0000 1011 1111 0001)
	Accel		0x0FF1 (0000 1111 1111 0001)
	Weather		0x13F1 (0001 0011 1111 0001)
	Mine		0x17F1 (0001 0111 1111 0001)
	MineLayer	0x1BF1 (0001 1011 1111 0001)
	Gauge Break	0x1FF1 (0001 1111 1111 0001)
	Drain		0x23F1 (0010 0011 1111 0001)
	Bonus		0x27F1 (0010 0111 1111 0001)
	Velocity	0x2BF1 (0010 1011 1111 0001)
	Random		0x2FF1 (0010 1111 1111 0001)
	Vel 3x		0x33F1 (0011 0011 1111 0001)
	Vel 4x		0x37F1 (0011 0111 1111 0001)
	Vel 8x		0x3BF1 (0011 1011 1111 0001)
	Vel 1x		0x3FF1 (0011 1111 1111 0001)
	Potion		0x43F1 (0100 0011 1111 0001)
	0 Deg		0x47F1 (0100 0111 1111 0001)
	90 Deg		0x4BF1 (0100 1011 1111 0001)
	180 Deg		0x4FF1 (0100 1111 1111 0001)
	-90 Deg		0x53F1 (0101 0011 1111 0001)

	Attr:
	1 Normal		0xB3 (1011 0011)
	2 Appear		0x93 (1001 0011)
	3 Vanish		0xA3 (1010 0011)
	4 Invisible		0x83 (1000 0011)
	5 Bonus			0xF3 (1111 0011)
	6 Bonus Appear	0xD3 (1101 0011)
	7 Hidden Vanish	0xE3 (1110 0011)
	8 Hidden		0xC3 (1100 0011)
	9 Ghost			0x73 (0111 0011)
	A Ghost Appear	0x53 (0101 0011)
	B Ghost Vanish	0x63 (0110 0011)

	Judge: 1234, 5678, 9AB
	Visual: 159, 26A, 37B, 48
	Bank: All
*/

// [v] == verified with disassembled PIUTESTER.EXE (0x6483F3E00)

// Possible values for Note::Attr
const int TypeMask			= 0x07;	// 0000 0111	// [v]
const int VisibilityMask	= 0x30;	// 0011 0000
const int CheckMask			= 0xC0;	// 1100 0000

const int Visible		= 0x30;	// 0011 0000	// [v]
const int Nones			= 0;

const int bNeedCheck	= 128;	// 1000 0000	// This note should be checked?	// [v]
const int bNoMiss		= 64;	// 0100 0000
const int bDrawBegin	= 32;	// 0010 0000
const int bDrawEnd		= 16;	// 0001 0000
const int bChecked		= 8;	// 0000 1000	// Note has already been checked? // [v]
const int bLong			= 4;	// 0000 0100

// Note::Attr & TypeMask
const int Item			= 1;	// 1111 0001	// [v]
const int Special		= 2;	// 1111 0010
const int Normal		= 3;	// 1011 0011	// [v]
const int LongStart		= 4;	// 1011 0100
const int LongMiddle	= 6;	// 1011 0110
const int LongEnd		= 7;	// 1011 0111

// Note::Attr & CheckMask
const int NoJudge		= 64;	// 0100 0000
const int JudgeMiss		= 128;	// 1000 0000

// Note::Id_Bank
const int JudgeBankMask = 3;	// 0000 0011	// [v]
const int nJudgeBank = 4;		// [v]
const int ItemBank = 0;

typedef struct Note
{
	int8_t Attr;
	int8_t Id_Bank;
} Note_t;

// There's some difference between what's saved inside of a .NX step file
// and what's used in the game. _Vars are names I can't verify due to not
// being referenced elsewhere in the game
struct Div
{
	float msStart;			// Total delay (start of block + manual delay)
	float msPerLine;
	float _beatsPerLine;	// Also called "MysteryBox"
	float _beatDelay;		// Block delay as set in StepEdit, in ms
	float Speed;
	int32_t ofsDivInfo;			// !null if "Division Set" info is filled out in the Block Info dialog box, Also called "DivisionConds"
	int8_t nBeatSplit;
	int8_t _pad1;				// MSB of BeatSplit, maybe is little endian [_pad1,nBeatSplit] = BeatSplit
	int8_t _beatsPerMeasure;
	int8_t bmFlags;			// Also called "smoothSpeed", maybe boolean
	int32_t nLine;				// Also called "NumRows"
	int32_t ofsLine[ANYSIZE_ARRAY];
	//int bmLight[MAX_STEP_LINES][3];	// number of lines * RGB
};

// For bmFlags
const int bSmooth = 1;

struct DivInfo
{
	int32_t Min[eDivTypeMax];
	int32_t Max[eDivTypeMax];
};

};

struct Block
{
	int32_t nDiv;
	int32_t ofsDiv[ANYSIZE_ARRAY];
};

struct Step_t
{
	int8_t magic[4];		// "NX10"
	int32_t nNoteOffset;	// 0xA if LM.
	int32_t nNote;			// line width (3 for LM, 5 for single, 10 for double)
	int32_t nBlock;
	int32_t ofsBlock[ANYSIZE_ARRAY];
};

struct Position
{
	Position() : Block(0), Line(0), pDiv(NULL) {};

	int Block;
	int Line;
	Step::Div* pDiv;
};

class CNX10Step
{
public:
	CNX10Step();
	virtual ~CNX10Step();

	bool Load(LPCTSTR name, const bool singleBank = true);
	void Unload();

	Block* GetBlock(const int blockIndex);
	void* GetPtr(int offset);

	// Division
	Step::Div* GetDiv(int blockIndex);					// Get a random division in the block. Randomly chosen in Load()
	Step::Div* GetDiv(int blockIndex, int divIndex);
	void SetDivision(int blockIndex, int divIndex);

	int GetDivIndex(int blockIndex);
	float DivEndTime(Step::Div* div);

	Step::Note* GetLinePtr(int index);

	void Randomize();

	void Change(const Step::Note& a, const Step::Note& b, const Step::Note& c, const Step::Note& d);
	void RandomChange(const Step::Note& a, const Step::Note& b, const Step::Note& c, const Step::Note* d);

	void SetCurrentTime(float time);

	struct SwapPair
	{
		int a;
		int b;
	};
	void Swap(const SwapPair* swapPairArray, int arrayLen);

	// Draw
	Position GetDrawStart(const float inTime);
	void Draw(const Position& pos);
	void DrawMarker(const Position& pos);

	void SetJudgeTiming(const int perfect, const int interval, const int judgeRegions, const int delay);
	void Judge(const int input, const int holds);

protected:
	virtual bool DrawLine(float BlockBeat, float BeatInBlock, Step::Note *pLine); // return false for 'DrawEnd'
	virtual bool DrawMarkerLine(float BlockBeat, int BeatInBlock, bool bMeasure);
	virtual bool Virtual_C(int Bank, int v16, Step::Note* pLine, int something);
	virtual bool JudgeItem(int index, uint8 Id, int Grade);
	virtual bool JudgeSpecial(int index, uint8 Id, int Grade);
	virtual bool JudgeLine(int Bank, Step::Note *pLine, int Grade);

	int GetBlockIndex(float time, int oldBlockIndex);
	int GetJudgeRegion(float a2);

	// Variables
	// Ordered according to disassembly (for no reason in particular)
public:
	Step_t* m_pStep;

	struct JudgeTiming
	{
		float Perfect;
		float Interval;
		float Delay;
		float Miss;			// time past perfect that will cause a miss
		float m_MaxJudge;	// largest distance between user press and perfect timing that will trigger judgement
		int m_JudgeRegions;
	} m_Timing;

public:
	Position m_CurPos;
	Position m_PrevPos;
	Position m_ProcessingPos;
	Position m_JudgeStartPos;
	Position m_FooPos;

protected:
	int* m_pRandDiv;	// Normally 0s. Set to a random number if division info exists.
	float m_msCurTime;	// Time since the start of the step file
	float m_CurBeat;	// Current beat in the song
	bool m_bSingleBank;

	int m_FooArray[256][6];
	int m_NumFoos;		// Index into m_FooArray
};

#endif // __step_h__

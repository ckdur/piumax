//************************************************************************
// step.cpp
//
// Reconstructed by ahnada
//************************************************************************

#include "step.h"
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cmath>

#define USE_DECOMPILED_CODE 1

CNX10Step::CNX10Step() :
	m_pStep(NULL), m_pRandDiv(NULL)
{
	// Empty
}

CNX10Step::~CNX10Step()
{
	Unload();
}

// Decompiled
bool CNX10Step::Load(LPCTSTR name, const bool singleBank)
{
	FILE* in = _tfopen(name, "rb");
	if(!in)
	{
		return false;
	}

	fseek(in, 0, SEEK_END);
	int filesize = ftell(in);
	m_pStep = (Step_t*) malloc(filesize);
	if(!m_pStep)
	{
		fclose(in);
		Unload();
		return false;
	}

	fseek(in, 0, SEEK_SET);
	if(fread(m_pStep, 1, filesize, in) != filesize)
	{
		fclose(in);
		Unload();
		return false;
	}

	m_pRandDiv = (int*)calloc(m_pStep->nBlock * sizeof(int), 1);
	if(!m_pRandDiv)
	{
		fclose(in);
		Unload();
		return false;
	}

	for(int i = 0; i < m_pStep->nBlock; i++)
	{
		Block* curBlock = GetBlock(i);

		// Setup RandDiv
		// Only the first division of a block has the pointer to DivInfo
		if(GetDiv(i, 0)->ofsDivInfo)
		{
			Step::DivInfo* divInfo = (Step::DivInfo*)GetPtr(GetDiv(i, 0)->ofsDivInfo);
			// Why only check Min and Max of the first entry?
			if(divInfo->Min[0] == 1 && divInfo->Max[0] != 0)
			{
				m_pRandDiv[i] = rand() / 16 % curBlock->nDiv;
			}
		}

		// IMPORTANT: We're interested on original information
		// Next lines only converts BPM into msPerLine
		// and Delay in ms into Delay in Beats <-- this conversion only happens
		// if div->Speed is positive or 0

		/*for(int j = 0; j < curBlock->nDiv; j++)
		{
			Step::Div* div = GetDiv(i, j);
			if(div->msPerLine != 0.0f)
			{
				// Convert the input BPM field into an msPerLine field
				// (ms / min) / (line / beat * beat / min) ==> ms / line
				div->msPerLine = 60000.0f / (div->nBeatSplit * div->msPerLine);
			}

			if(0.0f <= div->Speed)
			{
				// Convert delay (ms) from step file into delay in beats
				// ms / (line / beat * ms / line) ==> beat
				div->_beatDelay /= div->nBeatSplit * div->msPerLine;
			}
			else
			{
				div->_beatDelay = 0.0f;
				div->Speed *= -1.0f;
			}
		}*/
	}

	fclose(in);

	m_msCurTime = 0.0f;

	m_CurPos.Block = m_PrevPos.Block = m_JudgeStartPos.Block = m_FooPos.Block = 0;
	m_CurPos.Line = m_PrevPos.Line = m_JudgeStartPos.Line = m_FooPos.Line = 0;
	m_CurPos.pDiv = m_PrevPos.pDiv = m_JudgeStartPos.pDiv = m_FooPos.pDiv = NULL;

	m_NumFoos = 0;
	m_bSingleBank = singleBank;

	return true;
}

// Decompiled
void CNX10Step::Unload()
{
	if(m_pStep)
	{
		free(m_pStep);
		m_pStep = NULL;
	}

	if(m_pRandDiv)
	{
		free(m_pRandDiv);
		m_pRandDiv = NULL;
	}
}

Block* CNX10Step::GetBlock(const int blockIndex)
{
	assert(blockIndex < m_pStep->nBlock);
	return (Block*)GetPtr(m_pStep->ofsBlock[blockIndex]);
}

void* CNX10Step::GetPtr(int offset)
{
	return (char*)m_pStep + offset;
}

Step::Div* CNX10Step::GetDiv(int blockIndex)
{
	assert(blockIndex < m_pStep->nBlock);

	return (Step::Div*)GetPtr(GetBlock(blockIndex)->ofsDiv[m_pRandDiv[blockIndex]]);
}

Step::Div* CNX10Step::GetDiv(int blockIndex, int divIndex)
{
	assert(blockIndex < m_pStep->nBlock);
	assert(divIndex < GetBlock(blockIndex)->nDiv);

	return (Step::Div*)GetPtr(GetBlock(blockIndex)->ofsDiv[divIndex]);
}

// Decompiled
void CNX10Step::SetDivision(int blockIndex, int divIndex)
{
	if(GetDivIndex(blockIndex) != divIndex)
	{
		m_pRandDiv[blockIndex] = blockIndex;
		if(m_JudgeStartPos.Block < blockIndex)
		{
			if(blockIndex <= m_FooPos.Block)
			{
				m_FooPos.Block = blockIndex;
				m_FooPos.Line = 0;
			}
		}
		else
		{
			m_FooPos = m_JudgeStartPos;
		}
	}
}

int CNX10Step::GetDivIndex(int blockIndex)
{
	return m_pRandDiv[blockIndex];
}

float CNX10Step::DivEndTime(Step::Div* div)
{
	return div->msStart + div->msPerLine * div->nLine;
}

Step::Note* CNX10Step::GetLinePtr(int index)
{
	if(index == 0)
	{
		return NULL;
	}

	return (Step::Note*) GetPtr(index);
}

void CNX10Step::Randomize()
{

}

void CNX10Step::Change(const Step::Note& a, const Step::Note& b, const Step::Note& c, const Step::Note& d)
{

}

void CNX10Step::RandomChange(const Step::Note& a, const Step::Note& b, const Step::Note& c, const Step::Note* d)
{

}

// Decompiled
int CNX10Step::GetBlockIndex(float time, int startBlockIndex)
{
	int outBlock = startBlockIndex;
	if(m_pStep->nBlock <= outBlock)
	{
		outBlock = 0;
	}

	float divEndTime = DivEndTime(GetDiv(outBlock));
	if(DivEndTime(GetDiv(outBlock)) <= time)
	{
		outBlock++;
		for(;outBlock < m_pStep->nBlock; outBlock++)
		{
			divEndTime = DivEndTime(GetDiv(outBlock));
			if(time < divEndTime)
			{
				break;
			}
		}

		if(m_pStep->nBlock <= outBlock)
		{
			outBlock--;
		}
	}
	else
	{
		for(;0 < outBlock; outBlock--)
		{
			divEndTime = DivEndTime(GetDiv(outBlock - 1));
			if(divEndTime <= time)
			{
				break;
			}
		}
	}

	return outBlock;
}

// Decompiled
int GetLineIndex(float time, Step::Div* pDiv)
{
	int outLine = 0;

	float divTime = time - pDiv->msStart;
	if((0.0f <= divTime) && (pDiv->msPerLine != 0.0f))
	{
		outLine = (int) ( divTime / pDiv->msPerLine );
	}

	return outLine;
}

// Decompiled
void CNX10Step::SetCurrentTime(float time)
{
	if(time < m_msCurTime)
	{
		time = m_msCurTime;
	}
	else
	{
		m_msCurTime = time;
	}

	m_PrevPos = m_CurPos;
	int newBlockIndex = GetBlockIndex(time, m_CurPos.Block);

	m_CurPos.Block = newBlockIndex;
	m_CurPos.pDiv = GetDiv(newBlockIndex);
	m_CurPos.Line = GetLineIndex(time, m_CurPos.pDiv);
}

void CNX10Step::Swap(const SwapPair* swapPairArray, int arrayLen)
{

}

// Decompiled
Position CNX10Step::GetDrawStart(const float inTime)
{
	int outBlock = m_CurPos.Block;
	int outLine = m_CurPos.Line;
	float divTime = m_msCurTime - m_CurPos.pDiv->msStart;			// Time from the start of the division
	m_CurBeat = m_CurPos.Line * m_CurPos.pDiv->_beatsPerLine;

	if(m_CurPos.pDiv->msPerLine != 0.0f)
	{
		if(divTime < 0.0f)
		{
			if(m_CurPos.pDiv->_beatDelay != 0.0f)
			{
				m_CurBeat -= divTime / (m_CurPos.pDiv->nBeatSplit * m_CurPos.pDiv->msPerLine);
			}
		}
		else
		{
			m_CurBeat -= divTime / m_CurPos.pDiv->msPerLine * m_CurPos.pDiv->_beatsPerLine;
		}
	}

	float v12 = inTime + m_CurBeat;
	if(v12 <= 0.0f)
	{
		Position outPos;
		outPos.Block = outBlock;
		outPos.Line = 0 < outLine ? outLine - 1 : outLine;
		outPos.pDiv = m_CurPos.pDiv;
		return outPos;
	}
	else
	{
		Step::Div* pDiv = m_CurPos.pDiv;
		while(1)
		{
			float curBeat = outLine * pDiv->_beatsPerLine;
			if(v12 < curBeat)
			{
				Position outPos;
				outPos.Block = outBlock;
				outPos.Line = outLine - (int)( v12 / pDiv->_beatsPerLine );
				outPos.pDiv = m_CurPos.pDiv;
				return outPos;
			}

			if(!outBlock)
			{
				break;
			}
			outBlock--;
			v12 -= curBeat + pDiv->_beatDelay;
			pDiv = GetDiv(outBlock);
			outLine = pDiv->nLine;
			if(v12 <= 0.0f)
			{
				Position outPos;
				outPos.Block = outBlock;
				outPos.Line = 0 < outLine ? outLine - 1 : outLine;
				outPos.pDiv = m_CurPos.pDiv;
				return outPos;
			}
		}
		Position outPos;
		outPos.pDiv = m_CurPos.pDiv;
		return outPos;
	}

	// Failsafe
	return m_CurPos;
}

// Decompiled
void CNX10Step::Draw(const Position& pos)
{
#if USE_DECOMPILED_CODE
	m_ProcessingPos = m_CurPos;
	float curBeat = m_CurBeat;
	m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);

	if(pos.Block < m_ProcessingPos.Block)
	{
		do
		{
			m_ProcessingPos.Block--;
			curBeat -= m_ProcessingPos.Line * m_ProcessingPos.pDiv->_beatsPerLine + m_ProcessingPos.pDiv->_beatDelay;
			m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
			m_ProcessingPos.Line = m_ProcessingPos.pDiv->nLine;
		} while(pos.Block < m_ProcessingPos.Block);
	}

	float blockBeat = curBeat - m_ProcessingPos.Line * m_ProcessingPos.pDiv->_beatsPerLine;
	m_ProcessingPos.Line = pos.Line;

	while(m_ProcessingPos.pDiv->nLine <= m_ProcessingPos.Line)
	{
LABEL_Draw_8:
		m_ProcessingPos.Block++;
		if(m_pStep->nBlock <= m_ProcessingPos.Block)
		{
			return;
		}
		float v20 = m_ProcessingPos.pDiv->nLine * m_ProcessingPos.pDiv->_beatsPerLine;
		m_ProcessingPos.Line = 0;
		m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
		blockBeat += v20 + m_ProcessingPos.pDiv->_beatDelay;
	}

	while(1)
	{
		Step::Note* pLine = GetLinePtr(m_ProcessingPos.pDiv->ofsLine[m_ProcessingPos.Line]);
		if(pLine)
		{
			float beatInBlock = m_ProcessingPos.Line * m_ProcessingPos.pDiv->_beatsPerLine;
			if(!DrawLine(blockBeat, beatInBlock, pLine))
			{
				return;
			}
		}
		m_ProcessingPos.Line++;
		if(m_ProcessingPos.pDiv->nLine <= m_ProcessingPos.Line)
		{
			// goto previous while loop
			goto LABEL_Draw_8;
		}
	}

#else
	for(int b = pos.Block; b < m_pStep->nBlock; b++)
	{
		Step::Div* div = (b == pos.Block) ? pos.pDiv : GetDiv(b);
		for(int l = (b == pos.Block) ? pos.Line : 0; l < div->nLine; l++)
		{
			Step::Note* pLine = GetLinePtr(div->ofsLine[l]);
			if(!pLine)
			{
				continue;
			}

			float beatPerMS = div->_beatsPerLine / div->msPerLine;
			float divTime = div->msStart - m_msCurTime + div->msPerLine * l;

			// + number = down the screen
			// as time passes, number should get more negative
			int beat = l * div->_beatsPerLine;
			if(!DrawLine(divTime * beatPerMS - beat, beat, pLine))
			{
				return;
			}
		}
	}
#endif // #if USE_DECOMPILED_CODE
}

// Decompiled
void CNX10Step::DrawMarker(const Position& pos)
{
#if USE_DECOMPILED_CODE
	m_ProcessingPos = m_CurPos;
	// Do we need this? m_CurPos should have the correct pDiv...
	m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
	float curBeat = m_CurBeat;

	if(pos.Block < m_ProcessingPos.Block)
	{
		do
		{
			m_ProcessingPos.Block--;
			curBeat -= m_ProcessingPos.Line * m_ProcessingPos.pDiv->_beatsPerLine + m_ProcessingPos.pDiv->_beatDelay;
			m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
			m_ProcessingPos.Line = m_ProcessingPos.pDiv->nLine;
		} while(pos.Block < m_ProcessingPos.Block);
	}

	float blockBeat = curBeat - m_ProcessingPos.Line * m_ProcessingPos.pDiv->_beatsPerLine;
	int beatInBlock = (pos.Line + m_ProcessingPos.pDiv->nBeatSplit - 1) / m_ProcessingPos.pDiv->nBeatSplit;
	m_ProcessingPos.Line = beatInBlock * m_ProcessingPos.pDiv->nBeatSplit;

	while(m_ProcessingPos.pDiv->nLine <= m_ProcessingPos.Line)
	{
LABEL_DrawMarker_7:
		m_ProcessingPos.Block++;
		if(m_pStep->nBlock <= m_ProcessingPos.Block)
		{
			return;
		}
		beatInBlock = 0;
		float v19 = m_ProcessingPos.pDiv->nLine * m_ProcessingPos.pDiv->_beatsPerLine;
		m_ProcessingPos.Line = 0;
		m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
		blockBeat += v19 + m_ProcessingPos.pDiv->_beatDelay;
	}

	while(true)
	{
		bool bMeasure = (beatInBlock % m_ProcessingPos.pDiv->_beatsPerMeasure) == 0;
		if(!DrawMarkerLine(blockBeat, beatInBlock, bMeasure))
		{
			return;
		}
		m_ProcessingPos.Line += m_ProcessingPos.pDiv->nBeatSplit;
		beatInBlock++;
		if(m_ProcessingPos.pDiv->nLine <= m_ProcessingPos.Line)
		{
			goto LABEL_DrawMarker_7;
		}
	}
#else
	for(int b = pos.Block; b < m_pStep->nBlock; b++)
	{
		Step::Div* div = (b == pos.Block) ? pos.pDiv : GetDiv(b);
		for(int l = (b == pos.Block) ? pos.Line : 0; l < div->nLine; l++)
		{
			// Draw a line every measure
			if(l % (div->nBeatSplit * div->_beatsPerMeasure) != 0)
			{
				continue;
			}

			m_ProcessingPos.Block = b;
			m_ProcessingPos.Line = l;
			m_ProcessingPos.pDiv = div;

			float beatPerMS = div->_beatsPerLine / div->msPerLine;
			float divTime = div->msStart - m_msCurTime;

			// + number = down the screen
			// as time passes, number should get more negative
			if(!DrawMarkerLine(divTime * beatPerMS, l * div->_beatsPerLine, true))
			{
				return;
			}
		}
	}
#endif // #if USE_DECOMPILED_CODE
}

// Decompiled
void CNX10Step::SetJudgeTiming(const int perfect, const int interval, const int judgeRegions, const int delay)
{
	m_Timing.Perfect = (float) perfect;
	m_Timing.Interval = (float) interval;
	m_Timing.m_JudgeRegions = judgeRegions;
	m_Timing.Delay = (float) delay;
	m_Timing.m_MaxJudge = (float) (perfect + (judgeRegions - 1) * interval);
	m_Timing.Miss = -(float)delay - m_Timing.m_MaxJudge;

#if 0
	// ridiculous timing for testing purposes
	m_Timing.Perfect = 1000;
	m_Timing.Interval = 1000;
	m_Timing.m_JudgeRegions = 4;
	m_Timing.Delay = 1000;
	m_Timing.m_MaxJudge = m_Timing.Perfect + (4 - 1) * m_Timing.Interval;	
	m_Timing.Miss = -m_Timing.Delay - m_Timing.m_MaxJudge;
#endif // #if 0
}

// Decompiled
int GetNextLineIndex(float time, Step::Div* pDiv)
{
	int outLine = 0;

	float divTime = time - pDiv->msStart;
	if((0.0f <= divTime) && pDiv->msPerLine != 0.0f)
	{
		outLine = (int) (divTime / pDiv->msPerLine + 1);
		if(pDiv->nLine <= outLine)
		{
			outLine = pDiv->nLine;	// This feels like a bug
		}
	}

	// Returns a line number
	return outLine;
}

// Decompiled
int CNX10Step::GetJudgeRegion(float a2)
{
	int v2 = (int)( (a2 - m_Timing.Perfect) / m_Timing.Interval + 1.0f );
	if(0 <= v2)
	{
		if(m_Timing.m_JudgeRegions <= v2)
		{
			v2 = m_Timing.m_JudgeRegions - 1;
		}
		return v2;
	}

	return 0;
}

int dword_48D560[] = {0, 0, // 16 lines, 32 total
					  1, 0, 
					  2, 0, 
					  1, 0, 
					  3, 0, 
					  1, 0, 
					  2, 0, 
					  1, 0, 
					  4, 0, 
					  1, 0, 
					  2, 0, 
					  1, 0, 
					  3, 0, 
					  1, 0, 
					  2, 0, 
					  1, 0};

#define _DWORD int
#define _WORD short
#define _BYTE char
void CNX10Step::Judge(const int input, const int holds)
{
	using namespace Step;

	CNX10Step *this2; // ebp@1
	Position *pProcessingPos; // esi@1
	int inputMask; // eax@1
	Position *pJudgeStartPos; // edi@1
	int v7; // eax@2
	Div *v8; // esi@2
	int nextLine; // ecx@3
	int lineOffset; // eax@8
	signed int bank; // ebx@9
	Note *pLine; // edi@9
	int v14; // esi@9
	int v15; // eax@11
	int v16; // esi@13
	Note *v17; // edi@15
	signed int v18; // edx@17
	char v19; // zf@17
	int Id; // eax@21
	int index; // esi@23
	char v23; // al@26
	signed int v24; // edx@30
	char v25; // zf@30
	int v26; // eax@33
	int v27; // eax@34
	int v28; // edi@34
	int v29; // eax@34
	int v30; // ecx@34
	int v31; // edx@35
	int v32; // ecx@36
	int result; // eax@37
	float v34; // ST38_4@37
	Div* v35; // eax@38
	int v36; // esi@38
	Step_t *v37; // ecx@38
	int v38; // edx@38
	int v39; // eax@39
	float v40; // ST38_4@39
	int v42; // ebx@44
	Step_t *pLine2; // ebx@45
	int v44; // eax@49
	int v45; // edx@49
	Step_t *v46; // edx@49
	signed int v47; // esi@49
	int v48; // ecx@49
	int v49; // edi@50
	char v50; // cl@51
	int v51; // ecx@55
	int v52; // ecx@57
	double v53; // st7@63
	double v54; // st7@64
	int v55; // ecx@70
	char v56; // dl@72
	signed int v57; // edi@72
	Note* v58; // esi@72
	double v60; // st7@73
	signed int v64; // ecx@74
	int v74; // eax@82
	char v75; // al@82
	signed int v76; // edi@84
	char v77; // zf@84
	int v78; // edi@86
	int v79; // eax@87
	float v80; // ST3C_4@87
	int loopNote; // eax@90
	Step_t *v82; // ecx@90
	int nNote; // esi@90
	char v84; // cl@91
	int v85; // ecx@93
	char v86; // cl@95
	int v87; // esi@95
	int v88; // esi@95
	Step_t *v89; // ecx@98
	unsigned __int8 v90; // sf@99
	unsigned __int8 v91; // of@99
	int v92; // ecx@100
	int v93; // esi@102
	signed int v94; // eax@104
	signed int v96; // eax@105
	char v97; // al@106
	int v98; // eax@106
	int v99; // ST3C_4@106
	int v100; // edx@106
	int v101; // eax@107
	int v102; // edx@107
	signed int v103; // eax@111
	char v104; // zf@111
	int i; // edi@114
	int v106; // esi@116
	int v107; // [sp+50h] [bp-3Ch]@11
	int v108; // [sp+50h] [bp-3Ch]@21
	unsigned int v109; // [sp+50h] [bp-3Ch]@68
	int v110; // [sp+50h] [bp-3Ch]@100
	float missTime; // [sp+54h] [bp-38h]@1
	float v112; // [sp+54h] [bp-38h]@63
	signed int v113; // [sp+58h] [bp-34h]@9
	int v114; // [sp+58h] [bp-34h]@67
	char *v115; // [sp+5Ch] [bp-30h]@44
	Note *v116; // [sp+60h] [bp-2Ch]@9
	Step_t *v117; // [sp+60h] [bp-2Ch]@47
	float v118; // [sp+64h] [bp-28h]@64
	int numFoos; // [sp+68h] [bp-24h]@1
	Div *v120; // [sp+6Ch] [bp-20h]@2
	Div* v121; // [sp+6Ch] [bp-20h]@38
	char *v122; // [sp+70h] [bp-1Ch]@67
	int v123; // [sp+74h] [bp-18h]@67
	int v124; // [sp+78h] [bp-14h]@37
	int v125; // [sp+7Ch] [bp-10h]@34
	int v126; // [sp+7Ch] [bp-10h]@72
	unsigned int missBlock; // [sp+80h] [bp-Ch]@1
	int v128; // [sp+80h] [bp-Ch]@37
	int v129; // [sp+84h] [bp-8h]@3
	int v130; // [sp+84h] [bp-8h]@39
	int maskedInput; // [sp+90h] [bp+4h]@1
	int maskedHolds; // [sp+94h] [bp+8h]@1

	this2 = this;
	inputMask = ((1 << this->m_pStep->nNote) - 1) << this->m_pStep->nNoteOffset;
	missTime = this2->m_Timing.Miss + this2->m_msCurTime;
	maskedHolds = inputMask & holds;
	pJudgeStartPos = &this->m_JudgeStartPos;
	maskedInput = inputMask & input;
	pProcessingPos = &this->m_ProcessingPos;
	numFoos = this->m_NumFoos;
	missBlock = GetBlockIndex(missTime, this->m_JudgeStartPos.Block);
	this2->m_ProcessingPos.Block = pJudgeStartPos->Block;
	this2->m_ProcessingPos.Line = pJudgeStartPos->Line;
	this2->m_ProcessingPos.pDiv = pJudgeStartPos->pDiv;
	if ( this2->m_ProcessingPos.Block <= (signed int)missBlock )
	{
		do
		{
			v7 = pProcessingPos->Block;
			v8 = GetDiv(pProcessingPos->Block);
			v120 = v8;
			this2->m_ProcessingPos.pDiv = v8;
			if ( v7 == missBlock )
			{
				nextLine = GetNextLineIndex(missTime, v8);
				v129 = nextLine;
				if ( (signed int)missBlock > this2->m_JudgeStartPos.Block || nextLine > this2->m_JudgeStartPos.Line )
				{
					this2->m_JudgeStartPos.Block = missBlock;
					this2->m_JudgeStartPos.Line = nextLine;
					this2->m_JudgeStartPos.pDiv = NULL;
				}
			}
			else
			{
				v129 = v8->nLine;
			}
			for ( ; this2->m_ProcessingPos.Line < v129; ++this2->m_ProcessingPos.Line )
			{
				lineOffset = v8->ofsLine[this2->m_ProcessingPos.Line];
				if ( lineOffset )
				{
					v14 = (int)&this2->m_FooArray[numFoos];
					pLine = (Note *)((char *)this2->m_pStep + lineOffset + -2 * this2->m_pStep->nNoteOffset);
					bank = 0;
					v116 = (Note *)((char *)this2->m_pStep + lineOffset + -2 * this2->m_pStep->nNoteOffset);
					v113 = 0;
					do
					{
						if ( *(_WORD *)(v14 + 2 * bank) )
						{
							v15 = *(_WORD *)(v14 + 2 * bank);
							v107 = *(_WORD *)(v14 + 2 * bank);
							if ( *(_WORD *)(v14 + 2 * bank) )
							{
								// No point since the function doesn't do anything?
								do
								{
									if ( v15 & 0x1F )
										v16 = dword_48D560[v15 & 0x1F];
									else
										v16 = dword_48D560[(unsigned int)v15 >> 5] + 5;
									v17 = &pLine[v16];
									if ( Virtual_C(bank, v16, v17, -1) )
										v17->Attr &= 0xCFu;
									pLine = v116;
									bank = v113;
									v18 = ~(1 << v16);
									v15 = v18 & v107;
									v19 = (v18 & v107) == 0;
									v107 &= v18;
								}
								while ( !v19 );
								v14 = (int)&this2->m_FooArray[numFoos];
							}
							JudgeLine(bank, pLine, -1);
						}
						++bank;
						v113 = bank;
					}
					while ( bank < 3 );
					Id = *(_WORD *)(v14 + 6);
					v108 = *(_WORD *)(v14 + 6);
					if ( *(_WORD *)(v14 + 6) )
					{
						do
						{
							if ( Id & 0x1F )
							{
								index = dword_48D560[Id & 0x1F];
							}
							else
							{
								Id = (unsigned int)Id >> 5;
								index = dword_48D560[Id] + 5;
							}
							Id = (unsigned __int8)pLine[index].Id_Bank >> 2;
							Id = LOBYTE(Id);
							if ( (pLine[index].Attr & 7) == 1 )
								v23 = JudgeItem(index, Id, -1);
							else
								v23 = JudgeSpecial(index, Id, -1);
							if ( v23 )
								pLine[index].Attr &= 0xCFu;
							v24 = ~(1 << index);
							Id = v24 & v108;
							v25 = (v24 & v108) == 0;
							v108 &= v24;
						}
						while ( !v25 );
					}
					v8 = v120;
				}
				numFoos = (numFoos + 1) & 0xFF;
			}
			pProcessingPos = &this2->m_ProcessingPos;
			v26 = this2->m_ProcessingPos.Block + 1;
			this2->m_ProcessingPos.Line = 0;
			this2->m_ProcessingPos.Block = v26;
		}
		while ( v26 <= (signed int)missBlock );
	}
	v28 = this2->m_FooPos.Block;
	v29 = this2->m_FooPos.Line - this2->m_JudgeStartPos.Line;
	this2->m_NumFoos = numFoos;
	v27 = numFoos + v29;
	v30 = this2->m_JudgeStartPos.Block;
	pProcessingPos->Block = v30;
	v125 = v28;
	if ( v30 < v28 )
	{
		v31 = (int)this2->m_pStep;
		do
		{
			v28 = v125;
			v27 += GetDiv(pProcessingPos->Block)->nLine;
			v32 = pProcessingPos->Block + 1;
			pProcessingPos->Block = v32;
		}
		while ( v32 < v125 );
	}
	v124 = (unsigned __int8)v27;
	v34 = *(float *)&this2->m_Timing.m_MaxJudge + this2->m_msCurTime;
	result = GetBlockIndex(v34, v28);
	v128 = result;
	pProcessingPos->Block = this2->m_JudgeStartPos.Block;
	pProcessingPos->Line = this2->m_JudgeStartPos.Line;
	pProcessingPos->pDiv = this2->m_JudgeStartPos.pDiv;
	if ( pProcessingPos->Block <= result )        // Continue Here
	{
		while ( 1 )
		{
			v36 = pProcessingPos->Block;
			v37 = this2->m_pStep;
			v38 = v37->ofsBlock[v36] + 4 * this2->m_pRandDiv[v36] + 4;
			v35 = GetDiv(v36);
			v121 = GetDiv(v36);
			if ( v36 == v128 )
			{
				v40 = *(float *)&this2->m_Timing.m_MaxJudge + this2->m_msCurTime;
				v39 = GetNextLineIndex(v40, v35);
				v130 = v39;
				if ( v128 > this2->m_FooPos.Block || v39 > this2->m_FooPos.Line )
				{
					this2->m_FooPos.Block = v128;
					this2->m_FooPos.Line = v39;
					this2->m_FooPos.pDiv = NULL;
				}
			}
			else
			{
				v39 = v35->nLine;
				v130 = v39;
			}
			if ( this2->m_ProcessingPos.Line < v39 )
				break;
LABEL_113:
			pProcessingPos = &this2->m_ProcessingPos;
			result = this2->m_ProcessingPos.Block + 1;
			this2->m_ProcessingPos.Line = 0;
			this2->m_ProcessingPos.Block = result;
			if ( result > v128 )
				goto LABEL_114;
		}
		while ( 1 )
		{
			v115 = (char *)&this2->m_FooArray[numFoos];
			v42 = v121->ofsLine[this2->m_ProcessingPos.Line];
			pLine2 = v42 ? (Step_t *)((char *)this2->m_pStep + v42 - 2 * this2->m_pStep->nNoteOffset) : 0;
			v117 = pLine2;
			if ( numFoos == v124 )
			{
				memset(v115, 0, 0x18u);
				if ( pLine2 )
				{
					v46 = this2->m_pStep;
					v48 = v46->nNoteOffset;
					v45 = v46->nNote;
					v47 = 1 << v48;
					v44 = (int)((char *)pLine2 + 2 * v48);
					if ( v45 > 0 )
					{
						v49 = v45;
						do
						{
							v50 = *(_BYTE *)v44;
							if ( *(_BYTE *)v44 < 0 )
							{
								if ( LOBYTE(this2->m_bSingleBank) )
								{
									if ( (v50 & 7u) >= 3 )
										*(_BYTE *)(v44 + 1) &= 0xFCu;
								}
								v51 = v50 & 7;
								if ( v51 )
								{
									if ( v51 == 3 )
									{
										v52 = (int)&v115[(*(_BYTE *)(v44 + 1) & 3) + 8];
										++*(_BYTE *)v52;
									}
									*(_WORD *)&v115[2 * (*(_BYTE *)(v44 + 1) & 3)] |= v47;
								}
							}
							v47 *= 2;
							v44 += 2;
							--v49;
						}
						while ( v49 );
					}
				}
				v124 = (v124 + 1) & 0xFF;
			}
			if ( pLine2 && (_WORD)maskedHolds )
				break;
LABEL_112:
			numFoos = (numFoos + 1) & 0xFF;           // % 255
			++this2->m_ProcessingPos.Line;
			if ( this2->m_ProcessingPos.Line >= v130 )
				goto LABEL_113;
		}
		v53 = (double)this2->m_ProcessingPos.Line * v121->msPerLine + v121->msStart - this2->m_msCurTime;
		v112 = (float)v53;
		if ( v53 >= 0.0 )
		{
			v118 = (float)v53;
		}
		else
		{
			v54 = -v112 - this2->m_Timing.Delay;
			v118 = (float)v54;
			if ( v54 < 0.0 )
				v118 = 0;
		}
		v114 = 0;
		v122 = (char *)&this2->m_FooArray[numFoos];
		v123 = (int)(v115 + 12);
LABEL_68:
		v109 = (unsigned __int16)(maskedHolds & *(_WORD *)v122);
		if ( !((_WORD)maskedHolds & *(_WORD *)v122) )
			goto LABEL_99;
		while ( 1 )
		{
			if ( v109 & 0x1F )
				v55 = dword_48D560[v109 & 0x1F];
			else
				v55 = dword_48D560[v109 >> 5] + 5;
			v58 = (Note*)((char *)pLine2 + 2 * v55);
			v56 = pLine2->magic[2 * v55] & 7;
			v57 = 1 << v55;
			v126 = v55;
			if ( pLine2->magic[2 * v55] & 4 )
			{
				v60 = -(this2->m_Timing.Delay + *(float *)&this2->m_Timing.Interval + *(float *)&this2->m_Timing.Perfect);
				if ( v60 < v112 )
				{
					v64 = ~v57;
					maskedInput &= ~v57;
					if ( v112 < (double)*(float *)&this2->m_Timing.Perfect )
					{
						if ( v56 != 4 )
						{
							if ( v56 == 7 )
								maskedHolds &= v64;
							goto LABEL_82;
						}
						if ( v112 <= 0.0 )
							goto LABEL_82;
					}
					maskedHolds &= v64;
				}
			}
			else
			{
				if ( (_WORD)maskedInput & (_WORD)v57 )
				{
					v64 = ~(_WORD)v57;
					v64 = LOWORD(v64);
					maskedInput &= ~v57;
					maskedHolds &= ~v57;
					*(float *)v123 = v118 + *(float *)v123;
LABEL_82:
					*(_WORD *)v122 &= v64;
					v58->Attr |= 8u;
					v74 = GetJudgeRegion(v118);
					v75 = Virtual_C(
						v114,
						v126,
						v58,
						v74);                         // Return0
					pLine2 = v117;
					if ( v75 )
						v58->Attr &= 0xCFu;
					goto LABEL_84;
				}
			}
LABEL_84:
			v76 = ~v57;
			v77 = (v76 & v109) == 0;
			v109 &= v76;
			if ( v77 )
			{
				if ( !*(_WORD *)v122 )
				{
					v78 = v114;
					if ( v115[v114 + 8] )
					{
						v80 = *(float *)v123 / (float)(unsigned __int8)v115[v114 + 8];
						v79 = GetJudgeRegion(v80);
					}
					else
					{
						v79 = 0;
					}
					if ( JudgeLine(
						v114,
						(Note*)pLine2,
						v79) )        // CPlayer::JudgeLine()
					{
						v82 = this2->m_pStep;
						loopNote = v82->nNoteOffset;
						nNote = v82->nNote;
						if ( loopNote < loopNote + nNote )  // if(0 < nNote)
						{
							do
							{
								v84 = pLine2->magic[2 * loopNote];
								if ( v84 & 8 )                  // bChecked
								{
									if ( (pLine2->magic[2 * loopNote + 1] & 3) == v78 )// curNote.Id_Bank & JudgeBankMask
									{
										v85 = v84 & 7;              // TypeMask
										if ( v85 & 4 )              // bLong
										{
											if ( v85 != 7 )           // !LongEnd
											{
												v88 = loopNote + nNote;
												v86 = pLine2->magic[2 * v88];
												v87 = (int)((char *)pLine2 + 2 * v88);
												if ( (v86 & 7) == 6 )   // LongMiddle
													*(_BYTE *)v87 = v86 & 0xFD | 5;
											}
										}
										v78 = v114;
										pLine2->magic[2 * loopNote] &= 0xCFu;
									}
								}
								v89 = this2->m_pStep;
								++loopNote;
								nNote = v89->nNote;
							}
							while ( loopNote < nNote + v89->nNoteOffset );
						}
					}
				}
LABEL_99:
				v91 = v114 + 1 < 3;
				v90 = v114++ - 2 < 0;
				v122 += 2;
				v123 += 4;
				if ( !(v90 ^ v91) )
				{
					v92 = (unsigned __int16)(maskedHolds & *((_WORD *)v115 + 3));
					v110 = (unsigned __int16)(maskedHolds & *((_WORD *)v115 + 3));
					if ( (_WORD)maskedHolds & *((_WORD *)v115 + 3) )
					{
						do
						{
							if ( v92 & 0x1F )
								v93 = dword_48D560[v92 & 0x1F];
							else
								v93 = dword_48D560[(unsigned int)v92 >> 5] + 5;
							v94 = 1 << v93;
							if ( (unsigned __int16)maskedInput & (1 << v93) )
							{
								v96 = ~v94;
								maskedHolds &= v96;
								maskedInput &= v96;
								*((_WORD *)v115 + 3) &= v96;
								v96 = pLine2->magic[2 * v93] | 8;
								v96 = LOBYTE(v96);
								pLine2->magic[2 * v93] = v96;
								if ( (v96 & 7) == 1 )
								{
									v98 = GetJudgeRegion(v118);
									v99 = v98;
									v98 = (unsigned __int8)pLine2->magic[2 * v93 + 1] >> 2;
									v98 = LOBYTE(v98);
									v97 = JudgeItem(
										v100,
										v93,
										v98);
								}
								else
								{
									v101 = GetJudgeRegion(v118);
									v102 = (unsigned __int8)pLine2->magic[2 * v93 + 1] >> 2;
									v102 = LOBYTE(v102);
									v97 = JudgeSpecial(
										v93,
										v102,
										v101);
								}
								if ( v97 )
									pLine2->magic[2 * v93] &= 0xCFu;
								v94 = 1 << v93;
							}
							v103 = ~v94;
							v92 = v103 & v110;
							v104 = (v103 & v110) == 0;
							v110 &= v103;
						}
						while ( !v104 );
					}
					goto LABEL_112;
				}
				goto LABEL_68;
			}
		}
	}
LABEL_114:
	// Doesn't seem like this code does anything.
	// lpVtbl + 3 points to an empty function that just returns 0.
	// (for both CNX10Step and CPlayer)
	for ( i = maskedInput; (_WORD)i; i &= ~(1 << v106) )
	{
		if ( i & 0x1F )
			v106 = dword_48D560[i & 0x1F];
		else
			v106 = dword_48D560[(unsigned int)(unsigned __int16)i >> 5] + 5;
		result = Virtual_C(0, v106, 0, -1);
	}
	return;
#if USE_DECOMPILED_CODE && 0
	int inputMask = ((1 << m_pStep->nNote) - 1) << m_pStep->nNoteOffset;
	float missTime = m_Timing.Miss + m_msCurTime;
	int maskedInput = inputMask & input;
	int maskedHolds = inputMask & holds;
	int numFoos = m_NumFoos;
	int missBlock = GetBlockIndex(missTime, m_JudgeStartPos.Block);
	m_ProcessingPos = m_JudgeStartPos;

	if(m_ProcessingPos.Block <= missBlock)
	{
		do
		{
			m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
			int v129 = m_ProcessingPos.pDiv->nLine;
			if(m_ProcessingPos.Block == missBlock)
			{
				v129 = GetNextLineIndex(missTime, m_ProcessingPos.pDiv);
				if(m_ProcessingPos.Block < missBlock || m_JudgeStartPos.Line < v129)
				{
					m_JudgeStartPos.Block = missBlock;
					m_JudgeStartPos.Line = v129;
					m_JudgeStartPos.pDiv = NULL;	// set to stack variable that's not initialized
				}
			}

			for(; m_ProcessingPos.Line < v129; m_ProcessingPos.Line++)
			{
				int lineOffset = m_ProcessingPos.pDiv->ofsLine[m_ProcessingPos.Line];
				if(lineOffset)
				{
					Step::Note* pLine = (Step::Note*)((char*)m_pStep + lineOffset - 2 * m_pStep->nNoteOffset);
					for(int bank = 0; bank < 3; bank++)
					{
						int fooBank = m_FooArray[numFoos][bank * 2];
						if(fooBank)
						{
							int v15 = fooBank;
							int v107 = fooBank;

							do 
							{
								if(v15 & 0x1F)	// v15 % 32 = [0, 31]
								{
									v16 = dword_48D560[v15 & 0x1F];
								}
								else
								{
									v16 = dword_48D560[v15 >> 5] + 5;
								}

								Step::Note& v17 = pLine[v16 >> 1]; // pLine goes in two byte units (sizeof(Step::Note))
								if(Virtual_C(bank, v16, v17, -1))
								{
									v17.Attr &= ~Visible;
								}

								int v18 = ~(1 << v16);
								v15 = v18 & v107;
								v107 &= v18;
							} while (v15 != 0);
						}

						JudgeLine(bank, pLine, -1);
					}

					int Id = m_FooArray[numFoos + 1][0];
					int v108 = Id;
					if(Id)
					{
						do 
						{
							if(Id & 0x1F)	// % 32 = [0, 31]
							{
								index = dword_48D560[Id & 0x1F];
							}
							else
							{
								index = dword_48D560[Id >> 5] + 5;
							}
							Id = pLine[index].Id_Bank >> 2;

							bool v23 = false;
							if(pLine[index].Attr & TypeMask == Item)
							{
								v23 = JudgeItem(index, Id, -1);
							}
							else
							{
								v23 = JudgeSpecial(index, Id, -1);
							}
							if(v23)
							{
								pLine[index].Attr &= ~Visible;
							}

							v24 = ~(1 << index);
							Id = v24 & v108;
							v108 &= v24;
						} while (Id != 0);
					}
				}
				numFoos = (numFoos + 1) & 0xFF;
			}
			m_ProcessingPos.Line = 0;
			m_ProcessingPos.Block++;
			// pDiv set inside the loop
		}
		while(m_ProcessingPos.Block <= missBlock);
	}

	v28 = m_FooPos.Block;
	v29 = m_FooPos.Line - m_JudgeStartPos.Line;
	m_NumFoos = numFoos;
	v27 = numFoos + v29;
	m_ProcessingPos.Block = m_JudgeStartPos.Block;
	v125 = v28;

	if(m_ProcessingPos.Block < v28)
	{
		do 
		{
			v27 += GetDiv(m_ProcessingPos.Block)->nLine;
			m_ProcessingPos.Block++;
		} while (m_ProcessingPos.Block < v28);
	}
	v124 = v27;
	v34 = m_Timing.m_MaxJudge + m_msCurTime;
	v128 = GetBlockIndex(v34, v28);
	m_ProcessingPos = m_JudgeStartPos;
	if(m_ProcessingPos.Block < v128)
	{
		while(1)
		{
		}
	}
#else

	using namespace Step;

	m_ProcessingPos = m_CurPos;
	int endBlock = GetBlockIndex(m_Timing.m_MaxJudge + m_msCurTime, m_ProcessingPos.Block);
	for(; m_ProcessingPos.Block < endBlock; m_ProcessingPos.Block++)
	{
		m_ProcessingPos.pDiv = GetDiv(m_ProcessingPos.Block);
		for(; m_ProcessingPos.Line < m_ProcessingPos.pDiv->nLine; m_ProcessingPos.Line++)
		{
			Step::Note* pLine = GetLinePtr(m_ProcessingPos.pDiv->ofsLine[m_ProcessingPos.Line]);
			if(pLine)
			{
				int numChecked = 0;
				bool stepRegistered = false;

				const float lineTime = m_ProcessingPos.pDiv->msStart + m_ProcessingPos.pDiv->msPerLine * m_ProcessingPos.Line;
				for(int note = 0; note < m_pStep->nNote; note++)
				{
					bool notePress = (input & (1 << note))?true:false;
					bool noteHold = (holds & (1 << note))?true:false;

					Step::Note& curNote = pLine[note];

					if(curNote.Attr & bNeedCheck && !(curNote.Attr & bChecked))
					{
						// Assuming notePress also triggers noteHold
						if(((curNote.Attr & bLong) && noteHold) ||
							notePress)
						{
							curNote.Attr |= bChecked;
							numChecked++;
							stepRegistered = true;
						}
					}
					else
					{
						numChecked++;
					}
				}

				if(stepRegistered && (numChecked == m_pStep->nNote))
				{
					const float timeDiff = abs(lineTime - m_msCurTime);
					for(int zone = ePerfect; zone < eZoneMax; zone++)
					{
						if(timeDiff < m_Timing.Perfect + m_Timing.Interval * zone)	// use all zones or m_JudgeRegions?
						{
							// Increment the note counter and display judgment to the screen
							if(JudgeLine(0, pLine, zone)) // returns true if Great or Perfect. Stop drawing this line.
							{
								for(int note = 0; note < m_pStep->nNote; note++)
								{
									Step::Note& curNote = pLine[note];

									if(curNote.Attr & bChecked)
									{
										if((curNote.Id_Bank & JudgeBankMask) == 0)	// Not sure what a bank is...
										{
											if(curNote.Attr & bLong)
											{
												if((curNote.Attr & TypeMask) != LongEnd)
												{
													Step::Note& nextLineNote = pLine[note + m_pStep->nNote];
													if((nextLineNote.Attr & TypeMask) == LongMiddle)
													{
														// No idea what 1 is supposed to mean
														// Disassembly does var = var & 0xFD | 5
														nextLineNote.Attr &= ~Special;
														nextLineNote.Attr |= bLong | 1;
													}
												}
											}
											// Clear visible flags
											curNote.Attr &= ~Visible;
										}
									}
								}
							}
							break;
						}
					}
				}
			}
		}
	}
#endif // #if USE_DECOMPILED_CODE
}

// See overridden functions in CPlayer. Need stubs for the lightmap steps.
bool CNX10Step::DrawLine(float BlockBeat, float BeatInBlock, Step::Note *pLine)
{
	return false;
}

bool CNX10Step::DrawMarkerLine(float BlockBeat, int BeatInBlock, bool bMeasure)
{
	return false;
}

bool CNX10Step::Virtual_C(int Bank, int v16, Step::Note* pLine, int something)
{
	return false;
}

bool CNX10Step::JudgeItem(int index, uint8 Id, int Grade)
{
	return true;
}

bool CNX10Step::JudgeSpecial(int index, uint8 Id, int Grade)
{
	return true;
}

bool CNX10Step::JudgeLine(int Bank, Step::Note *pLine, int Grade)
{
	return false;
}

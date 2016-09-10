#include "main_document.h"
#include "libc_max_common.h"
#include "basutil.h"

CMaxDocument::CMaxDocument()
{
    m_strFile = _T("");

    m_strName = DEFAULT_DOCUMENT_NAME;
    m_bFiled = false;
    m_bSaved = false;

	m_nCurStack = 0;

	m_nStatusSM = 0;
}

CMaxDocument::CMaxDocument(int ord)
{
    m_strFile = _T("");
    m_strName.Format(DEFAULT_DOCUMENT_NAME _T(" %d"), ord);
    m_bFiled = false;
    m_bSaved = false;

	m_nCurStack = 0;

	m_nStatusSM = 0;
}

CMaxDocument::~CMaxDocument()
{
	// NO DESTRUCTION
	size_t s = m_sevStack.size();
	for(size_t i = 0; i < s; i++)
		DelAction(0);
}

void CMaxDocument::Init(void)
{
	m_nArrowsPerLine = 5;
	// ** Reservando memoria

	// STEPHEADER
	memset(&song.hStep, 0, sizeof(STEPHEADER));

	song.hStep.nArrowsPerLine = m_nArrowsPerLine;

	// STEP
	Init_SKSF2(&song);

	// STEPAUX
	InitAux();
}

void CMaxDocument::InitAux(void)
{
	hAux.fWChangeBPM=(float*)calloc(song.hStep.nSizeChangeBPM,sizeof(float));

	hAux.fWBeatSplit=(float*)calloc(song.hStep.nSizeBeatSplit,sizeof(float));

	hAux.fIChangeVel=(float*)calloc(song.hStep.nSizeChangeVel,sizeof(float));
	hAux.fEChangeVel=(float*)calloc(song.hStep.nSizeChangeVel,sizeof(float));

	hAux.fIJumps=(float*)calloc(song.hStep.nSizeJumps,sizeof(float));
	hAux.fEJumps=(float*)calloc(song.hStep.nSizeJumps,sizeof(float));

	hAux.fWTickCount=(float*)calloc(song.hStep.nSizeTickCount,sizeof(float));

	hAux.fIStops=(float*)calloc(song.hStep.nSizeStops,sizeof(float));
	hAux.fEStops=(float*)calloc(song.hStep.nSizeStops,sizeof(float));

	// STEPGROUPAUX
	hGroupAux=(STEPGROUPAUX*)calloc(song.hStep.nSizeArrowGroups,sizeof(STEPGROUPAUX));
	for(int i = 0; i < song.hStep.nSizeArrowGroups; i++)
	{
		hGroupAux[i].fLimitArrows = 0.0f;
	}
}

void CMaxDocument::InitArrowGruops(int nSizeBefore, int nSize)
{
	song.hStep.nSizeArrowGroups = nSize;
	if(nSizeBefore == nSize) return;
	if(nSizeBefore < nSize)
	{
		song.hStepGroup=(STEPGROUP*)max_realloc(song.hStepGroup, song.hStep.nSizeArrowGroups*sizeof(STEPGROUP));
		hGroupAux=(STEPGROUPAUX*)max_realloc(hGroupAux, song.hStep.nSizeArrowGroups*sizeof(STEPGROUPAUX));
		for(int i = nSizeBefore; i < song.hStep.nSizeArrowGroups; i++)
		{
		    memset(&song.hStepGroup[i].hGroup, 0, sizeof(STEPGROUPHEADER));

			song.hStepGroup[i].nArrows=(int*)calloc(song.hStepGroup[i].hGroup.nSizeArrows*m_nArrowsPerLine,sizeof(int));
			song.hStepGroup[i].nPArrows=(int*)calloc(song.hStepGroup[i].hGroup.nSizeArrows*m_nArrowsPerLine,sizeof(int));
		}
	}
	else
	{
		for(int i = song.hStep.nSizeArrowGroups; i < nSizeBefore; i++)
		{
			delete song.hStepGroup[i].nArrows;
			delete song.hStepGroup[i].nPArrows;
		}
		song.hStepGroup=(STEPGROUP*)max_realloc(song.hStepGroup, song.hStep.nSizeArrowGroups*sizeof(STEPGROUP));
		hGroupAux=(STEPGROUPAUX*)max_realloc(hGroupAux, song.hStep.nSizeArrowGroups*sizeof(STEPGROUPAUX));
	}
}

void CMaxDocument::Reinit(void)
{
	m_nArrowsPerLine = song.hStep.nArrowsPerLine;
	// ** Reservando memoria

	// STEP
	ReInit_SKSF2(&song);

	// STEPAUX
	ReinitAux();
}

void CMaxDocument::ReinitAux(void)
{
	hAux.fWChangeBPM=(float*)max_realloc(hAux.fWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(float));

	hAux.fWBeatSplit=(float*)max_realloc(hAux.fWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(float));

	hAux.fIChangeVel=(float*)max_realloc(hAux.fIChangeVel, song.hStep.nSizeChangeVel*sizeof(float));
	hAux.fEChangeVel=(float*)max_realloc(hAux.fEChangeVel, song.hStep.nSizeChangeVel*sizeof(float));

	hAux.fIJumps=(float*)max_realloc(hAux.fIJumps, song.hStep.nSizeJumps*sizeof(float));
	hAux.fEJumps=(float*)max_realloc(hAux.fEJumps, song.hStep.nSizeJumps*sizeof(float));

	hAux.fWTickCount=(float*)max_realloc(hAux.fWTickCount, song.hStep.nSizeTickCount*sizeof(float));

	hAux.fIStops=(float*)max_realloc(hAux.fIStops, song.hStep.nSizeStops*sizeof(float));
	hAux.fEStops=(float*)max_realloc(hAux.fEStops, song.hStep.nSizeStops*sizeof(float));

	// AUX
	hGroupAux=(STEPGROUPAUX*)max_realloc(hGroupAux, song.hStep.nSizeArrowGroups*sizeof(STEPGROUPAUX));
}

int CMaxDocument::New(void)
{
	m_strFile = _T("");

    m_strName = DEFAULT_DOCUMENT_NAME;
    m_bFiled = false;
    m_bSaved = false;
    //  Add new file here
	Init();
	if(!NewInit()) return 0;
    return 1;
}

int CMaxDocument::New(int ord)
{
	m_strFile = _T("");
    m_strName.Format(DEFAULT_DOCUMENT_NAME _T(" %d"), ord);
    m_bFiled = false;
    m_bSaved = false;
    //  Add new file here
	Init();
	if(!NewInit()) return 0;
    return 1;
}

int CMaxDocument::Open(LPCTSTR strFile)
{
    m_strFile = strFile;
	m_strName = m_strFile.Right(m_strFile.GetLength() - max(m_strFile.ReverseFind('/'), m_strFile.ReverseFind('\\')) - 1);
	int nExt = m_strName.ReverseFind('.');
	if(nExt != -1) m_strName = m_strName.Left(nExt);

	CString strExt = _T("");
	nExt = m_strFile.ReverseFind('.');
	if(nExt != -1)
		strExt = m_strFile.Right(m_strFile.GetLength() - nExt);

	if(strExt.CompareNoCase(MAIN_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		OpenRAW_SKSF2(strFile, &song);
		m_bSaved = true;
		m_bFiled = true;
	}
	else if(strExt.CompareNoCase(SKSF_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		SKSFv1 sksf;
		OpenRAW_SKSF1(m_strFile, &sksf);
		Convert_SKSFv1_SKSFv2(&song, &sksf);
		Delete_SKSF1(&sksf);
		m_bSaved = false;
		m_bFiled = false;
	}
	else if(strExt.CompareNoCase(SMA_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		OpenRAW_SM(m_strFile, &m_sm);
		Init();
		if(!NewInit()) return 0;
		m_nStatusSM = MAX_SMA_FILED;
		m_bSaved = false;
		m_bFiled = false;
	}
	else if(strExt.CompareNoCase(SM_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		OpenRAW_SM(m_strFile, &m_sm);
		Init();
		if(!NewInit()) return 0;
		m_nStatusSM = MAX_SM_FILED;
		m_bSaved = false;
		m_bFiled = false;
	}
	else if(strExt.CompareNoCase(KSF_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		OpenRAW_SM(m_strFile, &m_sm);
		Convert_KSF_SKSFv2(&song, &m_sm);
		m_nStatusSM = MAX_SM_LOADED;
		m_bSaved = false;
		m_bFiled = false;
	}
	else if(strExt.CompareNoCase(UCS_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		Open_UCS_SKSF2(strFile, &song);
		m_bSaved = false;
		m_bFiled = false;
	}
	else if (strExt.CompareNoCase(NX10_DOCUMENT_FILTER_EXTENSION) == 0)
	{
		CNX10Step nx10step;
		OpenRAW_NX10(strFile, &nx10step);
		Convert_NX10_SKSFv2(&song, &nx10step);

		nx10step.Unload();
		m_bSaved = false;
		m_bFiled = false;
	}
	else
	{
		// NO SUPPORTED FILE
		Init();
		if(!NewInit()) return 0;
		m_bSaved = false;
		m_bFiled = false;
		return 0;
	}

	// Init Aux
	Fix();
	InitAux();
	Actualizate();

    return 1;
}

void CMaxDocument::Close(void)
{
	free(hAux.fWChangeBPM);

	free(hAux.fWBeatSplit);

	free(hAux.fIChangeVel);
	free(hAux.fEChangeVel);

	free(hAux.fIJumps);
	free(hAux.fEJumps);

	free(hAux.fWTickCount);

	free(hAux.fIStops);
	free(hAux.fEStops);

	free(hGroupAux);

	Delete_SKSF2(&song);
}

void CMaxDocument::Delete_SKSF2(STEP* fsong)
{
	free(fsong->nWChangeBPM);
	free(fsong->fChangeBPM);

	free(fsong->nWBeatSplit);
	free(fsong->nBeatSplit);

	free(fsong->nIChangeVel);
	free(fsong->nEChangeVel);
	free(fsong->fChangeVel);
	free(fsong->fIChangeVel);

	free(fsong->nIJumps);
	free(fsong->nEJumps);

	free(fsong->nWTickCount);
	free(fsong->nTickCount);

	free(fsong->nIStops);
	free(fsong->nEStops);

	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
		free(fsong->hStepGroup[i].nArrows);
		free(fsong->hStepGroup[i].nPArrows);
	}

	// STEP
	free(fsong->fAttrib);
	free(fsong->hStepGroup);

	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			free(fsong->hMod[i].hModInfo[j].nModInt);
			free(fsong->hMod[i].hModInfo[j].fModFloat);
		}
		free(fsong->hMod[i].hModInfo);
	}

	free(fsong->hMod);
}

void CMaxDocument::Delete_SKSF1(SKSFv1* fsong)
{
	delete fsong->dwArrows1;
	delete fsong->dwArrows2;
	delete fsong->dwArrows3;
	delete fsong->nWChangeBPM;
	delete fsong->dChangeBPM;
	delete fsong->nWBeatSplit;
	delete fsong->nBeatSplit;
	delete fsong->nIChangeVel;
	delete fsong->nEChangeVel;
	delete fsong->dChanges;
	delete fsong->nIJumps;
	delete fsong->nEJumps;
	delete fsong->nWTickCount;
	delete fsong->nTickCount;
	delete fsong->nWStops;
	delete fsong->dStops;
}

void CMaxDocument::Init_SKSF2(STEP* fsong)
{
	// STEP
	fsong->fAttrib=(float*)calloc(fsong->hStep.nSizeAttrib,sizeof(float));

	fsong->nWChangeBPM=(int*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(int));
	fsong->fChangeBPM=(float*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(float));

	fsong->nWBeatSplit=(int*)calloc(fsong->hStep.nSizeBeatSplit,sizeof(int));
	fsong->nBeatSplit=(int*)calloc(fsong->hStep.nSizeBeatSplit,sizeof(int));

	fsong->nIChangeVel=(int*)calloc(fsong->hStep.nSizeChangeVel,sizeof(int));
	fsong->nEChangeVel=(int*)calloc(fsong->hStep.nSizeChangeVel,sizeof(int));
	fsong->fChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));
	fsong->fIChangeVel=(float*)calloc(fsong->hStep.nSizeChangeVel,sizeof(float));

	fsong->nIJumps=(int*)calloc(fsong->hStep.nSizeJumps,sizeof(int));
	fsong->nEJumps=(int*)calloc(fsong->hStep.nSizeJumps,sizeof(int));

	fsong->nWTickCount=(int*)calloc(fsong->hStep.nSizeTickCount,sizeof(int));
	fsong->nTickCount=(int*)calloc(fsong->hStep.nSizeTickCount,sizeof(int));

	fsong->nIStops=(int*)calloc(fsong->hStep.nSizeStops,sizeof(int));
	fsong->nEStops=(int*)calloc(fsong->hStep.nSizeStops,sizeof(int));

	fsong->hStepGroup=(STEPGROUP*)calloc(fsong->hStep.nSizeArrowGroups,sizeof(STEPGROUP));
	fsong->hMod=(STEPMOD*)calloc(fsong->hStep.nSizeMod,sizeof(STEPMOD));

	// STEPGROUP
	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
	    memset(&fsong->hStepGroup[i].hGroup, 0, sizeof(STEPGROUPHEADER));

		fsong->hStepGroup[i].nArrows=(int*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine,sizeof(int));
		fsong->hStepGroup[i].nPArrows=(int*)calloc(fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine,sizeof(int));
	}

	// STEPMOD
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		memset(&fsong->hMod[i].hMod, 0, sizeof(STEPMODHEADER));
		fsong->hMod[i].hModInfo=(STEPMODINFO*)calloc(fsong->hMod[i].hMod.nSize,sizeof(STEPMODINFO));
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fsong->hMod[i].hModInfo[j].nModInt=(int*)calloc(fsong->hMod[i].hMod.nSizeInt,sizeof(int));
			fsong->hMod[i].hModInfo[j].fModFloat=(float*)calloc(fsong->hMod[i].hMod.nSizeFloat,sizeof(float));
		}
	}
}

void CMaxDocument::Init_SKSF1(SKSFv1* fsong)
{
	fsong->dwArrows1=(uint32_t*)calloc(fsong->hStep.nSizeArrows1*10,sizeof(uint32_t));
	fsong->dwArrows2=(uint32_t*)calloc(fsong->hStep.nSizeArrows2*10,sizeof(uint32_t));
	fsong->dwArrows3=(uint32_t*)calloc(fsong->hStep.nSizeArrows3*10,sizeof(uint32_t));
	fsong->nWChangeBPM=(int*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(int));
	fsong->dChangeBPM=(double*)calloc(fsong->hStep.nSizeChangeBPM,sizeof(double));
	fsong->nWBeatSplit=(int*)calloc(fsong->hStep.nSizeBeatSplit,sizeof(int));
	fsong->nBeatSplit=(int*)calloc(fsong->hStep.nSizeBeatSplit,sizeof(int));
	fsong->nIChangeVel=(int*)calloc(fsong->hStep.nSizeChangeVel,sizeof(int));
	fsong->nEChangeVel=(int*)calloc(fsong->hStep.nSizeChangeVel,sizeof(int));
	fsong->dChanges=(double*)calloc(fsong->hStep.nSizeChangeVel,sizeof(double));
	fsong->nIJumps=(int*)calloc(fsong->hStep.nSizeJumps,sizeof(int));
	fsong->nEJumps=(int*)calloc(fsong->hStep.nSizeJumps,sizeof(int));
	fsong->nWTickCount=(int*)calloc(fsong->hStep.nSizeTickcount,sizeof(int));
	fsong->nTickCount=(int*)calloc(fsong->hStep.nSizeTickcount,sizeof(int));
	fsong->nWStops=(int*)calloc(fsong->hStep.nSizeStops,sizeof(int));
	fsong->dStops=(double*)calloc(fsong->hStep.nSizeStops,sizeof(double));
}

void CMaxDocument::ReInit_SKSF2(STEP* fsong)
{
	// STEP
	fsong->fAttrib=(float*)max_realloc(fsong->fAttrib, fsong->hStep.nSizeAttrib*sizeof(float));
	fsong->hStepGroup=(STEPGROUP*)max_realloc(fsong->hStepGroup, fsong->hStep.nSizeArrowGroups*sizeof(STEPGROUP));

	fsong->nWChangeBPM=(int*)max_realloc(fsong->nWChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(int));
	fsong->fChangeBPM=(float*)max_realloc(fsong->fChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(float));

	fsong->nWBeatSplit=(int*)max_realloc(fsong->nWBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));
	fsong->nBeatSplit=(int*)max_realloc(fsong->nBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));

	fsong->nIChangeVel=(int*)max_realloc(fsong->nIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fsong->nEChangeVel=(int*)max_realloc(fsong->nEChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fsong->fChangeVel=(float*)max_realloc(fsong->fChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fsong->fIChangeVel=(float*)max_realloc(fsong->fIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));

	fsong->nIJumps=(int*)max_realloc(fsong->nIJumps, fsong->hStep.nSizeJumps*sizeof(int));
	fsong->nEJumps=(int*)max_realloc(fsong->nEJumps, fsong->hStep.nSizeJumps*sizeof(int));

	fsong->nWTickCount=(int*)max_realloc(fsong->nWTickCount, fsong->hStep.nSizeTickCount*sizeof(int));
	fsong->nTickCount=(int*)max_realloc(fsong->nTickCount, fsong->hStep.nSizeTickCount*sizeof(int));

	fsong->nIStops=(int*)max_realloc(fsong->nIStops, fsong->hStep.nSizeStops*sizeof(int));
	fsong->nEStops=(int*)max_realloc(fsong->nEStops, fsong->hStep.nSizeStops*sizeof(int));

	fsong->hMod=(STEPMOD*)max_realloc(fsong->hMod, fsong->hStep.nSizeMod*sizeof(STEPMOD));

	// STEPGROUPHEADER
	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
	    fsong->hStepGroup[i].nArrows=(int*)max_realloc(fsong->hStepGroup[i].nArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
		fsong->hStepGroup[i].nPArrows=(int*)max_realloc(fsong->hStepGroup[i].nPArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
	}

	// STEPMOD
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		fsong->hMod[i].hModInfo=(STEPMODINFO*)max_realloc(fsong->hMod[i].hModInfo, fsong->hMod[i].hMod.nSize*sizeof(STEPMODINFO));
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fsong->hMod[i].hModInfo[j].nModInt=(int*)max_realloc(fsong->hMod[i].hModInfo[j].nModInt, fsong->hMod[i].hMod.nSizeInt*sizeof(int));
			fsong->hMod[i].hModInfo[j].fModFloat=(float*)max_realloc(fsong->hMod[i].hModInfo[j].fModFloat, fsong->hMod[i].hMod.nSizeFloat*sizeof(float));
		}
	}
}

void CMaxDocument::ReInit_SKSF1(SKSFv1* fsong)
{
	fsong->dwArrows1=(uint32_t*)max_realloc(fsong->dwArrows1, fsong->hStep.nSizeArrows1*10*sizeof(uint32_t));
	fsong->dwArrows2=(uint32_t*)max_realloc(fsong->dwArrows2, fsong->hStep.nSizeArrows2*10*sizeof(uint32_t));
	fsong->dwArrows3=(uint32_t*)max_realloc(fsong->dwArrows3, fsong->hStep.nSizeArrows3*10*sizeof(uint32_t));
	fsong->nWChangeBPM=(int*)max_realloc(fsong->nWChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(int));
	fsong->dChangeBPM=(double*)max_realloc(fsong->dChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(double));
	fsong->nWBeatSplit=(int*)max_realloc(fsong->nWBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));
	fsong->nBeatSplit=(int*)max_realloc(fsong->nBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));
	fsong->nIChangeVel=(int*)max_realloc(fsong->nIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fsong->nEChangeVel=(int*)max_realloc(fsong->nEChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fsong->dChanges=(double*)max_realloc(fsong->dChanges, fsong->hStep.nSizeChangeVel*sizeof(double));
	fsong->nIJumps=(int*)max_realloc(fsong->nIJumps, fsong->hStep.nSizeJumps*sizeof(int));
	fsong->nEJumps=(int*)max_realloc(fsong->nEJumps, fsong->hStep.nSizeJumps*sizeof(int));
	fsong->nWTickCount=(int*)max_realloc(fsong->nWTickCount, fsong->hStep.nSizeTickcount*sizeof(int));
	fsong->nTickCount=(int*)max_realloc(fsong->nTickCount, fsong->hStep.nSizeTickcount*sizeof(int));
	fsong->nWStops=(int*)max_realloc(fsong->nWStops, fsong->hStep.nSizeStops*sizeof(int));
	fsong->dStops=(double*)max_realloc(fsong->dStops, fsong->hStep.nSizeStops*sizeof(double));
}

int CMaxDocument::SaveRAW_SKSF2(LPCTSTR strFile, STEP* fsong)
{
	if(fsong == NULL) return 0;
	ofstream fs;
	fs.open(strFile, ios::out | ios::binary);
	if(!fs.is_open()) return 0;

	fs.write((char*)&fsong->hStep, sizeof(STEPHEADER));

	fs.write((char*)fsong->fAttrib, fsong->hStep.nSizeAttrib*sizeof(float));

	fs.write((char*)fsong->nWChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(int));
	fs.write((char*)fsong->fChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(float));

	fs.write((char*)fsong->nWBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));
	fs.write((char*)fsong->nBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));

	fs.write((char*)fsong->nIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.write((char*)fsong->nEChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.write((char*)fsong->fChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fs.write((char*)fsong->fIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));

	fs.write((char*)fsong->nIJumps, fsong->hStep.nSizeJumps*sizeof(int));
	fs.write((char*)fsong->nEJumps, fsong->hStep.nSizeJumps*sizeof(int));

	fs.write((char*)fsong->nWTickCount, fsong->hStep.nSizeTickCount*sizeof(int));
	fs.write((char*)fsong->nTickCount, fsong->hStep.nSizeTickCount*sizeof(int));

	fs.write((char*)fsong->nIStops, fsong->hStep.nSizeStops*sizeof(int));
	fs.write((char*)fsong->nEStops, fsong->hStep.nSizeStops*sizeof(int));

	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
		fs.write((char*)&fsong->hStepGroup[i].hGroup, sizeof(STEPGROUPHEADER));
		fs.write((char*)fsong->hStepGroup[i].nArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
		fs.write((char*)fsong->hStepGroup[i].nPArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
	}
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		fs.write((char*)&fsong->hMod[i].hMod, sizeof(STEPMODHEADER));
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fs.write((char*)fsong->hMod[i].hModInfo[j].nModInt, fsong->hMod[i].hMod.nSizeInt*sizeof(int));
			fs.write((char*)fsong->hMod[i].hModInfo[j].fModFloat, fsong->hMod[i].hMod.nSizeFloat*sizeof(float));
		}
	}

	fs.close();
	return 1;
}

int CMaxDocument::SaveRAW_SKSF1(LPCTSTR strFile, SKSFv1* fsong)
{
	ofstream fs;
	fs.open(strFile, ios::out | ios::binary);
	if(!fs.is_open()) return 0;

	fs.write((char*)&fsong->hStep, sizeof(SKSFv1HEADER));
	fs.write((char*)fsong->dwArrows1,	fsong->hStep.nSizeArrows1*sizeof(uint32_t)*10);
	fs.write((char*)fsong->dwArrows2,	fsong->hStep.nSizeArrows2*sizeof(uint32_t)*10);
	fs.write((char*)fsong->dwArrows3,	fsong->hStep.nSizeArrows3*sizeof(uint32_t)*10);
	fs.write((char*)fsong->nWChangeBPM,	fsong->hStep.nSizeChangeBPM*sizeof(int));
	fs.write((char*)fsong->dChangeBPM,	fsong->hStep.nSizeChangeBPM*sizeof(double));
	fs.write((char*)fsong->nWBeatSplit,	fsong->hStep.nSizeBeatSplit*sizeof(int));
	fs.write((char*)fsong->nBeatSplit,	fsong->hStep.nSizeBeatSplit*sizeof(int));
	fs.write((char*)fsong->nIChangeVel,	fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.write((char*)fsong->nEChangeVel,	fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.write((char*)fsong->dChanges,		fsong->hStep.nSizeChangeVel*sizeof(double));
	fs.write((char*)fsong->nIJumps,		fsong->hStep.nSizeJumps*sizeof(int));
	fs.write((char*)fsong->nEJumps,		fsong->hStep.nSizeJumps*sizeof(int));
	fs.write((char*)fsong->nWTickCount,	fsong->hStep.nSizeTickcount*sizeof(int));
	fs.write((char*)fsong->nTickCount,	fsong->hStep.nSizeTickcount*sizeof(int));
	fs.write((char*)fsong->nWStops,		fsong->hStep.nSizeStops*sizeof(int));
	fs.write((char*)fsong->dStops,		fsong->hStep.nSizeStops*sizeof(double));

	fs.close();
	return 1;
}

int CMaxDocument::OpenRAW_SKSF2(LPCTSTR strFile, STEP* fsong)
{
	if(fsong == NULL) return 0;
	ifstream fs;
	fs.open(strFile, ios::in | ios::binary);
	if(!fs.is_open()) return 0;

	fs.read((char*)&fsong->hStep, sizeof(STEPHEADER));
	if(fsong->hStep.dwVersion != 0x20)
	{
		fs.close();
		return 0;	// No signature
	}

	Init_SKSF2(fsong);

	fs.read((char*)fsong->fAttrib, fsong->hStep.nSizeAttrib*sizeof(float));

	fs.read((char*)fsong->nWChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(int));
	fs.read((char*)fsong->fChangeBPM, fsong->hStep.nSizeChangeBPM*sizeof(float));

	fs.read((char*)fsong->nWBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));
	fs.read((char*)fsong->nBeatSplit, fsong->hStep.nSizeBeatSplit*sizeof(int));

	fs.read((char*)fsong->nIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.read((char*)fsong->nEChangeVel, fsong->hStep.nSizeChangeVel*sizeof(int));
	fs.read((char*)fsong->fChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));
	fs.read((char*)fsong->fIChangeVel, fsong->hStep.nSizeChangeVel*sizeof(float));

	fs.read((char*)fsong->nIJumps, fsong->hStep.nSizeJumps*sizeof(int));
	fs.read((char*)fsong->nEJumps, fsong->hStep.nSizeJumps*sizeof(int));

	fs.read((char*)fsong->nWTickCount, fsong->hStep.nSizeTickCount*sizeof(int));
	fs.read((char*)fsong->nTickCount, fsong->hStep.nSizeTickCount*sizeof(int));

	fs.read((char*)fsong->nIStops, fsong->hStep.nSizeStops*sizeof(int));
	fs.read((char*)fsong->nEStops, fsong->hStep.nSizeStops*sizeof(int));

	for(int i = 0; i < fsong->hStep.nSizeArrowGroups; i++)
	{
		fs.read((char*)&fsong->hStepGroup[i].hGroup, sizeof(STEPGROUPHEADER));
		ReInit_SKSF2(fsong);
		fs.read((char*)fsong->hStepGroup[i].nArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
		fs.read((char*)fsong->hStepGroup[i].nPArrows, fsong->hStepGroup[i].hGroup.nSizeArrows*fsong->hStep.nArrowsPerLine*sizeof(int));
	}
	for(int i = 0; i < fsong->hStep.nSizeMod; i++)
	{
		fs.read((char*)&fsong->hMod[i].hMod, sizeof(STEPMODHEADER));
		ReInit_SKSF2(fsong);
		for(int j = 0; j < fsong->hMod[i].hMod.nSize; j++)
		{
			fs.read((char*)fsong->hMod[i].hModInfo[j].nModInt, fsong->hMod[i].hMod.nSizeInt*sizeof(int));
			fs.read((char*)fsong->hMod[i].hModInfo[j].fModFloat, fsong->hMod[i].hMod.nSizeFloat*sizeof(float));
		}
	}

	fs.close();
	return 1;
}

int CMaxDocument::OpenRAW_SKSF1(LPCTSTR strFile, SKSFv1* fsong)
{
    memset(&fsong->hStep, 0, sizeof(SKSFv1HEADER));
	Init_SKSF1(fsong);

	ifstream fs;
	fs.open(strFile, ios::in | ios::binary);
	if(!fs.is_open()) return 0;

	fs.read((char*)&fsong->hStep, sizeof(SKSFv1HEADER));
	ReInit_SKSF1(fsong);
	if(fsong->dwArrows1 != NULL) fs.read((char*)fsong->dwArrows1,		fsong->hStep.nSizeArrows1*sizeof(uint32_t)*10);
	if(fsong->dwArrows2 != NULL) fs.read((char*)fsong->dwArrows2,		fsong->hStep.nSizeArrows2*sizeof(uint32_t)*10);
	if(fsong->dwArrows3 != NULL) fs.read((char*)fsong->dwArrows3,		fsong->hStep.nSizeArrows3*sizeof(uint32_t)*10);
	if(fsong->nWChangeBPM != NULL) fs.read((char*)fsong->nWChangeBPM,	fsong->hStep.nSizeChangeBPM*sizeof(int));
	if(fsong->dChangeBPM != NULL) fs.read((char*)fsong->dChangeBPM,	fsong->hStep.nSizeChangeBPM*sizeof(double));
	if(fsong->nWBeatSplit != NULL) fs.read((char*)fsong->nWBeatSplit,	fsong->hStep.nSizeBeatSplit*sizeof(int));
	if(fsong->nBeatSplit != NULL) fs.read((char*)fsong->nBeatSplit,	fsong->hStep.nSizeBeatSplit*sizeof(int));
	if(fsong->nIChangeVel != NULL) fs.read((char*)fsong->nIChangeVel,	fsong->hStep.nSizeChangeVel*sizeof(int));
	if(fsong->nEChangeVel != NULL) fs.read((char*)fsong->nEChangeVel,	fsong->hStep.nSizeChangeVel*sizeof(int));
	if(fsong->dChanges != NULL) fs.read((char*)fsong->dChanges,		fsong->hStep.nSizeChangeVel*sizeof(double));
	if(fsong->nIJumps != NULL) fs.read((char*)fsong->nIJumps,		fsong->hStep.nSizeJumps*sizeof(int));
	if(fsong->nEJumps != NULL) fs.read((char*)fsong->nEJumps,		fsong->hStep.nSizeJumps*sizeof(int));
	if(fsong->nWTickCount != NULL) fs.read((char*)fsong->nWTickCount,	fsong->hStep.nSizeTickcount*sizeof(int));
	if(fsong->nTickCount != NULL) fs.read((char*)fsong->nTickCount,	fsong->hStep.nSizeTickcount*sizeof(int));
	if(fsong->nWStops != NULL) fs.read((char*)fsong->nWStops,		fsong->hStep.nSizeStops*sizeof(int));
	if(fsong->dStops != NULL) fs.read((char*)fsong->dStops,		fsong->hStep.nSizeStops*sizeof(double));

	fs.close();
	return 1;
}

int CMaxDocument::Save(void)
{
    m_bSaved = true;

	Fix();
	if(m_strFile.Find(MAIN_DOCUMENT_FILTER_EXTENSION) != -1)
	{
		SaveRAW_SKSF2(m_strFile, &song);
		m_bFiled = true;
		m_bSaved = true;
	}
	else if(m_strFile.Find(SKSF_DOCUMENT_FILTER_EXTENSION) != -1)
	{
		SKSFv1 sksf;
		Convert_SKSFv2_SKSFv1(&sksf, &song);
		SaveRAW_SKSF1(m_strFile, &sksf);
		Delete_SKSF1(&sksf);
		m_bFiled = false;
		m_bSaved = false;
	}
	else if(m_strFile.Find(UCS_DOCUMENT_FILTER_EXTENSION) != -1)
	{
		Save_UCS_SKSF2(m_strFile, &song);
		m_bFiled = false;
		m_bSaved = false;
	}


    return 1;
}

int CMaxDocument::Save(LPCTSTR strFile)
{
    m_strFile = strFile;
    m_strName = m_strFile.Right(m_strFile.GetLength() - max(m_strFile.ReverseFind('/'), m_strFile.ReverseFind('\\')) - 1);
	int nExt = m_strName.ReverseFind('.');
	if(nExt != -1) m_strName = m_strName.Left(nExt);

    // ** Only save the document using the same function
    Save();
    return 1;
}

int CMaxDocument::NewInit(void)
{
	// Almenos tiene que haber un cambio de BPM, un
	// cambio de velocidad, un cambio de tickcount
	// y un cambio de BeatSplit, los pondrémos todos por defecto
	// BPM = 120, Velocity = 1.0x, Tickcount = 1, BeatSplit = 1
	InitArrowGruops(song.hStep.nSizeArrowGroups, 1);

	song.hStep.fBPM = 120.0f;
	song.hStep.nBeatSplit = 1;
	song.hStep.fVel = 1.0f;
	song.hStep.nTickCount = 1;

	// IMPORTANTE: Como este editor va a regalarse, debemos
	// cuidar de que la PIU Max no sea hackeada, por tanto
	// se escribe la version del archivo aca
	song.hStep.dwVersion = 0x20;	//2.0

	ResizeSteps(0, 50);
	return 1;
}

float CMaxDocument::BeatArrowEdit(int nArrow)
{
	return BeatArrowEdit(&song, nArrow);
}

float CMaxDocument::BeatArrowEdit(STEP* fsong, int nArrow)
{
	float fBeat = 0.0;
	int nBS = fsong->hStep.nBeatSplit;
	int nWBS = 0;
	for(int x = 0; x < fsong->hStep.nSizeBeatSplit; x++)
	{
		if(nArrow <= fsong->nWBeatSplit[x])
		{
			break;
		}
		else
		{
			fBeat += ((float)(fsong->nWBeatSplit[x]-nWBS))*
				(1.0f/((float)nBS));
			nWBS = fsong->nWBeatSplit[x];
			nBS = fsong->nBeatSplit[x];
		}
	}

	fBeat += ((float)(nArrow-nWBS))*(1/((float)nBS));
	return fBeat;
}

float CMaxDocument::BeatArrowDraw(int nArrow)
{
	return BeatArrowDraw(BeatArrowEdit(nArrow));
}

float CMaxDocument::BeatArrowDraw(STEP* fsong, int nArrow)
{
	return BeatArrowDraw(fsong, BeatArrowEdit(fsong, nArrow));
}

float CMaxDocument::BeatArrowDraw(float dBeatEdit)
{
	float fBeat = dBeatEdit;
	for(int x = 0; x < song.hStep.nSizeStops; x++)
	{
		if(dBeatEdit <= hAux.fIStops[x])
		{
			break;
		}
		else if(dBeatEdit > hAux.fIStops[x]
			&& dBeatEdit <= hAux.fEStops[x])
		{
			fBeat -= dBeatEdit - hAux.fIStops[x];
			break;
		}
		else
		{
			fBeat -= hAux.fEStops[x]-hAux.fIStops[x];
		}
	}
	return fBeat;
}

float CMaxDocument::BeatArrowDraw(STEP* fsong, float dBeatEdit)
{
	float fBeat = dBeatEdit;
	for(int x = 0; x < fsong->hStep.nSizeStops; x++)
	{
		float fIStop = BeatArrowEdit(fsong, fsong->nIStops[x]);
		float fEStop = BeatArrowEdit(fsong, fsong->nIStops[x]);
		if(dBeatEdit <= fIStop)
		{
			break;
		}
		else if(dBeatEdit > fIStop
			&& dBeatEdit <= fEStop)
		{
			fBeat -= dBeatEdit - fIStop;
			break;
		}
		else
		{
			fBeat -= fEStop-fIStop;
		}
	}
	return fBeat;
}

float CMaxDocument::BeatArrowTime(int nArrow)
{
	return BeatArrowTime(BeatArrowEdit(nArrow));
}

float CMaxDocument::BeatArrowTime(STEP* fsong, int nArrow)
{
	return BeatArrowTime(fsong, BeatArrowEdit(fsong, nArrow));
}

float CMaxDocument::BeatArrowTime(float dBeatEdit)
{
	float fBeat = dBeatEdit;
	for(int x = 0; x < song.hStep.nSizeJumps; x++)
	{
		if(dBeatEdit <= hAux.fIJumps[x])
		{
			break;
		}
		else if(dBeatEdit > hAux.fIJumps[x]
			&& dBeatEdit <= hAux.fEJumps[x])
		{
			fBeat -= dBeatEdit - hAux.fIJumps[x];
			break;
		}
		else
		{
			fBeat -= hAux.fEJumps[x]-hAux.fIJumps[x];
		}
	}
	return fBeat;
}

float CMaxDocument::BeatArrowTime(STEP* fsong, float dBeatEdit)
{
	float fBeat = dBeatEdit;
	for(int x = 0; x < song.hStep.nSizeJumps; x++)
	{
		float fIJump = BeatArrowEdit(fsong, fsong->nIJumps[x]);
		float fEJump = BeatArrowEdit(fsong, fsong->nIJumps[x]);
		if(dBeatEdit <= hAux.fIJumps[x])
		{
			break;
		}
		else if(dBeatEdit > fIJump
			&& dBeatEdit <= fEJump)
		{
			fBeat -= dBeatEdit - fIJump;
			break;
		}
		else
		{
			fBeat -= fEJump-fIJump;
		}
	}
	return fBeat;
}

int CMaxDocument::ArrowBeatEdit(float fBeat)
{
	int nArrow = 0;
	int nBS = song.hStep.nBeatSplit;
	float fWBS = 0;
	float fWBeatSplit = 0;
	for(int x = 0; x < song.hStep.nSizeBeatSplit; x++)
	{
		fWBeatSplit = hAux.fWBeatSplit[x];
		if(fBeat <= fWBeatSplit)
		{
			break;
		}
		else
		{
			nArrow = song.nWBeatSplit[x];
			fWBS = fWBeatSplit;
			nBS = song.nBeatSplit[x];
		}
	}

	nArrow += int((fBeat-fWBS)*float(nBS));
	return nArrow;
}

int CMaxDocument::Actualizate(void)
{
	Reinit();
	for(int y = 0; y < song.hStep.nSizeArrowGroups; y++)
	{
		hGroupAux[y].fLimitArrows = BeatArrowEdit(song.hStepGroup[y].hGroup.nSizeArrows);
	}
	for(int x = 0; x < song.hStep.nSizeBeatSplit; x++)
	{
		hAux.fWBeatSplit[x] = BeatArrowEdit(song.nWBeatSplit[x]);
	}
	for(int x = 0; x < song.hStep.nSizeChangeBPM; x++)
	{
		hAux.fWChangeBPM[x] = BeatArrowEdit(song.nWChangeBPM[x]);
	}
	for(int x = 0; x < song.hStep.nSizeChangeVel; x++)
	{
		hAux.fIChangeVel[x] = BeatArrowEdit(song.nIChangeVel[x]);
		hAux.fEChangeVel[x] = BeatArrowEdit(song.nEChangeVel[x]);
	}
	for(int x = 0; x < song.hStep.nSizeJumps; x++)
	{
		hAux.fIJumps[x] = BeatArrowEdit(song.nIJumps[x]);
		hAux.fEJumps[x] = BeatArrowEdit(song.nEJumps[x]);
	}
	for(int x = 0; x < song.hStep.nSizeTickCount; x++)
	{
		hAux.fWTickCount[x] = BeatArrowEdit(song.nWTickCount[x]);
	}
	for(int x = 0; x < song.hStep.nSizeStops; x++)
	{
		hAux.fIStops[x] = BeatArrowEdit(song.nIStops[x]);
		hAux.fEStops[x] = BeatArrowEdit(song.nEStops[x]);
	}
	return 1;
}

int CMaxDocument::GetArrow(int x, int y, int nGArrow)
{
	int nRel;
	nRel = y*m_nArrowsPerLine+x;
	if(nGArrow < 0) return 0;
	if(nGArrow >= song.hStep.nSizeArrowGroups) return 0;
	if(nRel < 0) return 0;
	if(nRel >= (song.hStepGroup[nGArrow].hGroup.nSizeArrows*m_nArrowsPerLine)) return 0;
	return song.hStepGroup[nGArrow].nArrows[nRel];
}

int CMaxDocument::GetArrowPar(int x, int y, int nGArrow)
{
	int nRel;
	nRel = y*m_nArrowsPerLine+x;
	if(nGArrow < 0) return 0;
	if(nGArrow >= song.hStep.nSizeArrowGroups) return 0;
	if(nRel < 0) return 0;
	if(nRel >= (song.hStepGroup[nGArrow].hGroup.nSizeArrows*m_nArrowsPerLine)) return 0;
	return song.hStepGroup[nGArrow].nPArrows[nRel];
}

void CMaxDocument::SetArrow(int x, int y, int nGArrow, int nType, int nPar)
{
	int nRel;
	nRel = y*m_nArrowsPerLine+x;
	if(nGArrow < 0) return;
	if(nGArrow >= song.hStep.nSizeArrowGroups) return;
	if(nRel < 0) return;
	if(nRel >= (song.hStepGroup[nGArrow].hGroup.nSizeArrows*m_nArrowsPerLine)) return;
	song.hStepGroup[nGArrow].nArrows[nRel] = nType;
	song.hStepGroup[nGArrow].nPArrows[nRel] = nPar;
}

void CMaxDocument::AddMod(int nMod, int* nInt, int nSizeInt, float* fFloat, int nSizeFloat)
{
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		if(nSizeInt>=2 && nSizeFloat>=0)
		{
			int n = song.hStep.nSizeBeatSplit;
			song.hStep.nSizeBeatSplit++;
			song.nWBeatSplit=(int*)max_realloc(song.nWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int));
			song.nBeatSplit=(int*)max_realloc(song.nBeatSplit, song.hStep.nSizeBeatSplit*sizeof(float));
			song.nWBeatSplit[n] = nInt[0];
			song.nBeatSplit[n] = nInt[1];
		}
		break;
	case MAXSTEP_CHANGEBPM:
		if(nSizeInt>=1 && nSizeFloat>=1)
		{
			int n = song.hStep.nSizeChangeBPM;
			song.hStep.nSizeChangeBPM++;
			song.nWChangeBPM=(int*)max_realloc(song.nWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(int));
			song.fChangeBPM=(float*)max_realloc(song.fChangeBPM, song.hStep.nSizeChangeBPM*sizeof(float));
			song.nWChangeBPM[n] = nInt[0];
			song.fChangeBPM[n] = fFloat[0];
		}
		break;
	case MAXSTEP_CHANGEVEL:
		if(nSizeInt>=2 && nSizeFloat>=2)
		{
			int n = song.hStep.nSizeChangeVel;
			song.hStep.nSizeChangeVel++;
			song.nIChangeVel=(int*)max_realloc(song.nIChangeVel, song.hStep.nSizeChangeVel*sizeof(int));
			song.nEChangeVel=(int*)max_realloc(song.nEChangeVel, song.hStep.nSizeChangeVel*sizeof(int));
			song.fChangeVel=(float*)max_realloc(song.fChangeVel, song.hStep.nSizeChangeVel*sizeof(float));
			song.fIChangeVel=(float*)max_realloc(song.fIChangeVel, song.hStep.nSizeChangeVel*sizeof(float));
			song.nIChangeVel[n] = nInt[0];
			song.nEChangeVel[n] = nInt[1];
			song.fChangeVel[n] = fFloat[0];
			song.fIChangeVel[n] = fFloat[1];
		}
		break;
	case MAXSTEP_JUMP:
		if(nSizeInt>=2 && nSizeFloat>=0)
		{
			int n = song.hStep.nSizeJumps;
			song.hStep.nSizeJumps++;
			song.nIJumps=(int*)max_realloc(song.nIJumps, song.hStep.nSizeJumps*sizeof(int));
			song.nEJumps=(int*)max_realloc(song.nEJumps, song.hStep.nSizeJumps*sizeof(int));
			song.nIJumps[n] = nInt[0];
			song.nEJumps[n] = nInt[1];
		}
		break;
	case MAXSTEP_TICKCOUNT:
		if(nSizeInt>=2 && nSizeFloat>=0)
		{
			int n = song.hStep.nSizeTickCount;
			song.hStep.nSizeTickCount++;
			song.nWTickCount=(int*)max_realloc(song.nWTickCount, song.hStep.nSizeTickCount*sizeof(int));
			song.nTickCount=(int*)max_realloc(song.nTickCount, song.hStep.nSizeTickCount*sizeof(int));
			song.nWTickCount[n] = nInt[0];
			song.nTickCount[n] = nInt[1];
		}
		break;
	case MAXSTEP_STOP:
		if(nSizeInt>=2 && nSizeFloat>=0)
		{
			int n = song.hStep.nSizeStops;
			song.hStep.nSizeStops++;
			song.nIStops=(int*)max_realloc(song.nIStops, song.hStep.nSizeStops*sizeof(int));
			song.nEStops=(int*)max_realloc(song.nEStops, song.hStep.nSizeStops*sizeof(int));
			song.nIStops[n] = nInt[0];
			song.nEStops[n] = nInt[1];
		}
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				if(nSizeInt >= song.hMod[nIndex].hMod.nSizeInt && nSizeFloat >= song.hMod[nIndex].hMod.nSizeFloat)
				{
					int i = nIndex;
					int n = song.hMod[i].hMod.nSize;
					song.hMod[i].hMod.nSize++;
					song.hMod[i].hModInfo=(STEPMODINFO*)max_realloc(song.hMod[i].hModInfo, song.hMod[i].hMod.nSize*sizeof(STEPMOD));
					song.hMod[i].hModInfo[n].nModInt=(int*)max_realloc(song.hMod[i].hModInfo[n].nModInt, song.hMod[i].hMod.nSizeInt*sizeof(int));
					song.hMod[i].hModInfo[n].fModFloat=(float*)max_realloc(song.hMod[i].hModInfo[n].fModFloat, song.hMod[i].hMod.nSizeFloat*sizeof(float));
					memcpy(song.hMod[i].hModInfo[n].nModInt, nInt, song.hMod[i].hMod.nSizeInt*sizeof(int));
					memcpy(song.hMod[i].hModInfo[n].fModFloat, fFloat, song.hMod[i].hMod.nSizeFloat*sizeof(int));
				}
			}
		}
		break;
	}
	Fix();
	Actualizate();
}

void CMaxDocument::EditMod(int nMod, int y, int* nInt, int nSizeInt, float* fFloat, int nSizeFloat)
{
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		if(nSizeInt>=2 && nSizeFloat>=0 && y < song.hStep.nSizeBeatSplit)
		{
			int n = y;
			if(n == -1)
			{
				song.hStep.nBeatSplit = nInt[1];
			}
			else if(n >= 0)
			{
				song.nWBeatSplit[n] = nInt[0];
				song.nBeatSplit[n] = nInt[1];
			}
		}
		break;
	case MAXSTEP_CHANGEBPM:
		if(nSizeInt>=1 && nSizeFloat>=1 && y < song.hStep.nSizeChangeBPM)
		{
			int n = y;
			if(n == -1)
			{
				song.hStep.fBPM = fFloat[0];
			}
			else if(n >= 0)
			{
				song.nWChangeBPM[n] = nInt[0];
				song.fChangeBPM[n] = fFloat[0];
			}
		}
		break;
	case MAXSTEP_CHANGEVEL:
		if(nSizeInt>=2 && nSizeFloat>=2 && y < song.hStep.nSizeChangeVel)
		{
			int n = y;
			if(n == -1)
			{
				song.hStep.fVel = fFloat[0];
			}
			else if(n >= 0)
			{
				song.nIChangeVel[n] = nInt[0];
				song.nEChangeVel[n] = nInt[1];
				song.fChangeVel[n] = fFloat[0];
				song.fIChangeVel[n] = fFloat[1];
			}
		}
		break;
	case MAXSTEP_JUMP:
		if(nSizeInt>=2 && nSizeFloat>=0 && y < song.hStep.nSizeJumps)
		{
			int n = y;
			song.nIJumps[n] = nInt[0];
			song.nEJumps[n] = nInt[1];
		}
		break;
	case MAXSTEP_TICKCOUNT:
		if(nSizeInt>=2 && nSizeFloat>=0 && y < song.hStep.nSizeTickCount)
		{
			int n = y;
			if(n == -1)
			{
				song.hStep.nTickCount = nInt[1];
			}
			else if(n >= 0)
			{
				song.nWTickCount[n] = nInt[0];
				song.nTickCount[n] = nInt[1];
			}
		}
		break;
	case MAXSTEP_STOP:
		if(nSizeInt>=2 && nSizeFloat>=0 && y < song.hStep.nSizeStops)
		{
			int n = y;
			song.nIStops[n] = nInt[0];
			song.nEStops[n] = nInt[1];
		}
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				if(nSizeInt >= song.hMod[nIndex].hMod.nSizeInt && nSizeFloat >= song.hMod[nIndex].hMod.nSizeFloat && y < song.hMod[nIndex].hMod.nSize)
				{
					int n = y;
					int i = nIndex;
					memcpy(song.hMod[i].hModInfo[n].nModInt, nInt, song.hMod[i].hMod.nSizeInt*sizeof(int));
					memcpy(song.hMod[i].hModInfo[n].fModFloat, fFloat, song.hMod[i].hMod.nSizeFloat*sizeof(int));
				}
			}
		}
		break;
	}
	Fix();
	Actualizate();
}

void CMaxDocument::DelMod(int nMod, int y)
{
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		if(y < song.hStep.nSizeBeatSplit)
		{
			for(int n = y; n < (song.hStep.nSizeBeatSplit-1); n++)
			{
				song.nWBeatSplit[n] = song.nWBeatSplit[n+1];
				song.nBeatSplit[n] = song.nBeatSplit[n+1];
			}
			song.hStep.nSizeBeatSplit--;
			song.nWBeatSplit=(int*)max_realloc(song.nWBeatSplit, song.hStep.nSizeBeatSplit*sizeof(int));
			song.nBeatSplit=(int*)max_realloc(song.nBeatSplit, song.hStep.nSizeBeatSplit*sizeof(float));
		}
		break;
	case MAXSTEP_CHANGEBPM:
		if(y < song.hStep.nSizeChangeBPM)
		{
			for(int n = y; n < (song.hStep.nSizeChangeBPM-1); n++)
			{
				song.nWChangeBPM[n] = song.nWChangeBPM[n+1];
				song.fChangeBPM[n] = song.fChangeBPM[n+1];
			}
			song.hStep.nSizeChangeBPM--;
			song.nWChangeBPM=(int*)max_realloc(song.nWChangeBPM, song.hStep.nSizeChangeBPM*sizeof(int));
			song.fChangeBPM=(float*)max_realloc(song.fChangeBPM, song.hStep.nSizeChangeBPM*sizeof(float));
		}
		break;
	case MAXSTEP_CHANGEVEL:
		if(y < song.hStep.nSizeChangeVel)
		{
			for(int n = y; n < (song.hStep.nSizeChangeVel-1); n++)
			{
				song.nIChangeVel[n] = song.nIChangeVel[n+1];
				song.nEChangeVel[n] = song.nEChangeVel[n+1];
				song.fChangeVel[n] = song.fChangeVel[n+1];
				song.fIChangeVel[n] = song.fIChangeVel[n+1];
			}
			song.hStep.nSizeChangeVel--;
			song.nIChangeVel=(int*)max_realloc(song.nIChangeVel, song.hStep.nSizeChangeVel*sizeof(int));
			song.nEChangeVel=(int*)max_realloc(song.nEChangeVel, song.hStep.nSizeChangeVel*sizeof(int));
			song.fChangeVel=(float*)max_realloc(song.fChangeVel, song.hStep.nSizeChangeVel*sizeof(float));
			song.fIChangeVel=(float*)max_realloc(song.fIChangeVel, song.hStep.nSizeChangeVel*sizeof(float));
		}
		break;
	case MAXSTEP_JUMP:
		if(y < song.hStep.nSizeJumps)
		{
			for(int n = y; n < (song.hStep.nSizeJumps-1); n++)
			{
				song.nIJumps[n] = song.nIJumps[n+1];
				song.nEJumps[n] = song.nEJumps[n+1];
			}
			song.hStep.nSizeJumps--;
			song.nIJumps=(int*)max_realloc(song.nIJumps, song.hStep.nSizeJumps*sizeof(int));
			song.nEJumps=(int*)max_realloc(song.nEJumps, song.hStep.nSizeJumps*sizeof(int));
		}
		break;
	case MAXSTEP_TICKCOUNT:
		if(y < song.hStep.nSizeTickCount)
		{
			for(int n = y; n < (song.hStep.nSizeTickCount-1); n++)
			{
				song.nWTickCount[n] = song.nWTickCount[n+1];
				song.nTickCount[n] = song.nTickCount[n+1];
			}
			song.hStep.nSizeTickCount--;
			song.nWTickCount=(int*)max_realloc(song.nWTickCount, song.hStep.nSizeTickCount*sizeof(int));
			song.nTickCount=(int*)max_realloc(song.nTickCount, song.hStep.nSizeTickCount*sizeof(int));
		}
		break;
	case MAXSTEP_STOP:
		if(y < song.hStep.nSizeStops)
		{
			for(int n = y; n < (song.hStep.nSizeStops-1); n++)
			{
				song.nIStops[n] = song.nIStops[n+1];
				song.nEStops[n] = song.nEStops[n+1];
			}
			song.hStep.nSizeStops--;
			song.nIStops=(int*)max_realloc(song.nIStops, song.hStep.nSizeStops*sizeof(int));
			song.nEStops=(int*)max_realloc(song.nEStops, song.hStep.nSizeStops*sizeof(int));
		}
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				int i = nIndex;
				if(y < song.hMod[i].hMod.nSize)
				{
					int n;
					for(n = y; n < (song.hMod[i].hMod.nSize-1); n++)
					{
						memcpy(song.hMod[i].hModInfo[n].nModInt, song.hMod[i].hModInfo[n+1].nModInt, song.hMod[i].hMod.nSizeInt*sizeof(int));
						memcpy(song.hMod[i].hModInfo[n].fModFloat, song.hMod[i].hModInfo[n+1].fModFloat, song.hMod[i].hMod.nSizeFloat*sizeof(int));
					}
					n = song.hMod[i].hMod.nSize;
					delete song.hMod[i].hModInfo[n].nModInt;
					delete song.hMod[i].hModInfo[n].fModFloat;
					song.hMod[i].hMod.nSize--;
					song.hMod[i].hModInfo=(STEPMODINFO*)max_realloc(song.hMod[i].hModInfo, song.hMod[i].hMod.nSize*sizeof(STEPMOD));
				}
			}
		}
		break;
	}
	Fix();
	Actualizate();
}

void CMaxDocument::GetMod(int nMod, int y, int** nInt, float** fFloat)
{
	(*nInt) = NULL;
	(*fFloat) = NULL;
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		if(y < song.hStep.nSizeBeatSplit)
		{
			int n = y;
			(*nInt) = (int*)malloc(2*sizeof(int));
			if(n == -1)
			{
				(*nInt)[0] = 0;
				(*nInt)[1] = song.hStep.nBeatSplit;
			}
			else if(n >= 0)
			{
				(*nInt)[0] = song.nWBeatSplit[n];
				(*nInt)[1] = song.nBeatSplit[n];
			}
		}
		break;
	case MAXSTEP_CHANGEBPM:
		if(y < song.hStep.nSizeChangeBPM)
		{
			int n = y;
			(*nInt) = (int*)malloc(1*sizeof(int));
			(*fFloat) = (float*)malloc(1*sizeof(float));
			if(n == -1)
			{
				(*nInt)[0] = 0;
				(*fFloat)[0] = song.hStep.fBPM;
			}
			else if(n >= 0)
			{
				(*nInt)[0] = song.nWChangeBPM[n];
				(*fFloat)[0] = song.fChangeBPM[n];
			}
		}
		break;
	case MAXSTEP_CHANGEVEL:
		if(y < song.hStep.nSizeChangeVel)
		{
			int n = y;
			(*nInt) = (int*)malloc(2*sizeof(int));
			(*fFloat) = (float*)malloc(2*sizeof(float));
			if(n == -1)
			{
				(*nInt)[0] = 0;
				(*nInt)[1] = 0;
				(*fFloat)[0] = song.hStep.fVel;
				(*fFloat)[1] = song.hStep.fVel;
			}
			else if(n >= 0)
			{
				(*nInt)[0] = song.nIChangeVel[n];
				(*nInt)[1] = song.nEChangeVel[n];
				(*fFloat)[0] = song.fChangeVel[n];
				(*fFloat)[1] = song.fIChangeVel[n];
			}
		}
		break;
	case MAXSTEP_JUMP:
		if(y < song.hStep.nSizeJumps)
		{
			int n = y;
			(*nInt) = (int*)malloc(2*sizeof(int));
			(*nInt)[0] = song.nIJumps[n];
			(*nInt)[1] = song.nEJumps[n];
		}
		break;
	case MAXSTEP_TICKCOUNT:
		if(y < song.hStep.nSizeTickCount)
		{
			int n = y;
			(*nInt) = (int*)malloc(2*sizeof(int));
			if(n == -1)
			{
				(*nInt)[0] = 0;
				(*nInt)[1] = song.hStep.nTickCount;
			}
			else if(n >= 0)
			{
				(*nInt)[0] = song.nWTickCount[n];
				(*nInt)[1] = song.nTickCount[n];
			}
		}
		break;
	case MAXSTEP_STOP:
		if(y < song.hStep.nSizeStops)
		{
			int n = y;
			(*nInt) = (int*)malloc(2*sizeof(int));
			(*nInt)[0] = song.nIStops[n];
			(*nInt)[1] = song.nEStops[n];
		}
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				int nSizeInt = song.hMod[nIndex].hMod.nSizeInt;
				int nSizeFloat = song.hMod[nIndex].hMod.nSizeFloat;
				if(y < song.hMod[nIndex].hMod.nSize)
				{
					if(nSizeInt > 0) (*nInt) = (int*)malloc(nSizeInt*sizeof(int));
					if(nSizeFloat > 0) (*fFloat) = (float*)malloc(nSizeFloat*sizeof(float));
					int n = y;
					int i = nIndex;
					memcpy(*nInt, song.hMod[i].hModInfo[n].nModInt, song.hMod[i].hMod.nSizeInt*sizeof(int));
					memcpy(*fFloat, song.hMod[i].hModInfo[n].fModFloat, song.hMod[i].hMod.nSizeFloat*sizeof(int));
				}
			}
		}
		break;
	}
}

void CMaxDocument::GetParMod(int nMod, int* nSize, int* nSizeInt, int* nSizeFloat)
{
	(*nSizeInt) = 0;
	(*nSizeFloat) = 0;
	(*nSize) = 0;
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		(*nSizeInt) = 2;
		(*nSizeFloat) = 0;
		(*nSize) = song.hStep.nSizeBeatSplit;
		break;
	case MAXSTEP_CHANGEBPM:
		(*nSizeInt) = 1;
		(*nSizeFloat) = 1;
		(*nSize) = song.hStep.nSizeChangeBPM;
		break;
	case MAXSTEP_CHANGEVEL:
		(*nSizeInt) = 2;
		(*nSizeFloat) = 2;
		(*nSize) = song.hStep.nSizeChangeVel;
		break;
	case MAXSTEP_JUMP:
		(*nSizeInt) = 2;
		(*nSizeFloat) = 0;
		(*nSize) = song.hStep.nSizeJumps;
		break;
	case MAXSTEP_TICKCOUNT:
		(*nSizeInt) = 2;
		(*nSizeFloat) = 0;
		(*nSize) = song.hStep.nSizeTickCount;
		break;
	case MAXSTEP_STOP:
		(*nSizeInt) = 2;
		(*nSizeFloat) = 0;
		(*nSize) = song.hStep.nSizeStops;
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				(*nSizeInt) = song.hMod[nIndex].hMod.nSizeInt;
				(*nSizeFloat) = song.hMod[nIndex].hMod.nSizeFloat;
				(*nSize) = song.hMod[nIndex].hMod.nSize;
			}
		}
		break;
	}
}

void CMaxDocument::FreeMod(int** nInt, float** fFloat)
{
	if((*nInt) != NULL)
		free(*nInt);
	if((*fFloat) != NULL)
		free(*fFloat);
}

void CMaxDocument::GetModWhere(int nMod, int* nIWhere, int* nEWhere)
{
	(*nIWhere) = -1;
	(*nEWhere) = -1;
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		(*nIWhere) = 0;
		(*nEWhere) = 0;
		break;
	case MAXSTEP_CHANGEBPM:
		(*nIWhere) = 0;
		(*nEWhere) = 0;
		break;
	case MAXSTEP_CHANGEVEL:
		(*nIWhere) = 0;
		(*nEWhere) = 1;
		break;
	case MAXSTEP_JUMP:
		(*nIWhere) = 0;
		(*nEWhere) = 1;
		break;
	case MAXSTEP_TICKCOUNT:
		(*nIWhere) = 0;
		(*nEWhere) = 0;
		break;
	case MAXSTEP_STOP:
		(*nIWhere) = 0;
		(*nEWhere) = 1;
		break;
	default:
		{
			int nIndex = -1;
			for(int k = 0; k < song.hStep.nSizeMod; k++)
			{
				if(nMod == song.hMod[k].hMod.nType)
				{
					nIndex = k;
					break;
				}
			}
			if(nIndex != -1)
			{
				// USER: invoke "ModWhere" in Python script
				(*nIWhere) = 0;
				(*nEWhere) = 0;
			}
		}
		break;
	}
}

void CMaxDocument::AddGroup(void)
{
	InitArrowGruops(song.hStep.nSizeArrowGroups, song.hStep.nSizeArrowGroups+1);
}

void CMaxDocument::DelGroup(int nGroup)
{
	if(song.hStep.nSizeArrowGroups<1) return;	// Can't do this if there is no groups
	STEPGROUP hStepGroup = song.hStepGroup[nGroup];
	int i;
	for(i = nGroup; i < (song.hStep.nSizeArrowGroups-1); i++)
	{
		song.hStepGroup[i].hGroup = song.hStepGroup[i+1].hGroup;
		song.hStepGroup[i].nArrows = song.hStepGroup[i+1].nArrows;
		song.hStepGroup[i].nPArrows = song.hStepGroup[i+1].nPArrows;
	}
	i = song.hStep.nSizeArrowGroups-1;
	song.hStepGroup[i].hGroup = hStepGroup.hGroup;
	song.hStepGroup[i].nArrows = hStepGroup.nArrows;
	song.hStepGroup[i].nPArrows = hStepGroup.nPArrows;
	InitArrowGruops(song.hStep.nSizeArrowGroups, song.hStep.nSizeArrowGroups-1);
}

void CMaxDocument::Fix(void)
{
	Fix(&song);
}

void CMaxDocument::Fix(STEP *fsong)
{
	for(int y = 0; y < fsong->hStep.nSizeChangeBPM; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeChangeBPM-1); x++)
		{
			if(fsong->nWChangeBPM[x] > fsong->nWChangeBPM[x+1])
			{
				int nTemp = fsong->nWChangeBPM[x];
				float dTemp = fsong->fChangeBPM[x];
				fsong->nWChangeBPM[x] = fsong->nWChangeBPM[x+1];
				fsong->fChangeBPM[x] = fsong->fChangeBPM[x+1];
				fsong->nWChangeBPM[x+1] = nTemp;
				fsong->fChangeBPM[x+1] = dTemp;
			}
		}
	}
	for(int y = 0; y < fsong->hStep.nSizeTickCount; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeTickCount-1); x++)
		{
			if(fsong->nWTickCount[x] > fsong->nWTickCount[x+1])
			{
				int nTemp1 = fsong->nWTickCount[x];
				int dTemp2 = fsong->nTickCount[x];
				fsong->nWTickCount[x] = fsong->nWTickCount[x+1];
				fsong->nTickCount[x] = fsong->nTickCount[x+1];
				fsong->nWTickCount[x+1] = nTemp1;
				fsong->nTickCount[x+1] = dTemp2;
			}
		}
	}
	for(int y = 0; y < fsong->hStep.nSizeBeatSplit; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeBeatSplit-1); x++)
		{
			if(fsong->nWBeatSplit[x] > fsong->nWBeatSplit[x+1])
			{
				int nTemp1 = fsong->nWBeatSplit[x];
				int dTemp2 = fsong->nBeatSplit[x];
				fsong->nWBeatSplit[x] = fsong->nWBeatSplit[x+1];
				fsong->nBeatSplit[x] = fsong->nBeatSplit[x+1];
				fsong->nWBeatSplit[x+1] = nTemp1;
				fsong->nBeatSplit[x+1] = dTemp2;
			}
		}
	}
	for(int y = 0; y < fsong->hStep.nSizeStops; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeStops-1); x++)
		{
			if(fsong->nIStops[x] > fsong->nIStops[x+1])
			{
				int nTemp = fsong->nIStops[x];
				int nTemp2 = fsong->nEStops[x];
				fsong->nIStops[x] = fsong->nIStops[x+1];
				fsong->nEStops[x] = fsong->nEStops[x+1];
				fsong->nIStops[x+1] = nTemp;
				fsong->nEStops[x+1] = nTemp2;
			}
		}
	}
	for(int y = 0; y < fsong->hStep.nSizeChangeVel; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeChangeVel-1); x++)
		{
			if(fsong->nIChangeVel[x] > fsong->nIChangeVel[x+1])
			{
				int nTemp1 = fsong->nIChangeVel[x];
				int nTemp2 = fsong->nEChangeVel[x];
				float dTemp1 = fsong->fChangeVel[x];
				float dTemp2 = fsong->fIChangeVel[x];
				fsong->nIChangeVel[x] = fsong->nIChangeVel[x+1];
				fsong->nEChangeVel[x] = fsong->nEChangeVel[x+1];
				fsong->fChangeVel[x] = fsong->fChangeVel[x+1];
				fsong->fIChangeVel[x] = fsong->fIChangeVel[x+1];
				fsong->nIChangeVel[x+1] = nTemp1;
				fsong->nEChangeVel[x+1] = nTemp2;
				fsong->fChangeVel[x+1] = dTemp1;
				fsong->fIChangeVel[x+1] = dTemp2;
			}
		}
	}
	for(int y = 0; y < fsong->hStep.nSizeJumps; y++)
	{
		for(int x = 0; x < (fsong->hStep.nSizeJumps-1); x++)
		{
			if(fsong->nIJumps[x] > fsong->nIJumps[x+1])
			{
				int nTemp1 = fsong->nIJumps[x];
				int nTemp2 = fsong->nEJumps[x];
				fsong->nIJumps[x] = fsong->nIJumps[x+1];
				fsong->nEJumps[x] = fsong->nEJumps[x+1];
				fsong->nIJumps[x+1] = nTemp1;
				fsong->nEJumps[x+1] = nTemp2;
			}
		}
	}
}

void CMaxDocument::ResizeSteps(int nGroup, int nSize)
{
	if(nGroup < 0 || nGroup >= song.hStep.nSizeArrowGroups) return;
	int nBefSize = song.hStepGroup[nGroup].hGroup.nSizeArrows;
	song.hStepGroup[nGroup].hGroup.nSizeArrows = nSize;

	if(!Actualizate()) return;

	if(nSize > 0 && nSize > nBefSize)
	{
		memset((song.hStepGroup[nGroup].nArrows+nBefSize*m_nArrowsPerLine),
			   0,
			   (nSize-nBefSize)*m_nArrowsPerLine*sizeof(int));
		memset((song.hStepGroup[nGroup].nPArrows+nBefSize*m_nArrowsPerLine),
			   0,
			   (nSize-nBefSize)*m_nArrowsPerLine*sizeof(int));
	}
}

int CMaxDocument::VerificateMod(int nMod, int y, int by)
{
	int nMinY = min(y, by),
		nMaxY = max(y, by);
	int nSize, nSizeInt, nSizeFloat;
	int *nInt;
	float *fFloat;
	int nIW, nEW;
	GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
	GetModWhere(nMod, &nIW, &nEW);

	for(int y = 0; y < nSize; y++)
	{
		GetMod(nMod, y, &nInt, &fFloat);
		if(INSIDE_OF_OUTSIDE(nInt, nMinY, nMaxY, nIW, nEW))
		{
			FreeMod(&nInt, &fFloat);
			return y;
		}
		FreeMod(&nInt, &fFloat);
	}
	return -1;
}

int CMaxDocument::VerificateMod(int nMod, int y, int by, int i)
{
	if(i < 0) return VerificateMod(nMod, y, by);
	int nMinY = min(y, by),
		nMaxY = max(y, by);
	int nSize, nSizeInt, nSizeFloat;
	int *nInt;
	float *fFloat;
	int nIW, nEW;
	GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
	GetModWhere(nMod, &nIW, &nEW);

	for(int y = i; y < nSize; y++)
	{
		GetMod(nMod, y, &nInt, &fFloat);
		if(INSIDE_OF_OUTSIDE(nInt, nMinY, nMaxY, nIW, nEW) && y != i)
		{
			FreeMod(&nInt, &fFloat);
			return y;
		}
		FreeMod(&nInt, &fFloat);
	}
	return -1;
}

void CMaxDocument::RegArrows(int m_nX, int m_nY, int m_nBackX, int m_nBackY, int m_nFamily, int m_nGroup, int nReg)
{
	int nMinX = min(m_nX, m_nBackX),
		nMinY = min(m_nY, m_nBackY),
		nMaxX = max(m_nX, m_nBackX),
		nMaxY = max(m_nY, m_nBackY);
	int nGroup = m_nGroup;

	MAX_STACKELEM elem;
	elem.hElem.bIsFile = false;	// Isn't a file
	elem.hElem.nFile = 0;		// NO FILE! FUCK
	elem.hElem.nFamily = 1;		// Aplicate to arrows
	// Aplicate to this group, at this coords
	elem.hElem.nGroup = nGroup;
	elem.hElem.nMinX = nMinX;
	elem.hElem.nMaxX = nMaxX;
	elem.hElem.nMinY = nMinY;
	elem.hElem.nMaxY = nMaxY;
	// Write UNDO data (similar as copy)
	int nW = nMaxX - nMinX + 1;
	int nH = nMaxY - nMinY + 1;
	if(nW*nH <= 0) return;
	elem.nArrows = (int*)malloc(nW*nH*sizeof(int));
	elem.nPArrows = (int*)malloc(nW*nH*sizeof(int));
	for(int y = 0; y < nH; y++)
	{
		memcpy(elem.nArrows+y*nW,
			song.hStepGroup[nGroup].nArrows+(nMinY+y)*m_nArrowsPerLine+nMinX,
			nW*sizeof(int));
		memcpy(elem.nPArrows+y*nW,
			song.hStepGroup[nGroup].nPArrows+(nMinY+y)*m_nArrowsPerLine+nMinX,
			nW*sizeof(int));
	}
	// Finally, register it! (MUST NOT FEEE DATA)
	if(nReg < 0) RegAction(&elem);
	else RegAction(&elem, nReg);
}

void CMaxDocument::RegMods(int m_nX, int m_nY, int m_nBackX, int m_nBackY, int m_nFamily, int m_nGroup, int nReg)
{
	int nMinY = min(m_nY, m_nBackY),
		nMaxY = max(m_nY, m_nBackY);
	int nMod = m_nGroup;

	MAX_STACKELEM elem;
	elem.hElem.bIsFile = false;	// Isn't a file
	elem.hElem.nFile = 0;		// NO FILE! FUCK
	elem.hElem.nFamily = 2;		// Aplicate to mods
	// Aplicate to this group, at this coords
	elem.hElem.nGroup = nMod;
	elem.hElem.nMinX = 0;
	elem.hElem.nMaxX = 0;
	// Write UNDO data (similar as copy)

	vector<STEPMODINFO*> smiVector;
	int nSize;
	int *nInt;
	float *fFloat;
	int nIW, nEW;
	elem.hMod = (STEPMODHEADER*) malloc(sizeof(STEPMODHEADER));
	GetParMod(nMod, &nSize, &elem.hMod->nSizeInt, &elem.hMod->nSizeFloat);
	GetModWhere(nMod, &nIW, &nEW);

	for(int y = 0; y < nSize; y++)
	{
		GetMod(nMod, y, &nInt, &fFloat);
		if(INSIDE_OF_OUTSIDE(nInt, nMinY, nMaxY, nIW, nEW))
		{
			STEPMODINFO* e;
			e = (STEPMODINFO*)malloc(sizeof(STEPMODINFO));
			if(elem.hMod->nSizeInt > 0)
			{
				e->nModInt = (int*)malloc(elem.hMod->nSizeInt*sizeof(int));
				memcpy(e->nModInt, nInt, elem.hMod->nSizeInt*sizeof(int));
			}
			if(elem.hMod->nSizeFloat > 0)
			{
				e->fModFloat = (float*)malloc(elem.hMod->nSizeFloat*sizeof(float));
				memcpy(e->fModFloat, fFloat, elem.hMod->nSizeFloat*sizeof(float));
			}
			smiVector.push_back(e);
			FreeMod(&nInt, &fFloat);
		}
	}

	elem.hMod->nSize = (int)smiVector.size();
	elem.hMod->nType = nMod;	// YES! THE SAME

	if(elem.hMod->nSize > 0)
	{
		elem.hModInfo = (STEPMODINFO*)malloc(elem.hMod->nSize*sizeof(STEPMODINFO));
		for(int y = 0; y < elem.hMod->nSize; y++)
		{
			elem.hModInfo[y].nModInt = smiVector.at(y)->nModInt;
			elem.hModInfo[y].fModFloat = smiVector.at(y)->fModFloat;
			free(smiVector[y]);
		}
	}

	// Register the corners

	elem.hElem.nMinY = nMinY;
	elem.hElem.nMaxY = nMaxY;

	// Finally, register it! (MUST NOT FREE DATA)
	if(nReg < 0) RegAction(&elem);
	else RegAction(&elem, nReg);
}

void CMaxDocument::RegSong(STEP* fsong)
{
	m_bSaved = false;
	static int nCurse = 0;
	CStdStringA str;

	str.Format("UNDO%d.sksf", nCurse);

	MAX_STACKELEM elem;
	elem.hElem.bIsFile = true;	// Is a file
	elem.hElem.nFile = str.GetLength();		// YEA! FILE
	elem.hElem.nFamily = 0;		// Aplicate to nothing
	// Aplicate to this group, at this coords
	elem.hElem.nGroup = 0;
	elem.hElem.nMinX = 0;
	elem.hElem.nMaxX = 0;
	// Write UNDO data
	TCHAR chCore[MAX_PATH], chCur[MAX_PATH];
	GetMaxProcessDirectory(chCore);
	GetMaxCurrentDirectory(chCur);
	SetMaxCurrentDirectory(chCore);
	SaveRAW_SKSF2(str, fsong);
	SetMaxCurrentDirectory(chCur);
	elem.chFile = (char*)malloc((size_t)elem.hElem.nFile);
	strcpy(elem.chFile, str);

	nCurse++;
}

void CMaxDocument::RegAction(MAX_STACKELEM* elem)
{
	m_bSaved = false;
	int s = m_sevStack.size();
	for(int i = m_nCurStack; i < s; i++)
	{
		DelAction(m_nCurStack);
	}
	RegAction(elem, m_nCurStack);
	m_nCurStack++;
	s = m_sevStack.size();
	if(s > MAX_UNDOREDO_STACK)
	{
		for(int i = MAX_UNDOREDO_STACK; i < s; i++)
		{
			DelAction(0);
		}
		m_nCurStack = MAX_UNDOREDO_STACK;
	}
}

void CMaxDocument::RegAction(MAX_STACKELEM* elem, int n)
{
	m_bSaved = false;
	int s = m_sevStack.size();
	if(n > s) return;
	if(n < s)
	{
		DelAction(n);
	}
	MAX_STACKELEM* e = (MAX_STACKELEM*)malloc(sizeof(MAX_STACKELEM));
	memcpy(e, elem, sizeof(MAX_STACKELEM));
	m_sevStack.insert(m_sevStack.begin()+n, e);
}

void CMaxDocument::AppAction(int n, int bBackUp)
{
	MAX_STACKELEM* elem = m_sevStack.at(n);
	int m_nX = elem->hElem.nMaxX;
	int m_nY = elem->hElem.nMaxY;
	int m_nBackX = elem->hElem.nMinX;
	int m_nBackY = elem->hElem.nMinY;
	if(elem->hElem.bIsFile)
	{
		Close();	// Perform data delete
		OpenRAW_SKSF2(elem->chFile, &song);
		InitAux();
		Actualizate();
	}
	else if(elem->hElem.nFamily == 1)
	{
		int nGroup = elem->hElem.nGroup;
		if(bBackUp >= 0) RegArrows(m_nX, m_nY, m_nBackX, m_nBackY, 1, nGroup, bBackUp);

		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		int nW = m_nArrowsPerLine - nMinX,
			nH = song.hStepGroup[nGroup].hGroup.nSizeArrows - nMinY;

		int naW = nMaxX - nMinX + 1,
			naH = nMaxY - nMinY + 1;

		nW = min(nW, naW);
		nH = min(nH, naH);

		for(int y = 0; y < nH; y++)
		{
			memcpy(song.hStepGroup[nGroup].nArrows+(nMinY+y)*m_nArrowsPerLine+nMinX,
				elem->nArrows+y*naW,
				nW*sizeof(int));
			memcpy(song.hStepGroup[nGroup].nPArrows+(nMinY+y)*m_nArrowsPerLine+nMinX,
				elem->nPArrows+y*naW,
				nW*sizeof(int));
		}
	}
	else if(elem->hElem.nFamily == 2)
	{
		int nMod = elem->hElem.nGroup;
		int nMinY = min(m_nY, m_nBackY),
			nMaxY = max(m_nY, m_nBackY);
		int nSize, nSizeInt, nSizeFloat;
		int nIW, nEW;

		GetModWhere(nMod, &nIW, &nEW);
		GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);

		// Do backup in UNDO
		if(bBackUp >= 0) RegMods(m_nX, m_nY, m_nBackX, m_nBackY, 1, nMod, bBackUp);

		// Erase mods between nMinY and nMaxY
		while(1)
		{
			int y = VerificateMod(nMod, nMinY, nMaxY);
			if(y == -1) break;
			DelMod(nMod, y);
		}

		// Get Size again
		GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);

		for(int i = 0; i < elem->hMod->nSize; i++)
		{
			bool bInc = true;
			int *nIntI = NULL;
			float* fFloatF = NULL;

			if(nSizeInt > 0)
			{
				nIntI = (int*)malloc(nSizeInt*sizeof(int));
				memcpy(nIntI, elem->hModInfo[i].nModInt, nSizeInt*sizeof(int));
			}
			if(nSizeFloat > 0)
			{
				fFloatF = (float*)malloc(nSizeFloat*sizeof(float));
				memcpy(fFloatF, elem->hModInfo[i].fModFloat, nSizeFloat*sizeof(float));
			}

			if(elem->hModInfo[i].nModInt[nIW] >= 0)
			{
				nIntI[nIW] = elem->hModInfo[i].nModInt[nIW];
				nIntI[nEW] = elem->hModInfo[i].nModInt[nEW];
				AddMod(nMod, nIntI, nSizeInt, fFloatF, nSizeFloat);
			}
			if(nSizeInt > 0) free(nIntI);
			if(nSizeFloat > 0) free(fFloatF);
		}
	}
}

void CMaxDocument::Undo(void)
{
	if(!CanUndo()) return;
	AppAction(m_nCurStack-1, m_nCurStack);
	m_nCurStack--;
}

void CMaxDocument::Redo(void)
{
	if(!CanRedo()) return;
	AppAction(m_nCurStack+1, m_nCurStack);
	m_nCurStack++;
}

bool CMaxDocument::CanUndo(void)
{
	bool bCan = true;
	if(m_nCurStack <= 0 || m_sevStack.size() == 0) bCan = false;
	return bCan;
}

bool CMaxDocument::CanRedo(void)
{
	bool bCan = true;
	int s = m_sevStack.size();
	if(m_nCurStack >= (s-1)) bCan = false;
	return bCan;
}

void CMaxDocument::DelStackUndo(void)
{
	vector<MAX_STACKELEM*>::iterator it;
	for(it = m_sevStack.begin(); it < m_sevStack.end(); it++)
	{
		FreeAction(*it);
	}
	m_sevStack.clear();
}

void CMaxDocument::DelAction(int n)
{
	int s = m_sevStack.size();
	if(n >= s) return;
	vector<MAX_STACKELEM*>::iterator it=m_sevStack.begin()+n;

	FreeAction(*it);

	m_sevStack.erase(it);
}

void CMaxDocument::FreeAction(MAX_STACKELEM* elem)
{
	if(elem->hElem.bIsFile)
	{
		remove(elem->chFile);
		free(elem->chFile);
	}
	else if(elem->hElem.nFamily == 1)
	{
		free(elem->nArrows);
		free(elem->nPArrows);
	}
	else if(elem->hElem.nFamily == 2)
	{
		if(elem->hMod->nSize > 0)
		{
			if(elem->hMod->nSizeInt > 0) free(elem->hModInfo->nModInt);
			if(elem->hMod->nSizeFloat > 0) free(elem->hModInfo->fModFloat);
			free(elem->hModInfo);
		}
		free(elem->hMod);
	}
	free(elem);
}

bool CMaxDocument::SomeNote(int y, int n)
{
	if(n < 0 || n >= song.hStep.nSizeArrowGroups) return false;
	if(y < 0 || y >= song.hStepGroup[n].hGroup.nSizeArrows) return false;
	for(int x = 0; x < song.hStep.nArrowsPerLine; x++)
	{
		if(GetArrow(x, y, n) >= 1 && GetArrow(x, y, n) <= 20) return true;
	}
	return false;
}

bool CMaxDocument::SomeBSQuestion(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > song.hStep.nSizeArrowGroups) return false;
	for(int x = 0; x < song.hStep.nArrowsPerLine; x++)
	{
		int dw = GetArrow(x, y, nGArrow);
		if(dw==21)
			bReturn = true;
	}
	return bReturn;
}

bool CMaxDocument::SomeBSAnswer(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > song.hStep.nSizeArrowGroups) return false;
	for(int x = 0; x < song.hStep.nArrowsPerLine; x++)
	{
		int dw = GetArrow(x, y, nGArrow);
		if(dw==22)
			bReturn = true;
	}
	return bReturn;
}

bool CMaxDocument::SomeBSPopUp(int y, int nGArrow)
{
	bool bReturn = false;
	if(nGArrow < 0 || nGArrow > song.hStep.nSizeArrowGroups) return false;
	for(int x = 0; x < song.hStep.nArrowsPerLine; x++)
	{
		int dw = GetArrow(x, y, nGArrow);
		if(dw==23)
			bReturn = true;
	}
	return bReturn;
}
